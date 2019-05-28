#include "CommandReaderTask.h"
#include "config.h"
#include "Arduino.h"
#include "MsgService.h"
#include "BluetoothService.h"
#include "Logger.h" /* per debug */

CommandReaderTask::CommandReaderTask(SharedContext *pContext) {
  this->pContext = pContext;
}

void CommandReaderTask::init(int period) {
  Task::init(period);
}

void CommandReaderTask::tick() {
  /*Check seriale*/
  if (MsgService.isMsgAvailable()) {
    Msg* msg = MsgService.receiveMsg();
    /*errore nell'autenticazione*/
    if(msg->getContent() == "aFAIL"){
      pContext->setAccessFail(true);
    }
    /*successo dell'autenticazione*/
    else if(msg->getContent() == "aOK"){
      pContext->setAccessSuccess(true);
//      Logger.log("aOK rcvd");
    }
    delete msg;
  }
  /*Check bluetooth*/
  if (Bluetooth.isMsgAvailable()){
    Msg* bluetoothMsg = Bluetooth.receiveMsg();
    Logger.log("rcvd msg = " + bluetoothMsg->getContent());
    /* LOGIN SEQUENCE:
     * 1) Send username in this format user:<username>
     * 2) Send password in this format pass:<password>
     * 3) Send an r to confirm login
     */
    if(bluetoothMsg->getContent().substring(0,5) == "user:"){
      pContext->setUser(bluetoothMsg->getContent().substring(5));
//      Logger.log("User set: " + pContext->getUser());
    }
    else if(bluetoothMsg->getContent().substring(0,5) == "pass:"){
      pContext->setPassword(bluetoothMsg->getContent().substring(5));
//      Logger.log("pwd set: " + pContext->getPassword());
    }
    else if(bluetoothMsg->getContent() == "r"){
      pContext->setReceivedLogin(true);
//      Logger.log("r rcvd");
    }
    // receiving new led value percentage
    else if(bluetoothMsg->getContent().substring(0,5) == "lval:") {
      pContext->setLedValue(bluetoothMsg->getContent().substring(5).toInt());
      pContext->setLedValueChanged(true);
//      Logger.log("Led val: " + String(pContext->getLedValue()));
//      Logger.log("Led val changed: " + String(pContext->isLedValueChanged()));
    }
    else if(bluetoothMsg->getContent() == "end") {
      pContext->setSessionStarted(false);
    }
    else if (bluetoothMsg->getContent() == "qm") {
      pContext->setAppStarted(false);
    }
    else if (bluetoothMsg->getContent() == "hm") {
      pContext->setAppStarted(true);
    }
    else if (bluetoothMsg->getContent() == "sc") {
      pContext->setSessionCreated(true);
    }
    delete bluetoothMsg;
  }
}


