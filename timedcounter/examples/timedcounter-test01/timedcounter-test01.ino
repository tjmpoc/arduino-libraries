#include <timedcounter.h>
#include <debugprint.h>

void setup() {
  // initialize serial communications
  Serial.begin(115200);


  // introduce ourselves  
  Serial.println("timedCounter test");

  // initialize timedCounter
  timedCounter::setConfiguration();
  timedCounter::start();

//  debugPrint_PowerAndSleepModes();
//  debugPrint_IOPorts();
//  debugPrint_TimerCounter1();
}

void loop() {
  delay(1000);
  Serial.println(timedCounter::getHertz());
}

