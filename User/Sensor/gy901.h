// @551

// 警告 使用该库时需要开启I2C中断

#ifndef __GYROSCOPE_H__
#define __GYROSCOPE_H__

#include "main.h"
#include "pid.h"

// GYR结构体
// 挂载于status sensor
// 用于驱动gy901
typedef struct GYR {
  uint8_t data_buf[24];      // 读取数据暂存
  uint8_t device_addr;       // 设备iic地址 默认0xa1
  uint8_t data_start_addr;   // gy901数据寄存器起始地址 默认0x34
  PID gy901_keep_angle_pid;  // 陀螺仪保持角度PID
} GYR;

enum gyroscope {
  gyr_a_x = 0x34,      // Acceleration of the sensor along the x-axis
  gyr_a_y = 0x35,      // Acceleration of the sensor along the y-axis
  gyr_a_z = 0x36,      // Acceleration of the sensor along the z-axis
  gyr_w_x = 0x37,      // The angular velocity of the sensor around the x-axis
  gyr_w_y = 0x38,      // The angular velocity of the sensor around the y-axis
  gyr_w_z = 0x39,      // The angular velocity of the sensor around the z-axis
  gyr_x_roll = 0x3D,   // The angle of the sensor around the x-axis
  gyr_y_pitch = 0x3E,  // The angle of the sensor around the y-axis
  gyr_z_yaw = 0x3F,    // The angle of the sensor around the z-axis
};

// 读取gy901的原始数据 放在status_update()中
void get_gyr_raw_data(I2C_HandleTypeDef *i2c, GYR *gyr);
// 将原始数据转化为实际物理量 key传入参数枚举 gyroscope见上
float get_gyr_value(GYR *gyr, enum gyroscope key);
// 初始化gyr 放在init_sensor()中
void init_gyr(GYR *gyr);

#endif /* !__GYROSCOPE_H__ */
