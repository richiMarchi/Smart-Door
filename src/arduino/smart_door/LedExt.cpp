#include "LedExt.h"
#include "Arduino.h"

LedExt::LedExt(int pin) : Led(pin) {
  currentIntensity = 0;
  isOnv = false;
}

LedExt::LedExt(int pin, int intensity) : Led(pin) {
  isOnv = false;
  currentIntensity = intensity;
}

bool LedExt::isOn(){
  return isOnv;
}

void LedExt::switchOn(){
  analogWrite(pin,currentIntensity);
  isOnv = true;
}

void LedExt::setIntensity(int value){
 currentIntensity = value;
 isOnv = (value > 0) ? true : false;
 analogWrite(pin,currentIntensity);
}

void LedExt::switchOff(){
  analogWrite(pin,0);
  isOnv = false;
}

