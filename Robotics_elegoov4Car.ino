#include "include/Motors.h"
#include "include/IR_sensors.h"
#include "include/Ultrasonic.h"
#include "src/Motor_control.cpp"
#include <Arduino.h>
#define speed 200
#define BLACK 500
#define WHITE 200

Motors motors;
IR_sensors IR; 
Motor_Control M_Control; 
Ultrasonic US; 

//Serial Monitor stop command to stop the motors for testing
void Serial_monitor_stop(){

  char input = Serial.read(); 
  if (input == 'q'){
   auto direction = M_Control.Motion_Control; 
   direction = ControlCommands::Forward; 
   while(1); 
   Serial.println("STOPPED");
   Drive(direction, 0); 

  }
}
void check_for_path(){
  auto left = IR.IR_Read_L(); 
  auto right = IR.IR_Read_R(); 
  auto center = IR.IR_Read_M(); 
  auto direction = M_Control.Motion_Control; 

  if(center <= WHITE && left <= WHITE && right <= WHITE ) direction = ControlCommands::Right; //Lost the path, turn right at random. 
  else if (center >=BLACK &&  left >=BLACK && right >= BLACK) direction = ControlCommands::Forward; 
  else if (center >=BLACK &&  left <=WHITE && right >= BLACK) direction = ControlCommands::RightForward; 
  else if (center >=BLACK &&  left <=BLACK && right >= WHITE) direction = ControlCommands::LeftForward; 

  Drive(direction, speed); 
}
void setup() {
  // put your setup code here, to run once:
  motors.Motor_init(); 
  IR.IR_init(); 
  US.Ultrasonic_Init(); 
  Serial.begin(9600); 
  auto direction = M_Control.Motion_Control; 
   direction = ControlCommands::stop_it; 
  Drive(direction, speed); 

}

void loop() {
 // Drive(M_Control.Motion_Control, 150); 
 Serial.print(" Ultrasonic:"); 
 Serial.print(US.Ultrasonic_Read()); 

  Serial.print("\n Left");
  Serial.print(IR.IR_Read_L());
  Serial.print("  Medium ");
  Serial.print(IR.IR_Read_M());
  Serial.print("  Right ");
  Serial.print(IR.IR_Read_R());
 // check_for_path(); 
  //Serial_monitor_stop(); //testing

}
