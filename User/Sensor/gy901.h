// @551

// 警告 使用该库时需要开启I2C中断

#ifndef __GYROSCOPE_H__
#define __GYROSCOPE_H__

#include "main.h"

typedef struct GYR {
  uint8_t data_buf[24];
  uint8_t device_addr;
  uint8_t data_start_addr;
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

void get_gyr_data(GYR *gyr);                        // 放在status_update()中
float get_gyr_value(GYR *gyr, enum gyroscope key);  // 放在status_driver()中
void init_gyr(GYR *gyr);                            // 放在init_sensor()中

#endif /* !__GYROSCOPE_H__ */
