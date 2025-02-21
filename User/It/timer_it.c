#include "buzzer.h"
#include "ccd.h"
#include "led.h"
#include "log.h"
#include "servo.h"
#include "status.h"
#include "tim.h"
#include "usart.h"

uint8_t update_or_driver = 0;  // 0 : upadte  1 : driver

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  status.time += status.T;  // 更新系统时间

  if (htim == &htim5)  // 周期 1ms
  {
    if (status.time == 50)
      status.device.led_on_board.on = 1;
    else if (status.time == 100)
      status.device.led_on_board.on = 0;
    else if (status.time == 150)
      status.device.led_on_board.on = 1;
    else if (status.time == 200)
      status.device.led_on_board.on = 0;
    else if (status.time == 250)
      status.device.led_on_board.on = 1;
    else if (status.time == 300)
      status.device.led_on_board.on = 0;

    if (status.time % 25 == 0) {  // 周期 25ms
      if (update_or_driver == 0) {
        update_status(&status);
        update_or_driver = 1;
      } else {
        driver_status(&status);
        update_or_driver = 0;
      }
    }
  } else if (htim == &htim6) {  // 周期 5us
    driver_ccd();
  }

  return;
}