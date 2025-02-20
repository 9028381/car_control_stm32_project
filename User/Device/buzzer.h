#ifndef __BUZZER_H
#define __BUZZER_H

#include "main.h"

typedef struct BUZZER {
  uint8_t which;
  uint8_t High_level_is_on;
  uint8_t on;
} BUZZER;

void driver_BUZZER(BUZZER *buzzer);
void init_BUZZER(BUZZER *buzzer, uint8_t which, uint8_t High_level_is_on);

#endif