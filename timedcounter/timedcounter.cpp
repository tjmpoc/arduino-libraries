#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "Arduino.h"

#include "timedcounter.h"

// these variables are changed by the ISR and thus must be declared "static volatile"
// also since they are multiunsigned char access must be atomic (ref: ?)
static volatile unsigned long counterIsrInterval;   // time between last and previous isr call in usec
static volatile unsigned long counterIsrTimeStamp;  // time of last isr call in usec (from a call to micros())


static unsigned char  timedCounter::pulsesPerCycle;        // used to compute hertz/rpm
static unsigned int   timedCounter::cyclesPerInterupt;     // used to compute hertz/rpm
static unsigned long  timedCounter::timeoutInMicroseconds; // used to validate isr data before computing hertz/rpm
static bool           timedCounter::enablePullOnInputPin;  // the chip can provide pullup resistor on input pin
static bool           timedCounter::triggerOnRisingEdge;  // used to initialize hardware
static bool           timedCounter::enableDebugPinOC1A;    // toggle OC1 pin at counter match

// this ISR simply captures interval between ISR calls and timestamp of most current call
ISR( TIMER1_COMPA_vect )
{
  unsigned long currentTime = micros();
  counterIsrInterval = currentTime - counterIsrTimeStamp;
  counterIsrTimeStamp = currentTime;
}

static void timedCounter::setConfiguration( unsigned char pulsesPerCycle, unsigned int cyclesPerInterupt, 
	unsigned long timeoutInMicroseconds, bool enablePullOnInputPin, bool triggerOnRisingEdge, bool enableDebugPinOC1A)
{
  timedCounter::pulsesPerCycle = pulsesPerCycle;
  timedCounter::cyclesPerInterupt = cyclesPerInterupt;
  timedCounter::timeoutInMicroseconds = timeoutInMicroseconds;
  timedCounter::enablePullOnInputPin = enablePullOnInputPin;
  timedCounter::triggerOnRisingEdge = triggerOnRisingEdge;
  timedCounter::enableDebugPinOC1A = enableDebugPinOC1A;
}



static void timedCounter::start()
{
  // initialize the timing variables
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    counterIsrInterval = counterIsrTimeStamp = 0;
  }
  
  // clear the PRTIM1 bit in the Power Reduction Register (PRR) to power timer/counter 1
  PRR &= ~(1<<PRTIM1);

  // in TCC1A we optionally set the output compare (OC) bits to toggle a pin on compare (to assist debugging and timing)
  //  unsigned char val = (enableDebugPinOC1A ? (1<<COM1A0) : 0);
  unsigned char val = 0;
  TCCR1A = val;

  // if using the debug pin then make it an output pin
  if (enableDebugPinOC1A)
  {
    DDRB |= (1<<DDB1);
  }
  
  // compute how many edges we need to count per interrupt
  unsigned int transitionsPerInterrupt = pulsesPerCycle * cyclesPerInterupt;
  
  // and write it to the output compare register
  // note that a write to the 16-bit OCR1A register must be atomic (ref Atmel 328p data sheet page 113)
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    OCR1A = transitionsPerInterrupt;
  }

  // in TCC1B set wave generation mode (WGM) bits to clear the counter when it matches the output compare register (OCR)
  // and set clock Select (CS) bits to specify that we are using an external signal to drive the counter and which edge to trigger on
  val = (1<<WGM12) | (1<<CS12) | (1<<CS11) | (triggerOnRisingEdge ? (1<<CS10) : 0);
  TCCR1B = val;

  // 
  // zero the counter (must be atomic)
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    TCNT1 = 0;
  }

  // for the pin used as input to this counter (IN1 = PD5) set as input pin and enable the pull-up resistor
  DDRD &= ~(1<<DDD5);	// clearing this bit makes it an input pin
  PORTD |= (1<<PORTD5);	// when it is an input pin, setting this the bit high enables the pull-up
  
  // enable the interrupt for this counter
  TIMSK1 |= (1<<OCIE1A);
}

static void timedCounter::stop()
{
  // disable the interrupt for this counter
  TIMSK1 &= (~(1<<OCIE1A));
  
  // put the timer into low power mode
  PRR &= (~(1<<PRTIM1));
}


// this uses longs instead of floats if speed is an issue for you
static unsigned long timedCounter::getPeriod()
{
  unsigned long timestamp;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    timestamp = counterIsrTimeStamp;
  }
  
  unsigned long staleness = micros() - timestamp;
  if (  staleness > timeoutInMicroseconds )
  {
    return 0;
  }
  else
  {
  unsigned long interval;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    interval = counterIsrInterval;
  }
    return interval / cyclesPerInterupt;    
  }
}

// hertz 
static float timedCounter::getHertz()
{
  unsigned long period = getPeriod();
  if ( ! period )
  {
    return 0.0;
  }
  else
  {
    float hertz = 1000000.0 / period;  
    return hertz;
  }
}

// rpm - fans speeds are typically displayed as rpm
static float timedCounter::getRpm()
{
  return 60.0 * getHertz();
}


static float timedCounter::calcMinimumHertz()
{
  float minimumHertz = 1000000.0 * cyclesPerInterupt / timeoutInMicroseconds;
}


