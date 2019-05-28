#include "Arduino.h"
#include "BluetoothService.h"

BluetoothService Bluetooth(RB, TB);

BluetoothService::BluetoothService(int rxPin, int txPin){
  channel = new SoftwareSerial(rxPin, txPin);
}

void BluetoothService::init(){
  content.reserve(256);
  channel->begin(9600);
  content = "";
  currentMsg = NULL;
  msgAvailable = false;
}

bool BluetoothService::sendMsg(const String& msg){
  channel->println(msg);  
}

bool BluetoothService::isMsgAvailable() {
  return msgAvailable;
}

bool BluetoothService::isDataAvailable(){
  return channel->available();
}

Msg* BluetoothService::receiveMsg(){
  if (msgAvailable) {
    Msg* msg = currentMsg;
    msgAvailable = false;
    currentMsg = NULL;
    content = "";
    return msg;
  } else {
    return NULL;
  }
}

void BluetoothService::BtSerialEvent() {
  while (channel->available()) {
    char ch = (char) channel->read();
    if (ch == '\n') {
      currentMsg = new Msg(content);
      msgAvailable = true;
    } else {
      content += ch;
    }
  }
}

/*aggiunta*/
bool BluetoothService::isMsgAvailable(Pattern& pattern){
  return (msgAvailable && pattern.match(*currentMsg));
}
/*aggiunta*/
Msg* BluetoothService::receiveMsg(Pattern& pattern){
  if (msgAvailable && pattern.match(*currentMsg)){
    Msg* msg = currentMsg;
    msgAvailable = false;
    currentMsg = NULL;
    content = "";
    return msg;  
  } else {
    return NULL; 
  }
}




