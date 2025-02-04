// @551

#include "gy901.h"

#include "i2c.h"

#define GYR_ADDR 0xa1

float Get_gyr_value(enum gyroscope key) {
  uint8_t buf[2];

  HAL_I2C_Mem_Read_DMA(&hi2c1, GYR_ADDR, key, I2C_MEMADD_SIZE_8BIT, buf, 2);
  float value = (short)(((short)buf[1] << 8) | buf[0]);

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
