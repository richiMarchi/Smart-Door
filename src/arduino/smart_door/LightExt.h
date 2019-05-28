#ifndef __LIGHT_EXT__
#define __LIGHT_EXT__

#include "Light.h"

class LightExt : public Light {
public:
  virtual void setIntensity(int) = 0;
  virtual bool isOn() = 0;
};

#endif


