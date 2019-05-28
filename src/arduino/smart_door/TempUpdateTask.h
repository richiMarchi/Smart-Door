#ifndef __TEMPUPDATETASK__
#define __TEMPUPDATETASK__

#include "Task.h"
#include "SharedContext.h"
#include "DHT.h"
#include "config.h"

class TempUpdateTask: public Task {

public:

  TempUpdateTask(SharedContext* pContext);
  void init(int period);  
  void tick();

private:
  SharedContext* pContext;
  enum {IDLE, ON} state;
  float tempValue;
  DHT* dht;
};



#endif
