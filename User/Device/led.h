#ifndef __LED_H
#define __LED_H

#include "main.h"

// LED 结构体
// 挂载于 status device
// 用于驱动LED
typedef struct LED {
  uint8_t which;             // LED编号
  uint8_t High_level_is_on;  // 用于设置该led是高电平亮还是低电平亮
  uint8_t on;                // 设置LED亮灭
} LED;

// 设置LED点亮 status.device.led.on = 1;

// 驱动LED 根据状态设置LED亮灭 放在status_driver()中
void driver_LED(LED *led);
// 初始化LED which传入LED编号 High_level_is_on用于设置该led是高电平亮还是低电平亮 放在init_device()中
void init_LED(LED *led, uint8_t which, uint8_t High_level_is_on);

#endif  // __LED_H