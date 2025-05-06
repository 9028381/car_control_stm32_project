#include "button.h"

#include "gpio.h"
#include "gw_anagloge.h"
#include "led.h"
#include "log.h"
#include "lq_step.h"
#include "math_tool.h"
#include "status.h"
#include "usart.h"

extern PID balance_pid;
extern int32_t rw_time_cur;
uint8_t is_p = 1;
extern uint8_t wait_finish_flag;
uint8_t button_2_cnt = 0;  // 按键2计数器

void server_button(BUTTON *button, BUTTON_STATION station) {
  if (button->which == 1) {
    if (station == BUTTON_DOWN) {
      correct_gw_analogue(&status.sensor.gw_analogue);
    }
  }
  if (status.state.motion == STOP) {
    if (button->which == 2) {
      if (button_2_cnt == 0) {
        if (station == BUTTON_DOWN) {
          rw_time_cur = status.state.time;
          status.state.base_speed = 40;
          status.state.motion = FIND_LINE;
          rw_time_cur = status.state.time;
        }
      } else if (button_2_cnt == 1) {
        wait_finish_flag = 1;
        status.device.led1.on = 0;
      }
      button_2_cnt++;
    }
  }
  return;
}

void driver_button(BUTTON *button) {
  if (button->which == 1) {
    button->now = HAL_GPIO_ReadPin(BUTTON_D2_GPIO_Port, BUTTON_D2_Pin);
  } else if (button->which == 2) {
    button->now = HAL_GPIO_ReadPin(BUTTON_B11_GPIO_Port, BUTTON_B11_Pin);
  }

  if (1 ^ (button->now ^ button->Press_is_high_level)) {  // 按键长按检测
    if (button->long_press_cnt > 0) {
      button->long_press_cnt--;
    } else if (button->long_press_cnt == 0) {
      server_button(button, BUTTON_LONG);
      button->long_press_cnt = -1;
    }
  } else {
    button->long_press_cnt = LONG_PRESS_CNT;
  }

  if (button->now != button->last) {  // 按键短按检测
    if (button->Press_is_high_level == 1) {
      if (button->now == 1) {
        server_button(button, BUTTON_DOWN);
        // 按键按下
        if (button->long_press_cnt - 1 >= 0) {
          button->long_press_cnt--;
        } else {
          server_button(button, BUTTON_LONG);
        }
      } else {
        server_button(button, BUTTON_UP);
        // 按键释放
        button->long_press_cnt = LONG_PRESS_CNT;
      }
    } else {
      if (button->now == 0) {
        server_button(button, BUTTON_DOWN);
        // 按键按下
      } else {
        server_button(button, BUTTON_UP);
        // 按键释放
        button->long_press_cnt = LONG_PRESS_CNT;
      }
    }
    button->last = button->now;
  }
}

void init_button(BUTTON *button, uint8_t which, uint8_t Press_is_high_level) {
  button->which = which;
  button->Press_is_high_level = Press_is_high_level;
  button->last = Press_is_high_level ? 0 : 1;
  button->now = Press_is_high_level ? 0 : 1;
  button->long_press_cnt = LONG_PRESS_CNT;
  return;
}