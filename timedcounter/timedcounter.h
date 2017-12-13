#ifndef TIMEDCOUNTER_H
#define TIMEDCOUNTER_H
/*
 measures the frequency of a digital signal averaged over N samples using the hardware timer/counter.
 this was designed to support measuring the RPM of a fan using the signal available from the fan hall effect sensor.

 using the timer/counter 1 we generates an interrupt every N pulses and the ISR records the interval between interrupts.
 an access function connverts the interval time into the desired parameter, e.g. hertz or rpm.

 input is on T1 whiich on the ATMega328 is PD5.  the optional debugging output is on OC1A which on the ATMega328 is PB1.

 Design notes:
a fan may generate multiple pulses per revolution (e.g. 2) [e.g. quadrapole magnet on rotor) 
the setup parameter "pulsesPerCycle" handles this.  It is used in the conversion of signal timing to hertz, rpm, etc.
if measuring rpm of a motor then set to the # of hall sensors.  if measuring a regular digital signal then set to 1.

since this design updates the measurement after timing N samples, thus the occurence of of updated data is a function of RPM.
special handling is required for 0 rpm since the isr will not get called to update the data.
this is handled by recording the timepoint when the ISR was called.  the client can check this for timepoint to a "timeout" condition.
the value used for this "timeout" sets a a minimum rpm the package can measure and an access fn is that does that calcuation.
*/

class timedCounter {
  public:

    // this must be called before calling the start function
    static void setConfiguration( unsigned char pulsesPerCycle = 2, unsigned int cyclesPerInterupt = 8, 
                            unsigned long timeoutInMicroseconds = 2000000, 
                            bool enablePullOnInputPin = true, bool triggerOnRisingEdge = false,
                            bool enableDebugPinOC1A = false);

    // the start fn turns on the system by configuring the hardware and enabling the interrupt
    static void start();
    static void stop();

    // client functions to read the parameter (averaged over N cycles)
    static float getHertz();          // cycles/sec (not equal to pulses/sec unless pulsesPerCycle=1)
    static float getRpm();            // cycles/min
    static unsigned long getPeriod(); // period of one cycle in microseconds

    // based on the configuration there is a minimum rpm/frequency which can be measured
    static float calcMinimumHertz();  // multiply by 60 to get minimum rpm
    
    
  private:
    // configuration
    static unsigned char           pulsesPerCycle;        // used to compute hertz/rpm
    static unsigned int   cyclesPerInterupt;     // used to compute hertz/rpm
    static unsigned long  timeoutInMicroseconds; // used to validate isr data before computing hertz/rpm
    static bool           enablePullOnInputPin;  // the chip can provide pullup resistor on input pin
    static bool           triggerOnRisingEdge;  // used to initialize hardware
    static bool           enableDebugPinOC1A;    // toggle OC1 pin at counter match
    // FIXME: after this package works maybe come back and reduce the RAM usage (e.g. bit fields)
};


/*
additional design notes:

this package was specifically designed to average over N cycles (by using the timer/counter to count to N).
this reduces the processing load and permits very low power deisgns (i.e. sleep a lot).
if measuring every cycle were desired a design using the timer/counter in timer-capture mode would probably be more appropriate.

note that the hall sensor on a standard computer fan has an open collector driver (i.e. pulls to ground only) so typicaly a pull-up resistor is required in the wiring or pin configuration.  this package inplements the pull-up resistor in the pin configuration so an external resistor is not required.
*/

#endif

