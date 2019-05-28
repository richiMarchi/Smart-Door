#ifndef __LED_EXT__
#define __LED_EXT__

#include "Led.h"
#include "LightExt.h"

class LedExt:  public LightExt, public Led { 
public:
  LedExt(int pin);
  LedExt(int pin, int intensity);
  void switchOn();
  void switchOff();
  void setIntensity(int v);
  bool isOn();
private:
  int currentIntensity;
  bool isOnv;
};

#endif

