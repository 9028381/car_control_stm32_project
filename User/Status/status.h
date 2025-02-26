// @63 @551

#ifndef __STATUS_H
#define __STATUS_H

#include "button.h"
#include "buzzer.h"
#include "gy901.h"
#include "led.h"
#include "main.h"
#include "motion.h"
#include "pid.h"
#include "servo.h"
#include "wheel.h"

typedef struct SENSOR {
  GYR gy901;
  int16_t Line;
} SENSOR;

typedef struct DEVICE {
  LED led_on_board;
  LED led1;
  LED led2;
  BUTTON button_D2;
  BUTTON button_B11;
  BUZZER buzzer;
} DEVICE;

typedef struct MOTOR {
  WHEEL wheel[4];
  SERVO servo[2];
} MOTOR;

typedef struct STATE {
  int8_t T;  // 系统周期单位ms
  uint64_t time;
  MOTION motion;
  float initial_angle;
  float cur_angle;
  float cur_angle;
} STATE;

typedef struct STATUS {
  STATE state;
  SENSOR sensor;  // 传感器数据
  MOTOR motor;    // 电机数据
  DEVICE device;
} STATUS;

extern STATUS status;

void init_status(STATUS *status, uint8_t T);
void update_status(STATUS *status);
void driver_status(STATUS *status);

#endif
