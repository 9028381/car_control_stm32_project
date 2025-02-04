// @551

#ifndef __GW_FIND_LINE_H
#define __GW_FIND_LINE_H

#include "main.h"

uint8_t gw_gray_get_line_digital_is_black();
int16_t compute_gw_gray_diff(uint8_t gray[8]);
void set_gw_gray_mode(uint8_t cmd);

#endif
