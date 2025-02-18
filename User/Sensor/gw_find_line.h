// @551

#ifndef __GW_FIND_LINE_H
#define __GW_FIND_LINE_H

#include "main.h"
#include "road.h"

void gw_gray_get_line_digital_is_black();
int16_t compute_gw_gray_diff(uint8_t gray);
void set_gw_gray_mode(uint8_t cmd);
LINE_TYPE get_line_value();

#endif
