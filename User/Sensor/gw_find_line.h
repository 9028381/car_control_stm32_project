// @551

#ifndef __GW_FIND_LINE_H
#define __GW_FIND_LINE_H

#include "main.h"
#include "pid.h"
#include "road.h"

typedef struct GW_8BIT {
  uint8_t data_buf;
  int16_t gw_bit_weight[8];
  uint8_t integral;
  uint8_t maybe;
  uint8_t cross_cnt;
  Road cross;
  PID gw_find_line_pid;
  int32_t gw_diff;
} GW_8BIT;

void get_gw_8bit_data(I2C_HandleTypeDef *hi2c, GW_8BIT *gw_8bit);
void get_gw_8bit_update_analog_data(I2C_HandleTypeDef *hi2c, GW_8BIT *gw_8bit);
void init_gw_8bit(GW_8BIT *gw_8bit);
int32_t gw_get_line_diff(GW_8BIT *gw_8bit);

#endif
