// @551

#ifndef __SERVO_H
#define __SERVO_H

#include "main.h"

typedef struct SERVO {
  uint8_t which;
  float angle;
  float max_angle;
} SERVO;

void driver_servo(SERVO *servo);
void init_servo(SERVO *servo, uint8_t which, float max_angle);

#endif
