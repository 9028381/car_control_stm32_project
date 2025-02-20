#ifndef __BUTTON_H_
#define __BUTTON_H_

#include "main.h"

#define LONG_PRESS_CNT 500

typedef struct BUTTON {
  uint8_t last;
  uint8_t now;
  uint8_t Press_is_high_level;
  uint8_t which;
  int16_t long_press_cnt;
} BUTTON;

typedef enum BUTTON_STATION {
  BUTTON_UP,
  BUTTON_DOWN,
  BUTTON_LONG,
} BUTTON_STATION;

void server_button(BUTTON *button, BUTTON_STATION station);
void driver_button(BUTTON *button);
void init_button(BUTTON *button, uint8_t which, uint8_t Press_is_high_level);

#endif
