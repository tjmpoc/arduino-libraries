#ifndef ADC2_H
#define ADC2_H
/*
 this exposes the ATMega328 ADC sub-system

there are two modes of operation: autotrigger and synchronous

autotrigger mode is non-blocking
  first call startAutotrigger to configures the sub-system to auto-measure from the analog source
  then call reread to return the most recent such measurement (non-blocking)
  autotrigger mode is typically the easiest way to go if you are reading multiple measurements from the same source

synchronous mode is blocking
  the readSynchronous function initialtes the specified measurement and does not return until the measurement is ready

the ATMega328 has a few measurement gotchas:

  the first couple of measurements after starting a new source appear to not be very accurate.  
    with the autotrigger mode this can be acomplished by letting the system run for a few measurement cycles before using the measurements.  
      each measurement is 13 ADC cycles (i.e. one prescale of CPU clocks) so with 16 mhz cpu clk and prescale_64 this is 13*64/16 usec = 52 usec
    with synchronous mode this can be acomplished by calling readSynchronous several times without changing analog sources and ignoring the first few (e.g. two) measurements

  effetive resolution appears to drop off at higher ADC clock rates.  an ADC clock rate of 250 mhz or lower is recommended and 1 mhz is maximum.  
    with a 16mhz CPU clock these values are realized using prescale_64 (default) and prescale_16, respectively

*/

class adc2 {
  public:

    enum AnalogSource { ADC0=0, ADC1, ADC2, ADC3, ADC4, ADC5, ADC6, ADC7, Temperature_sensor, V_1_1=14, Gnd=15 };
    enum ClockPrescaler { Prescale_2=1, Prescale_4, Prescale_8, Prescale_16, Prescale_32, Prescale_64, Prescale_128 };
    enum VoltageReference { Reference_AREF=0, Reference_AVcc, Reference_1_1_v=3 };
 
    static void setClockPrescaler( adc2::ClockPrescaler clockPrescaler );  	// if not otherwise specified, the default is Prescale_64
    static void setVoltageReference( adc2::VoltageReference voltageReference ); // if not otherwise specified, the default is AVcc

    // once autotrigger has been started a call to reread will return the most recent measurement without blocking
    // it is the easiest and most efficient method when reading the same channel repeatedly
    // note: the first read or two from a analog source appear to be invalid, so best let autotrigger run for a few measurements before using it (13 ADC cycles/measurement & ADC cycle depends on prescaler)
    static void startAutotrigger( adc2::AnalogSource analogSource );
    static int reread();

    // sets up the conversion, blocks till the answer is ready, and returns it
    // a call to readSingle will stop autoTrigger if started
    // note: the first read or two after starting or switching analog source appear to be invalid, after that repeated reads from the same source appear valid
     static int readSynchronous(adc2::AnalogSource analogSource );

    // resets the sub-system registers to the power-on-default values
    static void reset();

  private:
    static adc2::VoltageReference	voltageReference;
    static adc2::ClockPrescaler 	clockPrescaler;

    // a different usage mode is to call startConversion with autoTrigger=true and then use readConversionResult repeatedly with no intervening calls to startConversion.
    // after the first conversion, readConversionResult will return immediately with the most recent measurement (i.e. no blocking)
    static void startConversion( adc2::AnalogSource analogSource, bool autoTrigger );
    static void blockTillConversionDone();
    static int readConversionResult(); 
 

};
/*
additional design notes:

FIXME WHAT ABOUT THE PINS?  WHO DOES THAT?
*/
#endif
