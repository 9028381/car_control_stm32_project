// @551

#include "servo.h"

#include "log.h"
#include "status.h"
#include "tim.h"

void driver_servo(SERVO *servo) {
  if (servo->which == 1) {
    __HAL_TIM_SET_COMPARE(&htim15, TIM_CHANNEL_2, (int)((((float)servo->angle / (float)servo->max_angle) * 0.1 + 0.025) * 50000));
    log_uprintf(&huart1, "servo1: %d %f\r\n", (int)((((float)servo->angle / (float)servo->max_angle) * 0.1 + 0.025) * 50000), servo->angle);
  } else if (servo->which == 2) {
    __HAL_TIM_SET_COMPARE(&htim15, TIM_CHANNEL_1, (int)((((float)servo->angle / (float)servo->max_angle) * 0.1 + 0.025) * 50000));
  }

  return;
}

void init_servo(SERVO *servo, uint8_t which, float max_angle) {
  servo->which = which;
  servo->max_angle = max_angle;
  servo->angle = max_angle / 2;
  return;
}
