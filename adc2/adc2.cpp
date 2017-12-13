#include <avr/io.h>
#include "adc2.h"

// static class variables must be declared so that space can be allocated for them
static adc2::VoltageReference adc2::voltageReference = adc2::Reference_AVcc;
static adc2::ClockPrescaler adc2::clockPrescaler = adc2::Prescale_64;

// values set with these configuration calls are used within startAutotrigger and readSynchronous
static void adc2::setClockPrescaler( adc2::ClockPrescaler clockPrescaler )
{
  adc2::clockPrescaler = clockPrescaler;
}

static void adc2::setVoltageReference( adc2::VoltageReference voltageReference )
{
  adc2::voltageReference = voltageReference;
}

// start autoTrigger mode
static void adc2::startAutotrigger( adc2::AnalogSource analogSource )
{
  adc2::startConversion( analogSource, true );
  adc2::blockTillConversionDone();
}

// this will return the last autotrigger measurement without blocking
static int adc2::reread()
{
  return adc2::readConversionResult();
}

// sets up the conversion, waits till the answer is ready and returns it
static int adc2::readSynchronous(adc2::AnalogSource analogSource)
{
  adc2::startConversion( analogSource, false );
  adc2::blockTillConversionDone();
  return adc2::readConversionResult();
}

// resets the ADC sub-system registers to the power-on-default state
static void adc2::reset()
{
  ADCSRA = 0; // zero this one first
  ADMUX = ADCSRB = DIDR0 = ADCH = ADCL = 0; 
}

// -------------------------private class functions-----------------------------

static void adc2::startConversion( adc2::AnalogSource analogSource, bool autoTrigger )
{
  // power up the ADC sub-system by writing 0 in PRR.PRADC
  PRR &= ~ (1<<PRADC);

  // set the voltage reference (high two bits) and measurement source (bottom 3 bits)
  ADMUX = (adc2::voltageReference << 6) | analogSource ;

  // if we use auto-trigger, the mode will be "free-running"
  ADCSRB = 0;

  // if the analog source is an external pin, then we disable the digital input buffer for that pin (saves power)
  if ( ADC7 >= analogSource )
    DIDR0 |= ( 1 << analogSource );

  // if ADCSRA.ADIF is set clear it by writing a logical 1 to that bit (clearing a bit by setting it - how unusual)
  if ( (1<<ADIF) & ADCSRA )
    ADCSRA |= (1<<ADIF);

  // initiate the conversion
  ADCSRA = (1<<ADEN) | (1<<ADSC) | (autoTrigger ? (1<<ADATE) : 0) | adc2::clockPrescaler;

  // note that the ADSC bit in this register will be 1 while the conversion is in progress and 0 when it is done
  // in the case of autotrigger it will stay 1 after startAutotrigger returns (since we are not using an ADC ISR) 
}

static int adc2::readConversionResult()
{
  // must read ADCL first to lock register until ADCH is read
  unsigned char low, high;
  low  = ADCL;
  high = ADCH;
  return (high << 8) | low;
}

// block using the ADCSRA.ADIF bit (see discussion above)
static void adc2::blockTillConversionDone()
{
  // ADSRA.ADIF is set when conversion is completed
  while ( !(1<<ADIF) & ADCSRA);    
}
