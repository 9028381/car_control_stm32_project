#ifndef __MOTOR_H
#define __MOTOR_H

#include "gpio.h"
#include "main.h"
#include "motor.h"
#include "pid.h"
#include "status.h"
#include "tim.h"

#define TRUST_CONFINE 1000

int16_t get_motor_speed(MOTOR *motor);
void driver_motor(MOTOR *motor);

#endif
