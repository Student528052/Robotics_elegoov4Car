/**!
 * @file IR_sensors.h
 * @author Aleksandar Dikov (528052)
 * @version 1.0
 * @date 2025-06-04
 * @details This class holds all the functions for working with the 3 Infrared Sensors  of the Elegoo smart car 
 * @copyright GNU Public Licence
 */
#pragma once
#include <Arduino.h>

/**
 * @brief 
 * @class IR_sensors 
 */
 
class IR_sensors{
public:
/**
 * @brief 
 * IR_sensor initialization function
 * 
 * @return true if
 * successfully initialized
 * @return false if
 * unsucessfully initialized
 */
  bool IR_init(void);

  /**
   * @brief 
   * Read Left Infrared sensor
   * 
   * @return float value of infrared, between 0 and 700
   */
  float IR_Read_L(void);
  /**
   * @brief
   * Read Middle Ifrared Sensor
   * 
   * @return float value of infrared, between 0 and 700
   */
  float IR_Read_M(void);
  /**
   * @brief 
   * Read Right Infrared sensor
   * 
   * @return float value of infrared, between 0 and 700
   */
  float IR_Read_R(void);
  /**
   * @def
   * Left Infrared sensor pin
   * 
   */
  #define PIN_ITR20001xxxL A2
  /**
   * @def PIN_ITR20001xxxM 
   * Middle Infrared sensor pin
   * 
   */
  #define PIN_ITR20001xxxM A1
  /**
   * @def PIN_ITR20001xxxR 
   * Right Infrared sensor pin
   * 
   */
  #define PIN_ITR20001xxxR A0
}; 
