#ifndef __TEMP_SENSOR__
#define __TEMP_SENSOR__

class TempSensor {
  
public:
  TempSensor();
  int readTemperature();
  
private:
  const int temp_address = 72; //1001000 in decimale
  
};

#endif


