#include "LedValueUpdateTask.h"
#include "BluetoothService.h"
#include "Logger.h"

#define PERC_TO_BYTE(x) (map(x,0,100,0,255))

#define MSG_LED "3"

LedValueUpdateTask::LedValueUpdateTask(SharedContext *pContext) {
  this->pContext = pContext;
  this->ledValue = new LedExt(L2_PIN);
}

void LedValueUpdateTask::init(int period) {
//  Logger.log("Sono in LedValue init");
  Task::init(period);
  state = IDLE;
  pContext->setLedValue(0);
  pContext->setLedValueChanged(true); // this way the first time someone enters the web service the current value is shown.
//  Logger.log("Led val changed: " + String(pContext->isLedValueChanged()));
}

void LedValueUpdateTask::tick(){
  switch(state){
    case IDLE:
      if(true){
        state = ON;
      }
      break;
    case ON:
//      Logger.log("Sono in ON");
      if(pContext->isLedValueChanged()){
//        Serial.println("Sono nell'if");
//        Logger.log("Led val changed");
//        Logger.log("Setting int perc to: " + String(pContext->getLedValue()));
//        Logger.log("Led val: " + String(pContext->getLedValue()));
//        Logger.log("byte: " + String(PERC_TO_BYTE(pContext->getLedValue())));
        ledValue->setIntensity(PERC_TO_BYTE(pContext->getLedValue()));
        String msgLedUpdate = String(MSG_LED) + "|" + pContext->getLedValue();
        MsgService.sendMsg(msgLedUpdate);
        pContext->setLedValueChanged(false);
      }
      if (pContext->isSessionStarted() && pContext->isFirstConn()) { // when mobile connection occurs tell the device what's the value of the led.
          Logger.log("Sending led val to mobile");
          String msgLedValue = String(MSG_LED) + "|" + pContext->getLedValue();
          Bluetooth.sendMsg(msgLedValue);
          pContext->setFirstConn(false);
      }
      break;
  }
}

