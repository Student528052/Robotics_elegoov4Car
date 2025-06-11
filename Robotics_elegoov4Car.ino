#include "include/Motors.h"
#include "include/IR_sensors.h"
#include "include/Ultrasonic.h"
#include "src/Motor_control.cpp"
#include <Servo.h>
#include <Arduino.h>
#define speed 60

Motors motors;
IR_sensors IR; 
Motor_Control M_Control; 
Ultrasonic US; 
Servo servo; 
//TODO: use this instead of define
enum colors{
   BLACK_min = 690, 
   BLACK_max = 1200, 
   RED_min = 60, 
   RED_max = 200, 
   GREEN_min = 200, 
   GREEN_max = 400, 
   BLUE_min = 400,
   BLUE_max = 690,
   WHITE = 40, 
};
auto line_max = colors::BLUE_max; //change this to make sure it  follow a specific line. 
auto line_min = colors::BLUE_min; //change this to make sure it  follow a specific line. 
   

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
   print_data(); 
  }

  }

}

//function for checking weather the car is on the right path. 
//Since WHITE is the lowest value, we can assume that any value below the desired line
//is going off course
int line_direction(float left, float center, float right, colors line_min, colors line_max){
  bool Lir = left >= line_min && left <= line_max; //left in range
  bool Cir = center >= line_min && center <= line_max; // Center in range
  bool Rir = right >= line_min && right <= line_max; //Right in range
  if( Lir && Cir && Rir ) return 0; // 0 0 0 
  else if ( Lir && Cir && !Rir) return 1; // 0 0 1 
  else if ( !Lir && Cir && Rir) return 4; // 1 0 0 
  else if ( Lir && !Cir && !Rir) return 3; // 0 1 1 
  else if ( !Lir && !Cir && Rir) return 6; // 1 1 0
  else if ( Lir && !Cir && Rir) return 2; // 0 1 0  assume forward
  else if (!Lir && Cir && !Rir) return 5; // 1 0 1  assume forward
  else return 8; //lost in path

} 
void Drive_direction(int i){

   auto direction = M_Control.Motion_Control;  
   auto turning_speed = speed -5; 
   switch (i)
   {
      case 0: //0 0 0 
         direction = ControlCommands::Forward; 
         Drive(direction, speed); 
         break;
      case 1: //0 0 1 
         direction = ControlCommands::Left; 
         Drive(direction, turning_speed); 
         break; 
      case 4: //1 0  
         direction = ControlCommands::Right; 
         Drive(direction, turning_speed); 
         break; 
      case 3: //0 1 1 
         direction = ControlCommands::Left; 
         Drive(direction, turning_speed); 
         break;
      case 6: //1 1 0 
         direction = ControlCommands::Right; 
         Drive(direction, turning_speed); 
         break;
      case 2: //0 1 0 assume forward. 
         direction = ControlCommands::Forward; 
         Drive(direction, speed); 
         break;
      case 5: //1 0 1 assume forward. 
         direction = ControlCommands::Forward; 
         Drive(direction, speed); 
         break;
      
      default:
         break;
      }
   }
void check_for_path(){

   auto  left  =  IR.IR_Read_L();
   auto  center = IR.IR_Read_M();
   auto  right = IR.IR_Read_R();
   if((left >=50 && left <= 500) && ( center >=50 && center <= 500) &&  right >= 300) right  = right - 200; 
   auto direction = M_Control.Motion_Control; 
   int i = line_direction(left, center, right, colors::BLACK_min, colors::BLACK_max); 
   int j  = line_direction(left, center, right, line_min, line_max); 
   Serial.print("Direction: "); 
   Serial.print(i); 
   Serial.print(" "); 
   Serial.print(j); 
   if( j != 8){
      Drive_direction(j); 
   }else if( i != 8){//black line
      Drive_direction(i); 
   

  }else if ( i == 8 &&  j == 8){ //lost in path, hardcoded so that it checks left and right for path
   //I could add this to a function
      //stop and go forward to center yourself in on the angle(hardcoded)
     direction = ControlCommands::stop_it; 
     Serial.println("Searching..."); 
     Drive(direction, 50); 
     delay(500); 
     Drive(ControlCommands::Forward, 50); 
     delay(350); 
     Drive(ControlCommands::stop_it, 50); 
     delay(100); 


     //turn left and right incrementally until you find black
      int rad_time = 0; 
   while ((left <=BLACK_min && center <=BLACK_min && right <=BLACK_min) &&
    (left <=line_min && center <=line_min && right <=line_min) //TODO: CHANGE THIS
   ){

      delay(100); 
    left  =  IR.IR_Read_L();
    center = IR.IR_Read_M();
    right = IR.IR_Read_R();
   auto Mean = (left + right + center )/ 3; 
   if(Mean <= colors::WHITE) break; 
   if((left >=50 && left <= 500) && ( center >=50 && center <= 500) &&  right >= 300) right  = right - 200; 
   if(line_direction(left, center, right, line_min, line_max) !=8 || line_direction(left, center, right, colors::BLACK_min, colors::BLACK_max) !=8) break; 

    Mean = (left + right + center )/ 3; 
   

      if(rad_time % 2 == 0){
         direction = ControlCommands::Right; 
         Drive(direction, speed + 15); 
      }

      else {direction = ControlCommands::Left;
         Drive(direction, speed+15); 
       }
       
     rad_time = rad_time + 1; 
     delay(rad_time * 50); 
         Drive(ControlCommands::stop_it, 0); 
         delay(300); 
      print_data(); 
   }

  }
  delay(50); //needed for IR? 
  
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
   Serial.print(distance); 
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

   auto  left  =  IR.IR_Read_L();
   auto  center = IR.IR_Read_M();
   auto  right = IR.IR_Read_R();
   if((left >=50 && left <= 500) && ( center >=50 && center <= 500) &&  right >= 300) right  = right - 200; 
  Serial.print("\n Left: ");
  Serial.print(left);
  Serial.print("  Medium: ");
  Serial.print(center);
  Serial.print(" Right: ");
  Serial.print(right);
  auto Mean = (left + right + center )/ 3; 
  Serial.print("   Mean: "); 
  Serial.print(Mean); 
  Serial.print("  ");
}



void Standby(){
   Drive(ControlCommands::stop_it, 0); 
   Serial.println("STOPPED!!!"); 
   delay(300); 
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
  print_data(); 
 Serial_monitor_stop(); //testing

}
