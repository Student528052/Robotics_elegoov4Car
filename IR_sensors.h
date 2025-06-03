#pragma once
/*ITR20001 Detection*/
#include <Arduino.h>

class IR_sensors{
public:
  bool IR_init(void);
  float IR_Read_L(void);
  float IR_Read_M(void);
  float IR_Read_R(void);
  #define PIN_ITR20001xxxL A2
  #define PIN_ITR20001xxxM A1
  #define PIN_ITR20001xxxR A0
}; 