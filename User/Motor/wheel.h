#ifndef __WHEEL_H
#define __WHEEL_H

#include "main.h"
#include "pid.h"

#define TRUST_CONFINE 3000

// WHEEL 结构体
// 挂载于 status motor
// 用于驱动直流电机 默认挂载四个
typedef struct WHEEL {
  uint8_t which;      // 电机编号 1-4
  int16_t trust;      // 电机推力
  int16_t cur_speed;  // 电机当前速度
  int16_t tar_speed;  // 电机目标速度
  int8_t dir;         // 电机方向
  PID wheel_pid;
} WHEEL;

// 更新轮子当前速度至状态树： status.motor.wheel[0].cur_speed = get_wheel_speed(&status->motor.wheel[0]);
// 设置轮子目标速度至500： status.motor.wheel[0].tar_speed = 500;

// 获取当前轮子速度 放在status_update()中 只能定期调用 返回编码器定时器的当前CNT值于上次CNT值的插值
int16_t get_wheel_speed(WHEEL *wheel);
// 驱动轮子 用于设置轮子的pwm占空比与方向设置引脚 放在status_driver()中
void driver_wheel(WHEEL *wheel);
// 初始化轮子 which传入轮子的编号 默认1-4 用于识别电机调用对应的硬件 dir传入 1或-1 用于设置轮子正转时的方向 放在init_motor()中
void init_wheel(WHEEL *wheel, uint8_t which, int8_t dir);

#endif
