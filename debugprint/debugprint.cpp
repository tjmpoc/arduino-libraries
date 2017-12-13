#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "Arduino.h"

#include "debugprint.h"

void debugPrint_PowerAndSleepModes()
{
  Serial.println(); Serial.println("Power and Sleep Modes");
  Serial.print("SMCR 0x");  Serial.println(SMCR, HEX);
  Serial.print("MCUCR 0x");  Serial.println(MCUCR, HEX);
  Serial.print("PRR 0x");  Serial.println(PRR, HEX);
}

void debugPrint_IOPorts()
{
  Serial.println(); Serial.println("Port B");
  Serial.print("DDRB 0x");  Serial.println(DDRB, HEX);
  Serial.print("PORTB 0x");  Serial.println(PORTB, HEX);
  Serial.print("DDRC 0x");  Serial.println(DDRC, HEX);
  Serial.print("PORTC 0x");  Serial.println(PORTC, HEX);
  Serial.print("DDRD 0x");  Serial.println(DDRD, HEX);
  Serial.print("PORTD 0x");  Serial.println(PORTD, HEX);
  
}

void debugPrint_TimerCounter1()
{
  unsigned int val16;
  Serial.println(); Serial.println("Timer/Counter 1");
  Serial.print("TCCR1A 0x");  Serial.println(TCCR1A, HEX);
  Serial.print("TCCR1B 0x");  Serial.println(TCCR1B, HEX);
  Serial.print("TCCR1C 0x");  Serial.println(TCCR1C, HEX);
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { val16 = TCNT1; }
  Serial.print("TCNT1 0x");  Serial.println(val16, HEX);
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { val16 = OCR1A; }
  Serial.print("OCR1A 0x");  Serial.println(val16, HEX);
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { val16 = OCR1B; }
  Serial.print("OCR1B 0x");  Serial.println(val16, HEX);
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { val16 = ICR1; }
  Serial.print("ICR1 0x");  Serial.println(val16, HEX);
  Serial.print("TIMSK1 0x");  Serial.println(TIMSK1, HEX);
  Serial.print("TIFR1 0x");  Serial.println(TIFR1, HEX);
}


void debugPrint_TimerCounter2()
{
  Serial.println(); Serial.println("Timer/Counter 2");
  Serial.print("TCCR2A 0x");  Serial.println(TCCR2A, HEX);
  Serial.print("TCCR2B 0x");  Serial.println(TCCR2B, HEX);
  Serial.print("TCNT2 0x");  Serial.println(TCNT2, HEX);
  Serial.print("OCR2A 0x");  Serial.println(OCR2A, HEX);
  Serial.print("OCR2B 0x");  Serial.println(OCR2B, HEX);
  Serial.print("TIMSK2 0x");  Serial.println(TIMSK2, HEX);
  Serial.print("TIFR2 0x");  Serial.println(TIFR2, HEX);
  Serial.print("ASSR 0x");  Serial.println(ASSR, HEX);
  Serial.print("GTCCR 0x");  Serial.println(GTCCR, HEX);
}

void debugPrint_AnalogToDigitalConverter()
{
  Serial.println(); Serial.println("Analog to Digital Converter");
  Serial.print("ADMUX 0x");  Serial.println(ADMUX, HEX);
  Serial.print("ADCSRA 0x");  Serial.println(ADCSRA, HEX);
  Serial.print("ADCL 0x");  Serial.println(ADCL, HEX);
  Serial.print("ADCH 0x");  Serial.println(ADCH, HEX);
  Serial.print("ADCSRB 0x");  Serial.println(ADCSRB, HEX);
  Serial.print("DIDR0 0x");  Serial.println(DIDR0, HEX);
}


