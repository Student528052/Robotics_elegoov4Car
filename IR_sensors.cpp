#include "IR_sensors.h"

bool IR_sensors::IR_init(void)
{
  pinMode(PIN_ITR20001xxxL, INPUT);
  pinMode(PIN_ITR20001xxxM, INPUT);
  pinMode(PIN_ITR20001xxxR, INPUT);
  return false;
}
float IR_sensors::IR_Read_L(void)
{
  return analogRead(PIN_ITR20001xxxL);
}
float IR_sensors::IR_Read_M(void)
{
  return analogRead(PIN_ITR20001xxxM);
}
float IR_sensors::IR_Read_R(void)
{
  return analogRead(PIN_ITR20001xxxR);
}

