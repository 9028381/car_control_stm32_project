#include "led.h"

#include "gpio.h"
#include "log.h"
#include "status.h"
#include "stdbool.h"

void driver_LED(LED *led) {
	log_uprintf(&huart1, "time: %d %d %d\n", status.device.led1.on, status.device.led1.High_level_is_on, ((bool)(led->on) ^ (bool)(led->High_level_is_on)));
  if (led->which == 1) {
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 1^((bool)(led->on) ^ (bool)(led->High_level_is_on)));
		
  }

  return;
}