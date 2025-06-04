/**!
 * @file Camera.h 
 * @author Aleksandar Dikov (528052)
 * @version 1.0
 * @date 2025-06-04
 * @details This class holds all the functions for working with the Camera of the Elegoo smart car 
 * @copyright GNU Public Licence
 */
#pragma once
 #include <Arduino.h> 
 /**
  * @class Motors
  * Class containing the basic functionality for controlling the Motors of the Elegoo smartcar
  */
class Motors
{
public:

/**
 * @brief 
 * Motor Initialization function. 
 * 
 */
  void Motor_init(void);

  /**
   * @brief 
   * Sets the direction and speed of all the motors 
   * 
   * @param direction_A 
   * Group A motor Direction
   * @param speed_A 
   * Group A speed ( from 0 to 255)
   * @param direction_B 
   * Group B motor Direction
   * @param speed_B 
   * Group B speed ( from 0 to 255)
   * @param controlED 
   * Control group LED
   */
  void Set_motor(boolean direction_A, uint8_t speed_A, //A组电机参数
                                     boolean direction_B, uint8_t speed_B, //B组电机参数
                                     boolean controlED                     //AB使能允许 true
  );                                                                       //电机控制
private:

#define PIN_Motor_PWMA 5
#define PIN_Motor_PWMB 6
#define PIN_Motor_BIN_1 8
#define PIN_Motor_AIN_1 7
#define PIN_Motor_STBY 3

public:
#define speed_Max 255
#define direction_just true
#define direction_back false
#define direction_void 3

#define Duration_enable true
#define Duration_disable false
#define control_enable true
#define control_disable false

};