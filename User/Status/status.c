// @63 @551

/*
项目状态树介绍
status状态树的目的是将小车的所有状态(包括传感器、运动状态、以及led、蜂鸣器等设备)封装在一个结构体中
将status作为一个全局变量，关于小车的所有参数的获取于设置均通过该结构体进行
例如
  设置小车一个直流电机的速度： status.motor.wheel[0].tar_speed;
  开启一个LED灯： status.device.led_1.on = 1;
对每个设备的更新数据于驱动均以单个外设进行，如单个电机、单个舵机、单个led
例如
  获取陀螺仪的原始数据 get_gyr_data(&status->sensor.gy901);
  驱动status.motor.servo[0] driver_servo(&status->motor.servo[0]);
对于每个设备的初始化、获取原始数据、驱动(这三个根据外设不同可能不全都需要)提供统一的接口
例如
  init_xxx(XXX *xxx) 初始化xxx设备
  update_xxx(XXX *xxx) 获取xxx的原始数据
  driver_xxx(XXX *xxx) 驱动xxx设备
这三个函数具体放在哪里请看每个外设的.h文件
*/

#include "status.h"

#include "button.h"
#include "buzzer.h"
#include "led.h"
#include "log.h"
#include "motion.h"
#include "servo.h"
#include "wheel.h"

STATUS status;

void init_motor() {  // 电机初始化
  init_servo(&status.motor.servo[0], 1, 180);
  init_servo(&status.motor.servo[1], 2, 270);

  init_wheel(&status.motor.wheel[0], 1, -1);
  init_wheel(&status.motor.wheel[1], 2, 1);

  return;
}

void init_device() {  // 设备初始化
  init_button(&status.device.button_D2, 1, 0);
  init_button(&status.device.button_B11, 2, 0);
  init_LED(&status.device.led_on_board, 1, 1);
  init_LED(&status.device.led1, 2, 1);
  init_LED(&status.device.led2, 3, 1);
  init_BUZZER(&status.device.buzzer, 1, 1);

  return;
}

void init_sensor(STATUS *status) {  // 传感器初始化
  init_gyr(&status->sensor.gy901);
}

void init_state(STATUS *status, uint8_t T)  // 状态初始化
{
  status->state.T = T;
  status->state.time = 0;
  status->state.motion = STOP;
  status->state.initial_angle = 0;
  status->state.cur_angle = 0;

  return;
}

void init_status(STATUS *status, uint8_t T) {  // 状态树初始化

  init_state(status, T);

  init_sensor(status);

  init_motor();

  init_device();

  return;
}

void update_status(STATUS *status) {  // 状态树更新数据
  status->motor.wheel[0].cur_speed = get_wheel_speed(&status->motor.wheel[0]);
  status->motor.wheel[1].cur_speed = get_wheel_speed(&status->motor.wheel[1]);
  status->motor.wheel[2].cur_speed = get_wheel_speed(&status->motor.wheel[2]);
  status->motor.wheel[3].cur_speed = get_wheel_speed(&status->motor.wheel[3]);

  get_gyr_data(&status->sensor.gy901);

  return;
}

void driver_status(STATUS *status) {  // 状态数驱动
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
