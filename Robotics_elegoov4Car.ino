#include "include/Motors.h"
#include "include/IR_sensors.h"
#include "include/Ultrasonic.h"
#include "src/Motor_control.cpp"
#include <Servo.h>
#include <Arduino.h>
#define speed 60
#define BLACK 530
#define WHITE 200

Motors motors;
IR_sensors IR; 
Motor_Control M_Control; 
Ultrasonic US; 
Servo servo; 

   

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

   print_data(); 
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
         Drive(direction, speed); 
      }

      else {direction = ControlCommands::Left;
         Drive(direction, speed); 
       }
     rad_time = rad_time + 1; 
     delay(rad_time * 100); 
   print_data(); 
   }

  }
}



/*NOTE: the car avoids the object by continuisly changing the servo degrees while avoiding it. 
If at one point the ultrasonic sensor does not detect the object, it assumes it's next to it or past it
It takes the ammout of degrees the servo has moved and uses that as an indicator as to how much it should 
go forward back to the track. 

THIS FUNCTION ASSUMES THAT THE SERVO NEVER TRIES TO EXCEED ABOVE 180 DEGREES

*/
void check_for_object(){
   unsigned int distance = US.Ultrasonic_Read(); 
   Serial.print("DISTANCE : "); 
   Serial.println(distance); 
   int step = 0; 
   delay(10);  //needed for the ultrasonic sensor to reset.
   if (distance <= 14 && distance >=10 && distance != 0 ){
      int pos = 90; //servo position
      Drive(ControlCommands::stop_it, 0); 
      delay(500); 
      bool object_found = (distance <=20) ? true: false; 
      int obstacle_speed = speed + 10; 

      while (object_found){
         unsigned int distance = US.Ultrasonic_Read(); //refresh value
         

         Drive(ControlCommands::Right, obstacle_speed); 
         delay(400); 
         Drive(ControlCommands::Forward, obstacle_speed); 
         delay(450); 
         step++; 
         pos +=15; 

         //moving the angle of the ultrasonic sensor
         if (pos >180) pos = 180; 
         servo.write(pos); 
         

         object_found = (distance <=20) ? true: false; 
      }
      //recenter
      Drive(ControlCommands::Left, speed); 
         delay(325*step); 
      //once it has stopped finding the object, return to the original line 
      while (pos >90){
         Drive(ControlCommands::Forward, obstacle_speed); 
         delay(435); 
         Drive(ControlCommands::Left, obstacle_speed); 
         delay(350); 
         pos -=10; 
      }
         Drive(ControlCommands::Forward, obstacle_speed); 
         delay(435); 
      //recenter itself on the course
      Drive(ControlCommands::Right, speed); 
         delay(325*step); 
         servo.write(90); 

   }else{
      Serial.print("No object!"); 

   }

}

void print_data(){
 Serial.print(" Ultrasonic:"); 
 Serial.print(US.Ultrasonic_Read()); 
 delay(20); 

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
  Drive(ControlCommands::stop_it, 0); 
  servo.attach(10); 
  servo.write(90); //position the ultrasonic at the "center"
  delay(1000); //give the whole system time to "calibrate", mainly for the motors and servo

}

void loop() {

  check_for_path(); 
  check_for_object(); 
// Serial_monitor_stop(); //testing

}
