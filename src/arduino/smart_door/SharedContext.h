#ifndef __SHAREDCONTEXT__
#define __SHAREDCONTEXT__

#include "ProximitySensor.h"
#include "Arduino.h"

class SharedContext {
  
public:
  SharedContext();
  String getUser();
  String getPassword();
  int getLedValue();
  double getTempValue();
  bool isSessionStarted();
  bool isAccessFail();
  bool isAccessSuccess();
  bool isAskLogin();
  bool isReceivedLogin();
  bool isLedValueChanged();
  bool isFirstConn();
  bool isAccessAborted();
  bool isAppStarted();
  bool isSessionCreated();
  void setUser(String value);
  void setPassword(String value);
  void setLedValue(int value);
  void setTempValue(float value);
  void setSessionStarted(bool value);
  void setAccessFail(bool value);
  void setAccessSuccess(bool value);
  void setAskLogin(bool value);
  void setReceivedLogin(bool value);
  void setLedValueChanged(bool value);
  void setFirstConn(bool value);
  void setAppStarted(bool value);
  void setSessionCreated(bool value);
private:
  volatile bool sessionStarted = false;
  volatile bool accessFail = false;
  volatile bool accessSuccess = false;
  volatile bool askLogin = false;
  volatile bool receivedLogin = false;
  volatile bool ledValueChanged = true;
  volatile bool firstConn = false;
  volatile bool appStarted = false;
  volatile bool sessionCreated = false;
  double tempValue;  
  int ledValue;
  String password;
  String user;
 };

#endif

