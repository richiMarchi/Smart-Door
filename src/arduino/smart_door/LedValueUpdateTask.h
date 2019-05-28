#ifndef __LEDVALUEUPDATETASK__
#define __LEDVALUEUPDATETASK__

#include "Task.h"
#include "SharedContext.h"
#include "LedExt.h"


class LedValueUpdateTask: public Task {

public:

  LedValueUpdateTask(SharedContext* pContext);
  void init(int period);  
  void tick();

private:
  SharedContext* pContext;
  enum {IDLE, ON} state;
  LightExt* ledValue;
};



#endif
