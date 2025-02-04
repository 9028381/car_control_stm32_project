// @551
// 布瑞特绝对值编码器的驱动程序

#ifndef ABSLUTE_ANGLE_SENSOR_H
#define ABSLUTE_ANGLE_SENSOR_H

#define ABS_ANGLE_UART huart2

#include "main.h"
#include "math_tool.h"
#include "usart.h"

#define ACCURACY 12
#define MAX_CNT POW(2, ACCURACY)

float get_abslute_angle_value();
float get_set_angle_value();
float driver_abslute_angle();

#endif
