#ifndef __LED_H
#define __LED_H

#include "main.h"

typedef struct LED {
  uint8_t which;
  uint8_t High_level_is_on;
  uint8_t on;
} LED;

void driver_LED(LED *led);

#endif  // __LED_H