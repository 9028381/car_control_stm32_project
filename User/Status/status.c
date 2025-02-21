// @63 @551

#include "status.h"

#include "button.h"
#include "buzzer.h"
#include "led.h"
#include "log.h"
#include "servo.h"
#include "wheel.h"

STATUS status;

void init_motor() {
  init_servo(&status.motor.servo[0], 1, 180);
  init_servo(&status.motor.servo[1], 2, 270);

  init_wheel(&status.motor.wheel[0], 1, 1);
  init_wheel(&status.motor.wheel[1], 2, -1);

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
}

void init_status(STATUS *status, uint8_t T) {
  status->time = 0;
  status->T = T;

  init_sensor(status);

  init_motor();

  init_device();

  return;
}

void update_status(STATUS *status) {
  status->motor.wheel[0].cur_speed = get_wheel_speed(&status->motor.wheel[0]);
  status->motor.wheel[1].cur_speed = get_wheel_speed(&status->motor.wheel[1]);
  status->motor.wheel[2].cur_speed = get_wheel_speed(&status->motor.wheel[2]);
  status->motor.wheel[3].cur_speed = get_wheel_speed(&status->motor.wheel[3]);

  get_gyr_data(&status->sensor.gy901);

  return;
}

void driver_status(STATUS *status) {
  driver_button(&status->device.button_D2);
  driver_button(&status->device.button_B11);

  driver_LED(&status->device.led_on_board);
  driver_LED(&status->device.led1);
  driver_LED(&status->device.led2);

  driver_servo(&status->motor.servo[0]);
  driver_servo(&status->motor.servo[1]);

  driver_BUZZER(&status->device.buzzer);

  driver_wheel(&status->motor.wheel[0]);  // 执行状态更新
  driver_wheel(&status->motor.wheel[1]);

  return;
}
