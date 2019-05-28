#ifndef __CONFIG__
#define __CONFIG__

#define DOOR_PERIOD 50
#define DIST_MIN 0.5
#define MIN_SEC 5000 //ms
#define MAX_DELAY 10000 //ms

#define L1_PIN 13
#define L2_PIN 5 // 11
#define B_OFF 12
#define PIR 6 //attenzione
#define SONAR_TRIG_PIN 8
#define SONAR_ECHO_PIN 7
#define SERVO_PWM_PIN 9
#define TEMP_PIN A4 //attenzione
#define RB 2 //lettura bluetooth
#define TB 3 //scrittura bluetooth

#define DHTPIN 4
#define DHTTYPE DHT11

#define RESET_TIMEOUT 30000
#define GW_ERR_TIMEOUT 10000
#define SESS_TIMEOUT 60000

#endif

