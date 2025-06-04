#include "Motors.h"
#include "IR_sensors.h"
#include "Ultrasonic.h"
#include "Motor_control.cpp"
#include <Arduino.h>
#define BLACK 100
#define WHITE 500

Motors motors;
IR_sensors IR; 
Motor_Control M_Control; 
void setup() {
  // put your setup code here, to run once:
  motors.Motor_init(); 
  IR.IR_init(); 
  Serial.begin(9600); 
  M_Control.Motion_Control = ControlCommands::Forward; 
  Drive(M_Control.Motion_Control, 150);  

}

void steer(uint8t time ){
  auto left = IR.IR_Read_L(); 
  auto right = IR.IR_Read_R(); 
  auto center = IR.IR_Read_M(); 
  if()
}
void loop() {
  delay(1000); 
  M_Control.Motion_Control = ControlCommands::stop_it; 
  Drive(M_Control.Motion_Control, 150); 
  Serial.print("\n Left");
  Serial.print(IR.IR_Read_L());
  Serial.print("  Medium ");
  Serial.print(IR.IR_Read_M());
  Serial.print("  Right ");
  Serial.print(IR.IR_Read_R());


}
