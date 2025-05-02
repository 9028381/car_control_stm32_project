// @551

// 警告 使用该库时需要开启I2C中断

#include "gy901.h"

#include "i2c.h"
#include "pid.h"

#define GYR_ADDR 0xa1

void init_gyr(GYR *gyr) {
  gyr->device_addr = GYR_ADDR;
  gyr->data_start_addr = 0x34;
  for (int i = 0; i < 24; i++) {
    gyr->data_buf[i] = 0;
  }
  gyr->gy901_keep_angle_pid = init_pid(50, 0, 0, 50, 500);
  return;
}

void get_gyr_raw_data(I2C_HandleTypeDef *i2c, GYR *gyr) {
  HAL_I2C_Mem_Read(i2c, GYR_ADDR, gyr->data_start_addr, I2C_MEMADD_SIZE_8BIT, gyr->data_buf, 24, 10);

  return;
}

float get_gyr_value(GYR *gyr, enum gyroscope key) {
  uint8_t cnt = (key - gyr->data_start_addr) * 2;
  float value = (short)(((short)gyr->data_buf[cnt + 1] << 8) | gyr->data_buf[cnt]);

  switch (key) {
    case gyr_a_x:
    case gyr_a_y:
    case gyr_a_z:
      return value * 16 * 9.8;
    case gyr_w_x:
    case gyr_w_y:
    case gyr_w_z:
      return value / 2000;
    case gyr_x_roll:
    case gyr_y_pitch:
    case gyr_z_yaw:
      return value * 180 / 32768;
  }
}
