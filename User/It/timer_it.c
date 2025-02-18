#include "ccd.h"
#include "led.h"
#include "log.h"
#include "status.h"
#include "tim.h"
#include "usart.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim == &htim5)  // 周期 1ms
  {
    status.time += status.T;

    if (status.time > 900) {
      status.device.led1.on = 1;
    }
    driver_LED(&status.device.led1);
  } else if (htim == &htim6) {  // 周期 5us
    driver_ccd();
  }

  return;
}