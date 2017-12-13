#ifndef PWM2_H
#define PWM2_H
/*
 this uses the 8-bit Timer/Counter 2 to provide up to two PWM channels and one periodic interrupt
 these can be independently configured other than a shared clock prescaler

 the two PWM ouput pins are OC2A & OC2B.  
 on the ATMega328:
    OC2A is on pin PB3 (which is also used as MOSI for the SPI interface)
    OC2B is on pin PD3 
 
 the periodic interrupt is TIMER2_OVF
 if interrupt is enabled client code is responisble for implementing ISR(TIMER2_OVF_vect) 
 
 the package interface is:

 init   - configures timer/counter 2 registers for PWM operation with specified clock prescaler
 uninit - resets timer/counter 2 registers back to the power-on-default values
 
 setPwmA - sets 8-bit PWM for channel A  duty cycle as follows: 0=off, 255=on, else duty_cycle= (val+1)/256
 setPwmB - sets 8-bit PWM for channel B

 disablePwmA  - resets OC2A pin to the power-on-default state
 disablePwmB  - resets OC2B pin to the power-on-default state

 enablePeriodicInterrupt - enables TIMER_OVF interrupt at frequency (master_io_clk / prescaler / 256).
 disablePeriodicInterrupt - disables interrupt
*/

class pwm2 {
  public:

    enum ClockPrescaler { clock_off, clock_by1, clock_by8, clock_by32, clock_by64, clock_by128, clock_by256, clock_by1024 };

    static void init( pwm2::ClockPrescaler prescale = clock_by1 );
    static void setClockPrescaler( pwm2::ClockPrescaler prescale );
    static void uninit(); 

    static void setPwmA( unsigned char pwm );   // duty cycle as follows: 0=off, 255=on, else dutyFactor= (pwm+1)/256
    static void setPwmB( unsigned char pwm );
    
    static float calculateDutyFactor( unsigned char pwm );  // duty factor is between 0.0 and 1.0
    static unsigned char dutyFactorToPwm( float dutyFactor); // returns the pwm with duty factor nearest the requested value

    static void disablePwmA();    // don't disable a channel unless you have set it
    static void disablePwmB();    // don't disable a channel unless you have set it

    static void enablePeriodicInterrupt();
    static void disablePeriodicInterrupt();
    
};
/*
 additional design notes:

 setPwm sets as an output pin the pin associated with OCR2A or OCR2B.
 disablePwm resets the pin back to an input pin.  Do not call this unless you have set that pin
 since if you are not using that pin some other function (e.g. SPI) might need it to remain an output pin.
 uninit() only resets the timer/counter resgisters and does not reset any pins to input.

 the Arduino library tone() and notone() function collide with this timer and thus must be avoided
 */
#endif
