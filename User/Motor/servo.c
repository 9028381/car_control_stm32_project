#include "servo.h"

void set_servo_angle(SERVO *servo) {
  if (servo->which == 1) {
    __HAL_TIM_SET_COMPARE(&htim15, TIM_CHANNEL_1, (int)(((servo->angle / servo->max_angle) * 0.4 + 0.1) * 50000));
  } else if (servo->which == 2) {
    __HAL_TIM_SET_COMPARE(&htim15, TIM_CHANNEL_2, (int)(((servo->angle / servo->max_angle) * 0.4 + 0.1) * 50000));
  }

  return;
}
