#include "TempUpdateTask.h"
#include "BluetoothService.h"

#define MSG_TEMP  "2"

TempUpdateTask::TempUpdateTask(SharedContext *pContext) {
  this->pContext = pContext;
  this->dht = new DHT(DHTPIN, DHTTYPE);
}

void TempUpdateTask::init(int period) {
  Task::init(period);
  state = IDLE;
  dht->begin();
}

void TempUpdateTask::tick(){
  switch(state){
    case IDLE:
      if(true){
        state = ON;
      }
      break;
    case ON:
      /*tempValue = lettura del sensore*/
      tempValue = dht->readTemperature();
      if (!isnan(tempValue)) {
        String msgTemp = String(MSG_TEMP) + "|" + String(tempValue);
        if (pContext->isSessionStarted()) {
           Bluetooth.sendMsg(msgTemp);
        }
        MsgService.sendMsg(msgTemp);
      }
      break;
  }
}

