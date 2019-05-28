#include "SharedContext.h"
#include "config.h"
#include "Arduino.h"


SharedContext::SharedContext() {
    
}
  bool SharedContext::isSessionStarted() {
    return sessionStarted;
  }
  bool SharedContext::isSessionCreated() {
    return sessionCreated;
  }
  bool SharedContext::isAccessFail() {
    return accessFail;
  }
  bool SharedContext::isAccessSuccess() {
    return accessSuccess;
  }
  bool SharedContext::isAskLogin() {
    return askLogin;
  }
  bool SharedContext::isReceivedLogin() {
    return receivedLogin;
  }
  bool SharedContext::isLedValueChanged() {
    return ledValueChanged;
  }
  bool SharedContext::isFirstConn() {
    return firstConn;
  }
  bool SharedContext::isAppStarted() {
    return appStarted;
  }
  String SharedContext::getUser() {
    return user;
  }
  String SharedContext::getPassword() {
    return password;
  }
  int SharedContext::getLedValue() {
    return ledValue;
  }
  double SharedContext::getTempValue() {
    return tempValue;
  }
  void SharedContext::setUser(String value) {
    user = value;
  }
  void SharedContext::setLedValue(int value) {
    ledValue = value;
  }
  void SharedContext::setTempValue(float value) {
    tempValue = value;
  }
  void SharedContext::setPassword(String value) {
    password = value;
  }
  void SharedContext::setSessionStarted(bool value) {
    sessionStarted = value;
  }  
  void SharedContext::setAccessFail(bool value) {
    accessFail = value;
  }
  void SharedContext::setAccessSuccess(bool value) {
    accessSuccess = value;
  }
  void SharedContext::setAskLogin(bool value) {
    askLogin = value;
  }
  void SharedContext::setReceivedLogin(bool value) {
    receivedLogin = value;
  }
  void SharedContext::setLedValueChanged(bool value) {
    ledValueChanged = value;
  }
  void SharedContext::setFirstConn(bool value) {
    firstConn = value;
  }
  void SharedContext::setAppStarted(bool value) {
    appStarted = value;
  }
  void SharedContext::setSessionCreated(bool value) {
    sessionCreated = value;
  }





