#include "include/Motors.h"
#include "include/IR_sensors.h"
#include "include/Ultrasonic.h"
#include "src/Motor_control.cpp"
#include <Arduino.h>
#define speed 80
#define BLACK 650
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
   direction = ControlCommands::stop_it; 
   Serial.println("STOPPED");
   Serial.println("STOPPED");
   Serial.println("STOPPED");
   while(1){ 
   Drive(direction, 0); 
  }

  }
}
void check_for_path(){
  auto left = IR.IR_Read_L(); 
  auto right = IR.IR_Read_R(); 
  auto center = IR.IR_Read_M(); 
  auto direction = M_Control.Motion_Control; 

  if(center <= WHITE && left <= WHITE && right <= WHITE ){
     direction = ControlCommands::LeftForward; //Lost the path, slightly turn left until you find it. 
     Drive(direction, 50); 
  }
  else if ((center >=BLACK &&  left <=WHITE && right >= BLACK) || ( center <= WHITE && left <=WHITE && right >=BLACK)){
     direction = ControlCommands::Right; 
     Drive(direction, speed); 
  }
  else if ((center >=BLACK &&  left <=BLACK && right >= WHITE) || ( center <= WHITE && left >=BLACK && right <=WHITE)){
     direction = ControlCommands::Left; 
     Drive(direction, speed); 
  
  }else if (center >=BLACK &&  left >=BLACK && right >= BLACK){

     direction = ControlCommands::Forward; 
     Drive(direction, speed); 
  }

}


void check_for_object(){
   auto distance = US.Ultrasonic_Read(); 
   if (distance <= 30 && distance >=10){
      while(1){
         Drive(ControlCommands::stop_it, 0); 
         Serial.println("Ultrasonic object detected. STOPPED!!!"); 
      }
   }
}

void print_data(){
 Serial.print(" Ultrasonic:"); 
 Serial.print(US.Ultrasonic_Read()); 

  Serial.print("\n Left");
  Serial.print(IR.IR_Read_L());
  Serial.print("  Medium ");
  Serial.print(IR.IR_Read_M());
  Serial.print("  Right ");
  Serial.print(IR.IR_Read_R());
}
/* ---------------SETUP AND LOOP---------------- */
void setup() {
  // put your setup code here, to run once:
  motors.Motor_init(); 
  IR.IR_init(); 
  US.Ultrasonic_Init(); 
  Serial.begin(115200); 
  auto direction = M_Control.Motion_Control; 
   direction = ControlCommands::stop_it; 
  Drive(direction, speed); 

}

void loop() {
 // Drive(M_Control.Motion_Control, 150); 
  print_data(); 
  check_for_path(); 
  //check_for_object(); 
 Serial_monitor_stop(); //testing

}
