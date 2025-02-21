#include "buzzer.h"
#include "ccd.h"
#include "led.h"
#include "log.h"
#include "servo.h"
#include "status.h"
#include "tim.h"
#include "usart.h"

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

    if (status.time % 20 == 0) {  // 周期 20ms
      update_status(&status);
      driver_status(&status);
    }

  } else if (htim == &htim6) {  // 周期 5us
    driver_ccd();
  }

  return;
}