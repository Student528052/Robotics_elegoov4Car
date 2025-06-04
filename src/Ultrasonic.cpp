#include "include/Ultrasonic.h"
/*ULTRASONIC*/ 
  //#include <NewPing.h> 
  // NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distanc    e. 
  void Ultrasonic::Ultrasonic_Init(void) 
  { 
    pinMode(ECHO_PIN, INPUT); //Ultrasonic module initialization 
    pinMode(TRIG_PIN, OUTPUT); 
  } 
  unsigned int Ultrasonic::Ultrasonic_Read() 
  { 
    unsigned int tempda_x = 0; 
    digitalWrite(TRIG_PIN, LOW); 
    delayMicroseconds(2); 
    digitalWrite(TRIG_PIN, HIGH); 
    delayMicroseconds(10); 
    digitalWrite(TRIG_PIN, LOW); 
    tempda_x = ((unsigned int)pulseIn(ECHO_PIN, HIGH) / 58); 
    return tempda_x; 
    // sonar.ping() / US_ROUNDTRIP_CM; // Send ping, get ping time in microseconds (uS). 
  } 
   
