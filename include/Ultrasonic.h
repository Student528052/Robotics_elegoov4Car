/**!
 * @file Ultrasonic.h
 * @author Aleksandar Dikov (528052)
 * @version 1.0
 * @date 2025-06-04
 * @details This class holds all the functions for the Ultrasonic sensor of the Elegoo Smart car. 
 * @copyright GNU Public Licence
 */
#pragma once
 #include <Arduino.h>                                                                               
  
  //#include <NewPing.h>

  /**
   * @class Ultrasonic class
   *
   */
  class Ultrasonic
  {
  public:
  /**
   * @brief 
   * Ultrasonic initialization function
   * 
   */
    void Ultrasonic_Init(void);
    /**
     * @brief 
     * Read Input from Ultrasonic sensor 
     * 
     * 
     * @return unsigned int
     *  - distance in cm
     */
    unsigned int Ultrasonic_Read();
  
  private:
  /**
   * @def TRIG_PIN 
   * Ultrasonic Trigger pin
   * Arduino pin tied to trigger pin on the ultrasonic sensor.
   * 
   */
  #define TRIG_PIN 13      
  
   /** 
   * @def ECHO_PIN 
   * Ultrasonic Echo pin
   * Arduino pin tied to echo pin on the ultrasonic sensor.
   */
  #define ECHO_PIN 12      
  /**
   * @def MAX_DISTANCE 
   * 
   *  Maximum distance we want to ping for (in centimeters). Maximum sensor d    istance is rated at 400-500cm.
   * 
   */
  #define MAX_DISTANCE 200 
  };
  