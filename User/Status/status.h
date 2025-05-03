// @63 @551

#ifndef __STATUS_H
#define __STATUS_H

#include "button.h"
#include "buzzer.h"
#include "gw_anagloge.h"
#include "gw_find_line.h"
#include "gy901.h"
#include "led.h"
#include "main.h"
#include "pid.h"
#include "servo.h"
#include "wheel.h"

#define MOTION_BASE_SPEED 2000

typedef struct STATUS_PID {
  PID follow_line_pid;
  PID keep_angle_pid;  // PID结构体
} STATUS_PID;

typedef enum MOTION_STATION {
  STOP,
  KEEP_ANGLE,
  FIND_LINE,
} MOTION_STATION;

typedef struct SENSOR {
  GYR gy901;
  GW_8BIT gw_8bit;
  GW_ANALOGUE gw_analogue;
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

  MOTION_STATION motion;
  float initial_angle;
  float cur_angle;
  float tar_angle;

  int16_t base_speed;  // 基础速度

  RoadDetermine road_determine;  // 道路判断结构体

  uint8_t gw_8bit;

  STATUS_PID status_pid;  // PID结构体
} STATE;

typedef struct STATUS {
  STATE state;
  SENSOR sensor;  // 传感器数据
  MOTOR motor;    // 电机数据
  DEVICE device;
} STATUS;

extern STATUS status;

void after_init_state();
void init_status(STATUS *status, uint8_t T);
void update_status(STATUS *status);
void driver_status(STATUS *status);

#endif
