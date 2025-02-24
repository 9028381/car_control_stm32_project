#ifndef __BUZZER_H
#define __BUZZER_H

#include "main.h"

// BUZZER 结构体
// 挂载于 status device
// 用于驱动蜂鸣器
typedef struct BUZZER {
  uint8_t which;             // LED编号
  uint8_t High_level_is_on;  // 用于设置该led是高电平亮还是低电平亮
  uint8_t on;                // 设置LED亮灭
} BUZZER;

// 设置蜂鸣器响 status.device.buzzer.on = 1;

// 驱动蜂鸣器 根据状态设置蜂鸣器是否发声 放在status_driver()中
void driver_BUZZER(BUZZER *buzzer);
// 初始化蜂鸣器 which传入LED编号 High_level_is_on用于设置该蜂鸣器是高电平响还是低电平响 放在init_device()中
void init_BUZZER(BUZZER *buzzer, uint8_t which, uint8_t High_level_is_on);

#endif