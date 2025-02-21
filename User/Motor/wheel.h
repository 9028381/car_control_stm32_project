#ifndef __WHEEL_H
#define __WHEEL_H

#include "main.h"
#include "pid.h"

#define TRUST_CONFINE 3000

typedef struct WHEEL {
  uint8_t which;      // 电机编号 1-4
  int16_t trust;      // 电机推力
  int16_t cur_speed;  // 电机当前速度
  int16_t tar_speed;  // 电机目标速度
  int8_t dir;         // 电机方向
  PID wheel_pid;
} WHEEL;

int16_t get_wheel_speed(WHEEL *wheel);
void driver_wheel(WHEEL *wheel);
void init_wheel(WHEEL *wheel, uint8_t which, int8_t dir);

#endif
