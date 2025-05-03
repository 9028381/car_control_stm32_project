#include "status.h"

#include "button.h"
#include "buzzer.h"
#include "gw_find_line.h"
#include "i2c.h"
#include "led.h"
#include "log.h"
#include "math_tool.h"
#include "road.h"
#include "servo.h"
#include "wheel.h"

STATUS status;

int32_t rw_time_cur = -1;
int32_t rw_time_tar = -1;
extern uint8_t left_cnt;

void init_motor() {
  init_servo(&status.motor.servo[0], 1, 180);
  init_servo(&status.motor.servo[1], 2, 270);

  init_wheel(&status.motor.wheel[0], 1, -1);
  init_wheel(&status.motor.wheel[1], 2, 1);

  return;
}

void init_device() {
  init_button(&status.device.button_D2, 1, 0);
  init_button(&status.device.button_B11, 2, 0);
  init_LED(&status.device.led_on_board, 1, 1);
  init_LED(&status.device.led1, 2, 1);
  init_LED(&status.device.led2, 3, 1);
  init_BUZZER(&status.device.buzzer, 1, 1);

  return;
}

void init_sensor(STATUS *status) {
  init_gyr(&status->sensor.gy901);
  init_gw_8bit(&status->sensor.gw_8bit);
  init_gw_analogue(&status->sensor.gw_analogue);
}

void init_state(STATUS *status, uint8_t T) {
  status->state.T = T;
  status->state.time = 0;
  status->state.motion = STOP;
  status->state.cur_angle = 0;
  status->state.tar_angle = 90;

  status->state.gw_8bit = 0x00;

  status->state.road_determine.cross = Straight;
  status->state.road_determine.cross_cnt = 0;
  status->state.road_determine.maybe = 0;
  status->state.road_determine.integral = 0;
  status->state.road_determine.data_buf = 0;
  status->state.road_determine.integral_times = 6;

  status->state.base_speed = 0;

  status->state.motion = STOP;

  return;
}

void init_status_pid(STATUS *status) {
  status->state.status_pid.follow_line_pid = init_pid(1.5, 0.1, 500, 20, 20);
  status->state.status_pid.keep_angle_pid = init_pid(1, 0, 1, 20, 20);
}

void init_status(STATUS *status, uint8_t T) {
  init_state(status, T);

  init_status_pid(status);

  init_sensor(status);

  init_motor();

  init_device();

  return;
}

Road road_buf = Straight;

Road Turn_or_Straight() {
  if (road_buf != status.state.road_determine.cross) {
    status.motor.wheel[0].tar_speed = 0;
    status.motor.wheel[1].tar_speed = 0;
    if ((ABS(status.motor.wheel[0].cur_speed) < 5) && (ABS(status.motor.wheel[1].cur_speed) < 5)) {
      road_buf = status.state.road_determine.cross;
    }
  }
  if (status.state.road_determine.cross == LeftRoad && left_cnt == 1) {
    status.state.base_speed = 60;
    status.state.road_determine.integral = 4;
  }
  return road_buf;
}

void follow_line(STATUS *status) {
  get_gw_analoge_digital_data(&status->sensor.gw_analogue);
  get_gw_analogue_analogue_diff(&status->sensor.gw_analogue);

  get_road_type(&status->state.road_determine, status->sensor.gw_analogue.digital_8bit);

  if (Turn_or_Straight() == Straight) {
    float diff = compute_pid(&status->state.status_pid.follow_line_pid, status->sensor.gw_analogue.diff);
    status->motor.wheel[0].tar_speed = status->state.base_speed - (int16_t)diff;
    status->motor.wheel[1].tar_speed = status->state.base_speed + (int16_t)diff;
  }
  if (Turn_or_Straight() == LeftRoad) {
    status->motor.wheel[0].tar_speed = 20;
    status->motor.wheel[1].tar_speed = -20;
  }
  if (Turn_or_Straight() == RightRoad) {
    status->motor.wheel[0].tar_speed = -20;
    status->motor.wheel[1].tar_speed = 20;
  }
  if (road_buf != status->state.road_determine.cross) {
    status->motor.wheel[0].tar_speed = 0;
    status->motor.wheel[1].tar_speed = 0;
  }
}

uint8_t cnt = 20;
uint8_t flag = 1;

void keep_angle(STATUS *status) {
  float target = status->state.tar_angle + status->state.initial_angle;  // 目标角度
  float diff_angle = target - status->state.cur_angle;
  if (diff_angle > 180.0) {
    diff_angle -= 360.0;
  } else if (diff_angle < -180.0) {
    diff_angle += 360.0;
  }
  float diff = compute_pid(&status->state.status_pid.keep_angle_pid, diff_angle);  // PID计算
  diff = CONFINE(diff, -25, 25);                                                   // 限制速度范围
  status->motor.wheel[0].tar_speed = status->state.base_speed + (int16_t)diff;
  status->motor.wheel[1].tar_speed = status->state.base_speed - (int16_t)diff;  // 设置电机速度

  if (ABS(diff_angle) < 1.0) {
    if (cnt > 0) {
      cnt--;
    } else {
      if (flag == 1) {
        rw_time_tar = status->state.time;
        status->state.base_speed = 40;
        flag = 0;
      }
    }
  }
}

void update_status(STATUS *status) {
  get_gw_raw_data(&status->sensor.gw_analogue);

  status->motor.wheel[0].cur_speed = get_wheel_speed(&status->motor.wheel[0]);
  status->motor.wheel[1].cur_speed = get_wheel_speed(&status->motor.wheel[1]);
  status->motor.wheel[2].cur_speed = get_wheel_speed(&status->motor.wheel[2]);
  status->motor.wheel[3].cur_speed = get_wheel_speed(&status->motor.wheel[3]);

  get_gyr_raw_data(&hi2c1, &status->sensor.gy901);

  status->state.cur_angle = get_gyr_value(&status->sensor.gy901, gyr_z_yaw);

  if (status->state.motion == FIND_LINE) {
    follow_line(status);
  }
  if (status->state.motion == KEEP_ANGLE) {
    keep_angle(status);
  }

  log_uprintf(&huart1, "ok");

  driver_button(&status->device.button_D2);
  driver_button(&status->device.button_B11);

  driver_LED(&status->device.led_on_board);
  driver_LED(&status->device.led1);
  driver_LED(&status->device.led2);

  driver_servo(&status->motor.servo[0]);
  driver_servo(&status->motor.servo[1]);

  driver_BUZZER(&status->device.buzzer);

  driver_wheel(&status->motor.wheel[0]);
  driver_wheel(&status->motor.wheel[1]);

  return;
}

void driver_status(STATUS *status) {  // 鐘舵€佹暟椹卞姩
}

void after_init_state() {
  get_gyr_raw_data(&hi2c1, &status.sensor.gy901);
  HAL_Delay(50);
  status.state.initial_angle = get_gyr_value(&status.sensor.gy901, gyr_z_yaw);
}