#ifndef __DOOR__
#define __DOOR__

#include "Arduino.h"
#include "ServoTimer2.h"
#include "SharedContext.h"
#include "Task.h"
#include "Light.h"
#include "ProximitySensor.h"
#include "Button.h"
#include "Sonar.h"
#include "Pir.h"


class Door: public Task {
  
  public:
    Door(SharedContext *pContext);
    void init(int period);
    void tick();

  private:
    SharedContext* pContext;
    Light* ledOn;
    Button* buttonExit;
    Pir* pir;
    ProximitySensor* prox;
    ServoTimer2* servo;
    //TempSensor* termo;
    unsigned long tStart;
    unsigned long elapsed;
    unsigned long tStart2;
    unsigned long elapsed2;
    String stato;
    bool startCount;
    bool startCount2;
    enum {IDLE, WAIT_HELLO, ON, ASK_LOGIN, WAIT_LOGIN, LOGIN_SUCCESSFUL, SESSION_WAIT, SESSION} state;
};
#endif

