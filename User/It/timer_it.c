#include "buzzer.h"
#include "ccd.h"
#include "led.h"
#include "log.h"
#include "lq_step.h"
#include "servo.h"
#include "status.h"
#include "tim.h"
#include "usart.h"

uint8_t update_or_driver = 0;    // 0 : upadte  1 : driver
extern int32_t rw_time_cur;      // 临时使用的时间变量
extern int32_t rw_time_tar;      // 临时使用的时间变量
extern uint8_t cross_cnt;        // 路口计数器
uint8_t wait_finish_flag = 0;    // 等待完成标志位
extern int32_t keep_angle_time;  // 保持角度时间
extern uint8_t speed_show_flag;  // 显示速度标志位
uint8_t is_init = 0;

uint8_t maixcam[3] = {0xAA, 0x02, 0xBB};
uint8_t find_voice[3] = {0xAA, 0x01, 0xBB};

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
      if (is_init == 0) {
        if (status.state.time == rw_time_cur + 300) {
          status.state.initial_angle = status.state.cur_angle;
        }
        is_init = 1;
      }
      if (cross_cnt == 0) {
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
      } else if (cross_cnt == 1) {
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
          status.state.base_speed = 40;
        }
      } else if (cross_cnt == 2) {
        if (status.state.time == rw_time_cur + 500) {
          status.state.base_speed = -40;
          status.state.road_determine.integral_times = 6;
        }
      } else if (cross_cnt == 3) {
        if (status.state.time == rw_time_cur + 500) {
          status.state.base_speed = 40;
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
    if (rw_time_tar != -1) {
      if (status.state.time == rw_time_tar + 500) {
        status.state.base_speed = 40;
      } else if (status.state.time == rw_time_tar + 1500) {
        status.state.base_speed = 0;
      } else if (status.state.time == rw_time_tar + 1550) {
        status.device.buzzer.on = 1;
      } else if (status.state.time == rw_time_tar + 1600) {
        status.device.buzzer.on = 0;
      } else if (status.state.time == rw_time_tar + 1650) {
        status.device.buzzer.on = 1;
      } else if (status.state.time == rw_time_tar + 1700) {
        status.device.buzzer.on = 0;
      } else if (status.state.time == rw_time_tar + 1750) {
        status.device.buzzer.on = 1;
      } else if (status.state.time == rw_time_tar + 1800) {
        status.device.buzzer.on = 0;
        status.state.motion = STOP;
      } else if (status.state.time == rw_time_tar + 1850) {
        status.device.led1.on = 1;
        HAL_UART_Transmit(&huart2, find_voice, 3, 100);
      }
      if (wait_finish_flag == 1) {
        wait_finish_flag = 0;
        status.state.tar_angle = -63.5;
        status.state.motion = KEEP_ANGLE;
      }
      if (status.state.time == keep_angle_time + 3500) {
        status.state.base_speed = 0;
        status.state.motion = STOP;
        HAL_UART_Transmit(&huart3, maixcam, 3, 100);
      } else if (status.state.time == keep_angle_time + 50 + 3500) {
        status.device.buzzer.on = 1;
      } else if (status.state.time == keep_angle_time + 100 + 3500) {
        status.device.buzzer.on = 0;
      } else if (status.state.time == keep_angle_time + 150 + 3500) {
        status.device.buzzer.on = 1;
      } else if (status.state.time == keep_angle_time + 200 + 3500) {
        status.device.buzzer.on = 0;
      } else if (status.state.time == keep_angle_time + 250 + 3500) {
        status.device.buzzer.on = 1;
      } else if (status.state.time == keep_angle_time + 300 + 3500) {
        status.device.buzzer.on = 0;
      }
    }
    if (status.state.time % 20 == 0) {  // 周期 25ms
      update_status(&status);           // 状态更新中断 用于读取传感器原始数据
    }
    if (status.state.time % 100 == 0) {  // 周期 100ms
      log_uprintf(&huart4, "n0.val=%d\xff\xff\xff", (int)(0.83 * ((status.motor.wheel[0].cur_speed + status.motor.wheel[1].cur_speed) / 2)));
    }
  }
}
