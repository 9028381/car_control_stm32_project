#ifndef __STATUS_H
#define __STATUS_H

#include "gy901.h"
#include "main.h"
#include "motor.h"
#include "pid.h"
#include "servo.h"

extern STATUS status;

typedef struct ANGLE {
  float pitch;
  float roll;
  float yaw;
} ANGLE;

typedef struct SENSOR {
  ANGLE angle;
  int16_t Line;
} SENSOR;

typedef struct MOTOR {
  uint8_t which;      // 电机编号 1-4
  int16_t trust;      // 电机推力
  int16_t cur_speed;  // 电机当前速度
  int16_t tar_speed;  // 电机目标速度
  int8_t dir;         // 电机方向
  PID motor_pid;
} MOTOR;

typedef struct SERVO {
  uint8_t which;
  float angle;
  float max_angle;
} SERVO;

typedef struct STATUS {
  int64_t time;    // 系统时间单位ms
  int8_t T;        // 系统周期单位ms
  SENSOR sensor;   // 传感器数据
  MOTOR motor[4];  // 电机数据
  SERVO servo[2];  // 舵机数据
} STATUS;

#endif
