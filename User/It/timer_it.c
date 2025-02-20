#include "buzzer.h"
#include "ccd.h"
#include "led.h"
#include "log.h"
#include "servo.h"
#include "status.h"
#include "tim.h"
#include "usart.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim == &htim5)  // 周期 1ms
  {
    status.time += status.T;
    if (status.time == 200)
      status.device.buzzer.on = 1;
    if (status.time == 500)
      status.device.buzzer.on = 0;
    driver_button(&status.device.button_D2);
    driver_button(&status.device.button_B11);
    driver_LED(&status.device.led_on_board);
    driver_LED(&status.device.led1);
    driver_LED(&status.device.led2);
    driver_servo(&status.servo[0]);
    driver_servo(&status.servo[1]);
    driver_BUZZER(&status.device.buzzer);
  } else if (htim == &htim6) {  // 周期 5us
    driver_ccd();
  }

  return;
}