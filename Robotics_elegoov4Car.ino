#include "include/Motors.h"
#include "include/IR_sensors.h"
#include "include/Ultrasonic.h"
#include "src/Motor_control.cpp"
#include <Arduino.h>
#define speed 50
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

   auto  left  =  IR.IR_Read_L();
   auto  right = IR.IR_Read_R();
   auto  center = IR.IR_Read_M();
   auto direction = M_Control.Motion_Control; 

  if (  left >=BLACK && center >=BLACK &  right >= BLACK){ //0 0 0 
     direction = ControlCommands::Forward; 
     Drive(direction, speed); 

  }else if ( left >=BLACK && center >=BLACK && right <=BLACK){ // 0 0 1
     direction = ControlCommands::Left; 
     Drive(direction, speed);

  }else if (  left <=BLACK && center >=BLACK && right >=BLACK){ // 1 0 0 
     direction = ControlCommands::Right; 
     Drive(direction, speed);

  }else if ( left >=BLACK && center <=BLACK && right <=BLACK){ // 0 1 1
     direction = ControlCommands::Left; 
     Drive(direction, speed);

  }else if (  left <=BLACK && center <=BLACK && right >=BLACK){ // 1 1 0 
     direction = ControlCommands::Right; 
     Drive(direction, speed);

  }else if (  left >=BLACK && center <=BLACK && right >=BLACK){ // 0 1 0  : assume forward
     direction = ControlCommands::Forward; 
     Drive(direction, speed);
  
  }else{ //lost in path, hardcoded so that it checks left and right for path
   //I could add this to a function
      //stop and go forward to center yourself in on the angle(hardcoded)
     direction = ControlCommands::stop_it; 
     Serial.println("Searching..."); 
     Drive(direction, 50); 
     delay(500); 
     Drive(ControlCommands::Forward, 50); 
     delay(200); 
     Drive(ControlCommands::stop_it, 50); 
     delay(100); 


     //turn left and right incrementally until you find black
      int rad_time = 0; 
   while (IR.IR_Read_L() <=BLACK && IR.IR_Read_M() <=BLACK && IR.IR_Read_R() <=BLACK){

      if(rad_time % 2 == 0){ 
         direction = ControlCommands::Right; 
         Drive(direction, 50); 
      }

      else {direction = ControlCommands::Left;
         Drive(direction, 50); 
       }


     rad_time = rad_time + 1; 
     delay(rad_time * 100); 
     print_data(); 
   }

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

  motors.Motor_init(); 
  IR.IR_init(); 
  US.Ultrasonic_Init(); 
  Serial.begin(9600); 
  auto direction = M_Control.Motion_Control; 
   direction = ControlCommands::stop_it; 
  Drive(direction, speed); 

}

void loop() {
  print_data(); 
  check_for_path(); 
  //check_for_object(); 
// Serial_monitor_stop(); //testing

}
