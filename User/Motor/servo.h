// @551

#ifndef __SERVO_H
#define __SERVO_H

#include "main.h"

// SERVO 结构体
// 挂载于 status motor
// 用于驱动舵机 默认挂载两个
typedef struct SERVO {
  uint8_t which;    // 舵机编号 1-2
  float angle;      // 舵机目标角度
  float max_angle;  // 舵机最大角度 如180 270
} SERVO;

// 设置 舵机1 角度至90度： status.motor.servo[0].angle = 90;

// 驱动舵机 用于设置舵机的pwm占空比 放在status_driver()中
void driver_servo(SERVO *servo);
// 初始化舵机 which传入舵机编号 默认1-2 用于识别舵机 调用对应硬件 max_angle传入舵机最大范围 放在init_motor()中
void init_servo(SERVO *servo, uint8_t which, float max_angle);

#endif
