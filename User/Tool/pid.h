// @551

#ifndef __PID_H
#define __PID_H

#include "main.h"

typedef struct PID {
  float kp;
  float ki;
  float kd;
  float T;  // 采样周期
  float error;
  float last_error;
  float integral;
  float derivative;
  float integral_max;
  uint8_t is_first;
} PID;

// PID 初始化函数 参数 T: pid周期 单位ms integral_max: 积分限幅
PID init_pid(float kp, float ki, float kd, float T, float integral_max);
// PID 计算函数 参数 error: 误差
float compute_pid(PID *pid, float error);

/*
使用示例：
PID pid = init_pid(1, 0.1, 0.1, 0.1, 100);

while(1) {
  float error = get_aerror();
  float output = compute_pid(&pid, error);
  set_output(output);
  HAL_Delay(100);
  }
*/

#endif
