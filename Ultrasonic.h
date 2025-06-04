#pragma once
 #include <Arduino.h>                                                                               
  /*ULTRASONIC*/
  
  //#include <NewPing.h>
  class Ultrasonic
  {
  public:
    void Ultrasonic_Init(void);
    unsigned int Ultrasonic_Read();
  
  private:
  #define TRIG_PIN 13      // Arduino pin tied to trigger pin on the ultrasonic sensor.
  #define ECHO_PIN 12      // Arduino pin tied to echo pin on the ultrasonic sensor.
  #define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor d    istance is rated at 400-500cm.
  };
  