#include "buzzer.h"
#include "ccd.h"
#include "led.h"
#include "log.h"
#include "lq_step.h"
#include "servo.h"
#include "status.h"
#include "tim.h"
#include "usart.h"

uint8_t update_or_driver = 0;  // 0 : upadte  1 : driver
extern int32_t rw_time_cur;    // 临时使用的时间变量
extern int32_t rw_time_tar;    // 临时使用的时间变量
extern uint8_t cross_cnt;      // 路口计数器

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  status.state.time += status.state.T;  // 更新系统时间

  if (htim == &htim5)  // 周期 1ms
  {
    if (status.state.time == 50)
      status.device.led_on_board.on = 1;
    else if (status.state.time == 100)
      status.device.led_on_board.on = 0;
    else if (status.state.time == 150)
      status.device.led_on_board.on = 1;
    else if (status.state.time == 200)
      status.device.led_on_board.on = 0;
    else if (status.state.time == 250)
      status.device.led_on_board.on = 1;
    else if (status.state.time == 300)
      status.device.led_on_board.on = 0;

    if (rw_time_cur != -1) {
      if (status.state.time == rw_time_cur + 50)
        status.device.buzzer.on = 1;
      else if (status.state.time == rw_time_cur + 100)
        status.device.buzzer.on = 0;
      else if (status.state.time == rw_time_cur + 150)
        status.device.buzzer.on = 1;
      else if (status.state.time == rw_time_cur + 200)
        status.device.buzzer.on = 0;
      else if (status.state.time == rw_time_cur + 250)
        status.device.buzzer.on = 1;
      else if (status.state.time == rw_time_cur + 300)
        status.device.buzzer.on = 0;
      else if (status.state.time == rw_time_cur + 500) {
        if (cross_cnt == 1)
          status.state.base_speed = 40;
        if (cross_cnt == 2) {
          status.state.base_speed = -40;
          status.state.road_determine.integral_times = 6;
        }
        if (cross_cnt == 3) {
          status.state.motion = KEEP_ANGLE;
        }
      }
    }
    if (rw_time_tar != -1) {
      if (status.state.motion == KEEP_ANGLE) {
        if (status.state.time == rw_time_tar + 4500)
          status.state.base_speed = 0;
        if (status.state.time == rw_time_tar + 50 + 4500)
          status.device.buzzer.on = 1;
        else if (status.state.time == rw_time_tar + 100 + 4500)
          status.device.buzzer.on = 0;
        else if (status.state.time == rw_time_tar + 150 + 4500)
          status.device.buzzer.on = 1;
        else if (status.state.time == rw_time_tar + 200 + 4500)
          status.device.buzzer.on = 0;
        else if (status.state.time == rw_time_tar + 250 + 4500)
          status.device.buzzer.on = 1;
        else if (status.state.time == rw_time_tar + 300 + 4500)
          status.device.buzzer.on = 0;
      }
    }
    if (status.state.time % 10 == 0) {  // 周期 25ms
      if (update_or_driver == 0) {
        update_status(&status);  // 状态更新中断 用于读取传感器原始数据
        update_or_driver = 1;
      } else {
        driver_status(&status);  // 状态驱动中断 用于处理传感器数据与驱动外设
        update_or_driver = 0;
      }
    }
  } else if (htim == &htim6) {  // 周期 5us
    driver_ccd();
  }

  return;
}