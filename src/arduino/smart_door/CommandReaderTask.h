#ifndef __COMMANDREADERTASK__
#define __COMMANDREADERTASK__

#include "Task.h"
#include "SharedContext.h"


class CommandReaderTask: public Task {

public:

  CommandReaderTask(SharedContext* pContext);
  void init(int period);  
  void tick();

private:
  SharedContext* pContext;
};



#endif

