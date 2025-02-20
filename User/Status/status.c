// @63 @551

#include "status.h"

#include "button.h"
#include "buzzer.h"
#include "led.h"
#include "motor.h"
#include "servo.h"

STATUS status;

void init_motor(STATUS *status) {
  status->motion = STOP;  // 初始化运动状态为 STOP

  status->motor[0].which = 1;
  status->motor[0].trust = 0;
  status->motor[0].cur_speed = 0;
  status->motor[0].tar_speed = 0;
  status->motor[0].dir = 0;
  status->motor[0].motor_pid = init_pid(0.1, 0.1, 0.1, status->T, 10);

  status->motor[1].which = 2;
  status->motor[1].trust = 0;
  status->motor[1].cur_speed = 0;
  status->motor[1].tar_speed = 0;
  status->motor[1].dir = 0;
  status->motor[1].motor_pid = init_pid(0.1, 0.1, 0.1, status->T, 10);

  status->motor[2].which = 3;
  status->motor[2].trust = 0;
  status->motor[2].cur_speed = 0;
  status->motor[2].tar_speed = 0;
  status->motor[2].dir = 0;
  status->motor[2].motor_pid = init_pid(0.1, 0.1, 0.1, status->T, 10);

  status->motor[3].which = 4;
  status->motor[3].trust = 0;
  status->motor[3].cur_speed = 0;
  status->motor[3].tar_speed = 0;
  status->motor[3].dir = 0;
  status->motor[3].motor_pid = init_pid(0.1, 0.1, 0.1, status->T, 10);

  status->device.led1.which = 1;
  status->device.led1.High_level_is_on = 1;
  status->device.led1.on = 0;

  init_servo(&status->servo[0], 1, 180);
  init_servo(&status->servo[1], 2, 270);

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
  status->sensor.angle.pitch = 0;
  status->sensor.angle.roll = 0;
  status->sensor.angle.yaw = 0;
  status->sensor.Line = 0;
}

void init_status(STATUS *status, uint8_t T) {
  status->time = 0;
  status->T = T;

  init_sensor(status);

  init_motor(status);

  init_device();

  return;
}

void update_status(STATUS *status) {
  status->time += status->T;  // 更新系统时间

  status->sensor.angle.pitch = get_gyr_value(gyr_y_pitch);  // 更新传感器数据
  status->sensor.angle.roll = get_gyr_value(gyr_x_roll);
  status->sensor.angle.yaw = get_gyr_value(gyr_z_yaw);

  status->motor[0].cur_speed = get_motor_speed(&status->motor[0]);
  status->motor[1].cur_speed = get_motor_speed(&status->motor[1]);
  status->motor[2].cur_speed = get_motor_speed(&status->motor[2]);
  status->motor[3].cur_speed = get_motor_speed(&status->motor[3]);

  // 进行状态设置

  driver_motor(&status->motor[0]);  // 执行状态更新
  driver_motor(&status->motor[1]);
  driver_motor(&status->motor[2]);
  driver_motor(&status->motor[3]);

  return;
}
