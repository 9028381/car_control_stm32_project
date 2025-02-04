#ifndef __LQ_STEP_H
#define __LQ_STEP_H

#include "main.h"
#include "math.h"
#include "usart.h"

void set_lq_step_abslute_angle(UART_HandleTypeDef huart, float angle);
void trun_lq_step_angle(UART_HandleTypeDef huart, float angle, uint8_t dir);
void trun_lq_step_speed(UART_HandleTypeDef huart, float speed, uint8_t dir);
void turn_lq_step_current(UART_HandleTypeDef huart, uint16_t current, uint8_t dir);

#endif
