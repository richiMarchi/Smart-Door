#ifndef __BLUETOOTHSERVICE__
#define __BLUETOOTHSERVICE__

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "config.h"
#include "MsgService.h" //per la classe pattern
#include "Msg.h"

class BluetoothService {
    
public:
 /*aggiunta*/
  Msg* currentMsg;
  bool msgAvailable;
 /**/
  BluetoothService(int rxPin, int txPin);  
  void init();  
  bool isMsgAvailable();
  bool isDataAvailable(); // aggiunta
  void BtSerialEvent(); //aggiunta
  Msg* receiveMsg();
  /*aggiunta*/
  bool isMsgAvailable(Pattern& pattern);
  Msg* receiveMsg(Pattern& pattern);
  /**/
  bool sendMsg(const String& msg);

private:
  String content;
  SoftwareSerial* channel;
  
};

extern BluetoothService Bluetooth;

#endif

