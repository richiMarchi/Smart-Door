#include "ServoTimer2.h"
#include "CommandReaderTask.h"
#include "Door.h"
#include "Scheduler.h"
#include "MsgService.h"
#include "BluetoothService.h"
#include "config.h"
#include "TempUpdateTask.h"
#include "LedValueUpdateTask.h"

Scheduler sched;

void setup() {
  sched.init(50);
  
  SharedContext* pContext = new SharedContext();
  MsgService.init();
  Bluetooth.init();
  
  Door* door = new Door(pContext);
  door->init(100);
  sched.addTask(door);

  CommandReaderTask* reader = new CommandReaderTask(pContext);
  reader->init(50);
  sched.addTask(reader);

  TempUpdateTask* temp = new TempUpdateTask(pContext);
  temp->init(2000);
  sched.addTask(temp);

  LedValueUpdateTask* led = new LedValueUpdateTask(pContext);
  led->init(150);
  sched.addTask(led);
}

void loop() {
  sched.schedule();

  if (Bluetooth.isDataAvailable()) {
    Bluetooth.BtSerialEvent();
  }
}

