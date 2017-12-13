#include <avr/io.h>
#include <avr/interrupt.h>
#include "Arduino.h"

#include "pwm2.h"

static void pwm2::init( pwm2::ClockPrescaler prescale )
{
  // first set to power-on-default values (it appears the arduino is initializing timer for the tone fn)
  pwm2::uninit();  
  // now set wave generation mode = 3 & clock_select = prescaler
  TCCR2A = (1<<WGM21) | (1<<WGM20);
  TCCR2B = prescale;
}

static void pwm2::setClockPrescaler( pwm2::ClockPrescaler prescale )
{
  // change bottom 3 bits of TCCR2B according to prescale
  unsigned char val = TCCR2B;
  val &= ~ ((1<<CS22)|(1<<CS21)|(1<<CS20));
  val |= ( prescale & ((1<<CS22)|(1<<CS21)|(1<<CS20))); 
  TCCR2B = val;
}

static void pwm2::uninit()
{
  TIMSK2 = 0;  // default interupts
  TCCR2A = TCCR2B = 0;
  OCR2A = OCR2B = 0;
}

static void pwm2::setPwmA( unsigned char pwm )
{
  if ( 0 == pwm )
  {
    // disconnect pin from OC2 and drive directly
    PORTB &= ~(1<<PORTB3);
    TCCR2A &= ~ ( (1<<COM2A1) | (1<<COM2A0) );
  }
  else if (0xff == pwm )
  {
    // disconnect pin from OC2 and drive directly
    PORTB |= (1<<PORTB3);
    TCCR2A &= ~ ( (1<<COM2A1) | (1<<COM2A0) );
  }
  else
  {
    unsigned char val = TCCR2A;
    val &= ~ ( 1 << COM2A0 );
    val |= ( 1 << COM2A1 );
    TCCR2A = val;
    OCR2A = pwm;
  }
  // make sure this pin is an output pin
  DDRB |= (1<<DDB3);
}

static void pwm2::setPwmB( unsigned char pwm )
{
  if ( 0 == pwm )
  {
    // disconnect pin from OC2 and drive directly
    PORTD &= ~(1<<PORTD3);
    TCCR2A &= ~ ( (1<<COM2B1) | (1<<COM2B0) );
  }
  else if (0xff == pwm )
  {
    // disconnect pin from OC2 and drive directly
    PORTD |= (1<<PORTD3);
    TCCR2A &= ~ ( (1<<COM2B1) | (1<<COM2B0) );
  }
  else
  {
    unsigned char val = TCCR2A;
    val &= ~ ( 1 << COM2B0 );
    val |= ( 1 << COM2B1 );
    TCCR2A = val;
    OCR2B = pwm;
  }
  // make sure this pin is an output pin
  DDRD |= (1<<DDD3);
}

static float pwm2::calculateDutyFactor( unsigned char pwm )
{
  if (0==pwm)
    return 0.0;
  if (255==pwm)
    return 1.0;
  return ((pwm+1)/256.0);
}

static unsigned char pwm2::dutyFactorToPwm( float dutyFactor)
{
  float fval = ( dutyFactor * 256.0 ) - 1.0;
  int val = int( fval);
  if ( 0 > val )
    val = 0;
  else if ( 255 < val )
    val = 255;
  return ((unsigned char) val);

  
}


static void pwm2::disablePwmA()
{
  // make input pin
  DDRB &= ~(1<<DDB3);
}

static void pwm2::disablePwmB()
{
  // make input pin
  DDRD &= ~(1<<DDD3);
}

static void pwm2::enablePeriodicInterrupt()
{
  TIMSK2 = (1<<TOIE2);
}

static void pwm2::disablePeriodicInterrupt()
{
  TIMSK2 = 0;  
}


