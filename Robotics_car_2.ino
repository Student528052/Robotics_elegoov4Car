#include "Motors.h"
#include "IR_sensors.h"
#include "Ultrasonic.h"
#include "Motor_control.cpp"
#include <Arduino.h>
Motors motors;
IR_sensors IR; 
Motor_Control M_Control; 

void setup() {
  // put your setup code here, to run once:
  motors.Motor_init(); 
  IR.IR_init(); 
  Serial.begin(9600); 
  M_Control.Motion_Control = 1; 
  Drive(M_Control.Motion_Control, 150);  

}

void loop() {
  delay(1000); 
  M_Control.Motion_Control = 8; //stop
  Drive(M_Control.Motion_Control, 150); 
  Serial.print("\n Left");
  Serial.print(IR.IR_Read_L());
  Serial.print("  Medium ");
  Serial.print(IR.IR_Read_M());
  Serial.print("  Right ");
  Serial.print(IR.IR_Read_R());


}
