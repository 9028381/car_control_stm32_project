// @551

#include "gy901.h"

#include "i2c.h"

#define GYR_ADDR 0xa1

uint8_t data_buf[2] = {0};

void get_gyr_data(enum gyroscope key) {
  HAL_I2C_Mem_Read_DMA(&hi2c1, GYR_ADDR, key, I2C_MEMADD_SIZE_8BIT, data_buf, 2);

  return;
}

float get_gyr_value(enum gyroscope key) {
  float value = (short)(((short)data_buf[1] << 8) | data_buf[0]);

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
