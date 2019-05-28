#include "TempSensor.h"
#include <Wire.h>

TempSensor::TempSensor() {
  Wire.begin(); /* lib init */
}

int TempSensor::readTemperature() {
  return 21;
  /*
  Wire.beginTransmission(temp_address);
  Wire.write(0);
  Wire.endTransmission();
  Wire.requestFrom(temp_address, 1);
  while(Wire.available() == 0);
  int c = Wire.read();
  */
}

