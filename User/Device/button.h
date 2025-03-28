#ifndef __BUTTON_H_
#define __BUTTON_H_

#include "main.h"

#define LONG_PRESS_CNT 20  // 长按判断时间 单位 ms

// 按键结构体
// 挂载于 status device
// 用于驱动按键
typedef struct BUTTON {
  uint8_t last;                 // 按键上次状态
  uint8_t now;                  // 按键当前状态
  uint8_t Press_is_high_level;  // 按下电平为高还是低
  uint8_t which;                // 按键编号
  int16_t long_press_cnt;       // 长按计数
} BUTTON;

// 按键事件枚举
typedef enum BUTTON_STATION {
  BUTTON_UP,    // 按键抬起
  BUTTON_DOWN,  // 按键按下
  BUTTON_LONG,  // 按键长按
} BUTTON_STATION;

// 按键服务函数 该函数在driver_button()中已经被调用 请勿在其他地方调用 传入按键以及按键状态 使用时在button.c中修改调用效果
/*
使用示例 当编号为1的按钮发生按键按下事件时将wheel[0]的目标速度反向

void server_button(BUTTON *button, BUTTON_STATION station) {
  if (button->which == 1) {                                                      // 判断按键编号 为1
    if (station == BUTTON_DOWN) {                                                // 判断按键事件 为BUTTON_DOWN
      status.motor.wheel[0].tar_speed = -status.motor.wheel[0].tar_speed;        // 反向wheel[0]tar
    }
  }
  return;
}
*/
void server_button(BUTTON *button, BUTTON_STATION station);
// 按键驱动函数 放置在1ms中断中
void driver_button(BUTTON *button);
// 初始化按键 which传入按键编号 Press_is_high_level传入按键按下后是高电平还是低电平 放在init_device()中
void init_button(BUTTON *button, uint8_t which, uint8_t Press_is_high_level);

#endif
