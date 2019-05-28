#ifndef __PIR__
#define __PIR__

#include "PresenceSensor.h"

class Pir: public PresenceSensor {
 
public: 
  Pir(int pin);
  bool isDetected();

private:
  int pin;

};

#endif

