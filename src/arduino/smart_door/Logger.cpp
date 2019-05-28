#include "Logger.h"
#include "MsgService.h"

#define LOG_MSG "7"

LoggerService Logger;

void LoggerService::log(const String& msg){
  String logMsg = String(LOG_MSG) + "|" + msg;
  MsgService.sendMsg(logMsg);
}




