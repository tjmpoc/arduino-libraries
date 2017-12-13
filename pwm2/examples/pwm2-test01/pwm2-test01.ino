#include <pwm2.h>
#include <debugprint.h>

void setup() {
  Serial.begin(115200);
//  debugPrint_TimerCounter2();
//  debugPrint_IOPorts();

  Serial.println("pwm2-test01");
  pwm2::init();

/*
  debugPrint_TimerCounter2();
  debugPrint_IOPorts();
  
  delay(5000);
  Serial.println("");Serial.println("calling setPwa(128)");
  pwm2::setPwmA( 128 );
  debugPrint_TimerCounter2();
  debugPrint_IOPorts();

  delay(5000);
  Serial.println("");Serial.println("calling setClockPrescaler(clock_by8)");
  pwm2::setClockPrescaler(pwm2::clock_by8);
  debugPrint_TimerCounter2();
  debugPrint_IOPorts();

  delay(5000);
  Serial.println("");Serial.println("calling setPwa(0)");
  pwm2::setPwmA( 0 );
  debugPrint_TimerCounter2();
  debugPrint_IOPorts();

  delay(5000);
  Serial.println("");Serial.println("calling setPwa(255)");
  pwm2::setPwmA( 255 );
  debugPrint_TimerCounter2();
  debugPrint_IOPorts();

  delay(5000);
  Serial.println("");Serial.println("calling setPwa(16)");
  pwm2::setPwmA( 16 );
  debugPrint_TimerCounter2();
  debugPrint_IOPorts();
*/
}

unsigned char loopVal = 0;

void loop() {
  Serial.print(loopVal);Serial.print("\t");Serial.print(100.0 * pwm2::calculateDutyFactor(loopVal));
  Serial.print("\t");Serial.println(100.0 * pwm2::calculateDutyFactor((unsigned char)(loopVal+128)));
  pwm2::setPwmA( (unsigned char) loopVal );
  pwm2::setPwmB( (unsigned char) (loopVal+128) );
  loopVal++;
  delay(2000);
}
