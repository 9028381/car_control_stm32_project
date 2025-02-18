// @63

#include "log.h"

#include "stdarg.h"
#include "stdio.h"
#include "usart.h"

#define LOG_FORMAT_BUF_LENGTH 256

#define STM32

#ifdef STM32

void log_uprintf(UART_HandleTypeDef *huart, const char *format, ...) {
  static unsigned char abbuf = 0;
  static char buf[2][LOG_FORMAT_BUF_LENGTH];

  abbuf = abbuf ? 0 : 1;

  va_list args;
  va_start(args, format);
  unsigned int len =
      vsnprintf(buf[abbuf], LOG_FORMAT_BUF_LENGTH - 1, format, args);
  va_end(args);

  HAL_UART_Transmit(huart, (uint8_t *)buf[abbuf], len, 100);
}

#endif