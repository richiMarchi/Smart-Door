#include "Door.h"
#include "LedExt.h"
#include "Led.h"
#include "Sonar.h"
#include "config.h"
#include "ButtonImpl.h"
#include "BluetoothService.h"
#include "MsgService.h"
#include "Logger.h"

#define MIN_ANGLE 0
#define MAX_ANGLE 180

#define ANGLE_TO_PULSE(x) (map(x, MIN_ANGLE, MAX_ANGLE, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH))

#define MSG_LOGIN               "1"
#define MSG_ACC_FAIL            "aFAIL"
#define MSG_ACC_OK              "aOK"
#define MSG_QUIT                "q"
#define MSG_ACC_FAIL_PIR        "4"
#define MSG_HELLO               "h"
#define MSG_RESET_ON_WH         "RSTon"
#define MSG_RESET_AL_WH         "RSTal"
#define MSG_RESET_LS_WH         "RSTls"
#define MSG_INSIDE              "5"
#define MSG_QUIT_GW             "6"
#define MSG_SESS_START          "sSTART"
#define MSG_GW_ERR              "gwERR"
#define MSG_SESS_TIMEOUT        "sTOUT"

Door::Door(SharedContext *pContext) {
  this->prox = new Sonar(SONAR_ECHO_PIN, SONAR_TRIG_PIN);
  this->pContext = pContext;
  this->ledOn = new Led(L1_PIN);
  this->buttonExit = new ButtonImpl(B_OFF);
  this->pir = new Pir(PIR);
  this->servo = new ServoTimer2();
  //this->termo = new TempSensor();
  this->tStart = 0;
  this->elapsed = 0;
  this->tStart2 = 0;
  this->elapsed2 = 0;
  this->startCount= false;
  this->startCount2 = false;
}

void Door::init(int period){
//  Logger.log("Sono in Door::init()");
  Task::init(period);
  state = IDLE;
  servo->attach(SERVO_PWM_PIN);
  servo->write(ANGLE_TO_PULSE(0));
  //servo->detach();
}

void Door::tick(){
  switch(state){
    case IDLE:
      if(true){
        ledOn->switchOn();
        state = WAIT_HELLO;
      }
      break;
      case WAIT_HELLO:
        if(pContext->isAppStarted()) {
          tStart=0;
          elapsed = 0;
          tStart2 = millis();
          elapsed2 = 0;
          startCount2 = false;
          state=ON;
//          stato = "WAIT_HELLO";
//          Logger.log(stato);
        }
      break;
    case ON:
        elapsed2 = millis() - tStart2;
//        Logger.log("E2: " + String(elapsed2));
        if (!pContext->isAppStarted()) {
          if (startCount) {
           startCount = false;
           elapsed = 0;
           elapsed2 = 0;
          }
          state = WAIT_HELLO;          
        } else if (elapsed2 >= RESET_TIMEOUT) {
           if (startCount) {
           startCount = false;
           elapsed = 0;
           elapsed2 = 0;
          }
          pContext->setAppStarted(false);
          Bluetooth.sendMsg(MSG_RESET_ON_WH);
          state = WAIT_HELLO;
        } else if (prox->getDistance() > DIST_MIN && startCount) {
          startCount = false;
          elapsed = 0;
        } else if(prox->getDistance()<=DIST_MIN && startCount){
          elapsed = millis() - tStart;  
          if(elapsed>=MIN_SEC){
             elapsed=0;
             startCount = false;
             state = ASK_LOGIN;
          }
        } else if (prox->getDistance()<=DIST_MIN){
          startCount = true;
          tStart = millis();
        }
          
//        if(buttonExit->isPressed()){
//           state = IDLE;
//           ledOn->switchOff();
//           pContext->setUser("");
//           pContext->setPassword("");
//          }
//        stato = "s = ON, el = " + String(elapsed) + ", dst = " + String(prox->getDistance());      
//        Logger.log(stato);
       break;
    case ASK_LOGIN:
        if (!pContext->isAppStarted()) {
              pContext->setAskLogin(false);
              pContext->setReceivedLogin(false);
              pContext->setUser("");
              pContext->setPassword("");
              startCount = false;
              elapsed = 0;
              state = WAIT_HELLO; // time's up, going back to ON state. 
        }
        else if(!pContext->isAskLogin()){
          pContext->setAskLogin(true);
          startCount = true;
          tStart = millis();
          Bluetooth.sendMsg(MSG_HELLO);
//          Logger.log("ho mandato lacca");
        }
        else if(pContext->isAskLogin() && pContext->isReceivedLogin()){
          /*Mando al GW le credenziali, poi aspetto di ricevere l'esito*/
//          Logger.log("To WAIT_LOGIN with user: " + pContext->getUser() + ", password: " + pContext->getPassword());
          String loginMsg = String(MSG_LOGIN) + "|" + pContext->getUser() + "|" + pContext->getPassword();
          MsgService.sendMsg(loginMsg);
//          MsgService.sendMsg("user: " + pContext->getUser());
//          MsgService.sendMsg("pass: " + pContext->getPassword());
          startCount = false;
          elapsed = 0;
          tStart = millis();
          state = WAIT_LOGIN;
          pContext->setReceivedLogin(false);
        }
       else if (startCount) { // means that the timeout for access failed has started
          elapsed = millis() - tStart;
          if (elapsed >= RESET_TIMEOUT) {
              pContext->setAskLogin(false);
              pContext->setReceivedLogin(false);
              pContext->setUser("");
              pContext->setPassword("");
              startCount = false;
              elapsed = 0;
              state = WAIT_HELLO; // time's up, going back to ON state. 
              pContext->setAppStarted(false);
              Bluetooth.sendMsg(MSG_RESET_AL_WH);
          }
        }
        
//        if(buttonExit->isPressed()){
//          state = IDLE;
//          ledOn->switchOff();
//          pContext->setUser("");
//          pContext->setPassword("");
//        }
//        stato = "s = ASK_LOGIN, l rcvd? " + String(pContext->isReceivedLogin()) + " user: " + pContext->getUser() + " pass: " + pContext->getPassword();
//        Logger.log(stato);
      break;
    case WAIT_LOGIN:
      elapsed = millis() - tStart;
//        elapsed = 0;
      if (!pContext->isAppStarted()) {
        startCount = false;
        elapsed = 0;
        pContext->setAccessSuccess(false);
        pContext->setAccessFail(false);
        pContext->setAskLogin(false);
        state = WAIT_HELLO;
      } else if (elapsed >= GW_ERR_TIMEOUT) {
        startCount = true;
        elapsed = 0;
        tStart = millis();
        pContext->setUser("");
        pContext->setPassword("");
        pContext->setAccessSuccess(false);
        pContext->setAccessFail(false);
        Bluetooth.sendMsg(MSG_GW_ERR);
        state = ASK_LOGIN;
      } else if(pContext->isAccessSuccess()){
        servo->attach(SERVO_PWM_PIN);
        servo->write(ANGLE_TO_PULSE(180));
//        servo->detach();
        Bluetooth.sendMsg(MSG_ACC_OK);
        pContext->setAccessSuccess(false);
        pContext->setAskLogin(false);
        elapsed = 0;
        tStart = millis();
        state = LOGIN_SUCCESSFUL;
      }
      else if(pContext->isAccessFail()){
        pContext->setUser("");
        pContext->setPassword("");
        pContext->setAccessFail(false);
        Bluetooth.sendMsg(MSG_ACC_FAIL);
        startCount = true;
        elapsed = 0;
        tStart = millis();
        state = ASK_LOGIN;
      }
//      if(buttonExit->isPressed()){
//        state = IDLE;
//        ledOn->switchOff();
//        pContext->setUser("");
//        pContext->setPassword("");
//      }
//      stato = "s = WAIT_LOGIN";
//      Logger.log(stato);
      break;
    case LOGIN_SUCCESSFUL:
//      if(buttonExit->isPressed()){
////           servo->attach(SERVO_PWM_PIN);
//           servo->write(ANGLE_TO_PULSE(0));
////           servo->detach();
//           state = IDLE;
//           pContext->setUser("");
//           pContext->setPassword("");
//       }
      elapsed = millis() - tStart;
      if (!pContext->isAppStarted()) {
              startCount = false;
              elapsed = 0;
              servo->write(ANGLE_TO_PULSE(0));
              state = WAIT_HELLO;
      }
      else if (elapsed >= MAX_DELAY) {
          startCount = false;
          elapsed = 0;
          Bluetooth.sendMsg(MSG_RESET_LS_WH);
          MsgService.sendMsg(MSG_ACC_FAIL_PIR);
          pContext->setAppStarted(false);
          servo->write(ANGLE_TO_PULSE(0));
          state = WAIT_HELLO;
      }
      else if(pir->isDetected()){
        Bluetooth.sendMsg(MSG_SESS_START);
        state = SESSION_WAIT; // waiting for session activity to be created.
       }
      break;
    case SESSION_WAIT:
       if (pContext->isSessionCreated()) {
          elapsed = 0;
          startCount = false;
          tStart = millis();
          state = SESSION;
          pContext->setSessionStarted(true);
          pContext->setFirstConn(true);
          MsgService.sendMsg(MSG_INSIDE);
//          Logger.log("LOGIN_SUCCESSFUL, pir seen"); 
       }
      break;
    case SESSION:
//      Logger.log("SESSION");
        elapsed = millis() - tStart;
        if(!pContext->isSessionStarted()) {
          MsgService.sendMsg(MSG_QUIT_GW);
          servo->write(ANGLE_TO_PULSE(0));
          pContext->setUser("");
          pContext->setPassword("");
          pContext->setFirstConn(false);
          pContext->setAppStarted(false);
          pContext->setSessionCreated(false);
          state = WAIT_HELLO;
          Logger.log("to wait hello");
        }
        else if(buttonExit->isPressed()) {
          Logger.log("Exit pressed");
          MsgService.sendMsg(MSG_QUIT_GW);
          Bluetooth.sendMsg(MSG_QUIT); /*q --> quit*/ 
          servo->write(ANGLE_TO_PULSE(0));
          pContext->setUser("");
          pContext->setPassword("");
          pContext->setFirstConn(false);
          pContext->setAppStarted(false);
          pContext->setSessionStarted(false);
          pContext->setSessionCreated(false);
          state = WAIT_HELLO;
          Logger.log("to wait hello");
        }
        else if (elapsed >= SESS_TIMEOUT) {
          Logger.log("Sess timeout");
          pContext->setSessionStarted(false);
          Bluetooth.sendMsg(MSG_SESS_TIMEOUT);
          MsgService.sendMsg(MSG_QUIT_GW);
          servo->write(ANGLE_TO_PULSE(0));
          pContext->setUser("");
          pContext->setPassword("");
          pContext->setFirstConn(false);
          pContext->setAppStarted(false);
          pContext->setSessionCreated(false);
          state = WAIT_HELLO;
          Logger.log("to wait hello");
        }
        else if (pir->isDetected()) {
          Logger.log("Pir detected");
          tStart = millis();
        }
        break;
  }
}


