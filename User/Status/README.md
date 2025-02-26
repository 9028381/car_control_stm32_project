# 项目状态树部分介绍

## 概述

status 状态树的目的是将小车的所有状态(包括传感器、运动状态、以及 LED、蜂鸣器等设备)封装在一个结构体中。将 status 作为一个全局变量，关于小车的所有参数的获取与设置均通过该结构体进行。

## 使用示例

### 状态设置

```c
// 设置小车一个直流电机的速度
status.motor.wheel[0].tar_speed;

// 开启一个LED灯
status.device.led_1.on = 1;
```

### 设备更新

对每个设备的更新数据与驱动均以单个外设进行，如单个电机、单个舵机、单个 LED：

```c
// 获取陀螺仪的原始数据
get_gyr_data(&status->sensor.gy901);

// 驱动舵机
driver_servo(&status->motor.servo[0]);
```

## 统一接口

对于每个设备的初始化、获取原始数据、驱动(这三个根据外设不同可能不全都需要)提供统一的接口：

```c
// 初始化xxx设备
init_xxx(XXX *xxx)

// 获取xxx的原始数据
update_xxx(XXX *xxx)

// 驱动xxx设备
driver_xxx(XXX *xxx)
```

> 注：这三个函数的具体实现位置请查看每个外设的.h 文件
