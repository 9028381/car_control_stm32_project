// @551

#include "ms_find_line.h"
#include "stdlib.h"

int16_t ms_diff = 0;
char ms_rx_buf[8] = {0};

void driver_ms_diff(uint8_t buf) {
  static unsigned char index = 0;
  if (buf == '\n') {
    ms_rx_buf[index] = '\0';
    ms_diff = atoi(ms_rx_buf);
    index = 0;
  } else {
    ms_rx_buf[index++] = buf;
  }
}

int16_t get_ms_value(void) { return -ms_diff / 10; }