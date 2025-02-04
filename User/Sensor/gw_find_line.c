// @551
#include "gw_find_line.h"

#include "i2c.h"
#include "log.h"
#include "main.h"
#include "road.h"
#include "stdbool.h"
#include "stm_or_msp.h"
#include "usart.h"

#define GW_GRAY_ADDR 0x4C

#define Ping_CMD 0xAA
#define Digital_Output_CMD 0xDD
#define Analogue_Output_CMD 0xB0
#define Get_error_CMD 0xDE

#define Ping_SUCCESS 0x66

uint8_t data_buf = 0;

const int16_t gw_bit_weight[8] = {0, -1500, -300, -100, 100, 300, 1500, 0};
uint32_t line_record = 0x6666;  // 0b0110_0110_0110_0110初始化路为直线

void update_line_record() {
  line_record = (line_record << 8) | data_buf;

  return;
}

LINE_TYPE get_line_value() {
  update_line_record();  // 更新路线信息

  uint8_t tar = (line_record >> 16) & 0xFF;  // 将第三行的信息提取出来
  if (tar & 0x81)                            // 两端有黑线
  {
    // 开始判断是什么路口
    tar = ((line_record >> 24) & 0xFF) | ((line_record >> 16) & 0xFF) | ((line_record >> 8) & 0xFF) | (line_record >> 8);  // 将四行信息压缩为一行
    if (tar & 0x81 == 0x81)                                                                                                // 可能是十字路口或者丁字路口
    {
      tar = line_record & 0xff;  // 取出最新行信息
      if (tar & 0x18)            // 最新的前面有黑线
      {
        return CROSS;
      } else {
        return T;
      }
    } else if (tar & 0x01 == 0x01)  // 可能是R或者RT
    {
      tar = line_record & 0xff;  // 取出最新行信息
      if (tar & 0x01)            // 最新的前面有黑线
      {
        return RT;
      } else {
        return R;
      }
    } else if (tar & 0x80 == 0x80)  // 可能是L或者LT
    {
      tar = line_record & 0xff;  // 取出最新行信息
      if (tar & 0x80)            // 最新的前面有黑线
      {
        return LT;
      } else {
        return L;
      }
    }
  }
  tar = line_record & 0xff;  // 取出最新行信息
  if (tar & 0x18)            // 最新行最中间的两个有黑线
  {
    return STRAIGHT;
  }

  return NONE;  // 一点不特殊
}

int16_t gw_gray_diff(LINE_TYPE line_type) {
  if (1)  // 这里还没想好?????
  {
    if (line_record & 0x01)  // 新的左边有黑线
    {
      return compute_gw_gray_diff(line_record & 0xfc);  // 把左边的两个屏蔽掉
    }
    if (line_record & 0x80)  // 新的右边有黑线
    {
      return compute_gw_gray_diff(line_record & 0x3f);  // 把右边的两个屏蔽掉
    } else {
      return compute_gw_gray_diff(line_record & 0xff);  // 正常计算
    }
  }
}

int16_t compute_gw_gray_diff(uint8_t gray[8]) {
  int16_t diff = 0;
  uint8_t cnt = 0;
  for (int i = 0; i < 8; i++) {
    if (gray[i] == 1) {
      cnt++;
    }
    diff += gray[i] * gw_bit_weight[i];
  }
  if (cnt != 0)
    return diff / cnt;
  else
    return 0;
}

#ifdef STM32

void gw_gray_get_line_digital_is_black() {
  uint8_t cmd = Digital_Output_CMD;

  HAL_I2C_Master_Receive_DMA(&hi2c1, GW_GRAY_ADDR, &data_buf, 1);

  return;
}

void gw_gray_get_line_analog(uint8_t gray[8]) {
  uint8_t cmd = Analogue_Output_CMD;
  uint8_t buf[8] = {0};

  HAL_I2C_Master_Receive_DMA(&hi2c1, GW_GRAY_ADDR, buf, 8);

  // printf("1:%X 2:%X 3:%X 4:%X 5:%X 6:%X 7:%X 8:%X", buf[0], buf[1], buf[2],
  // buf[3], buf[4], buf[5], buf[6], buf[7]);
}

uint8_t gw_gray_is_ok() {
  uint8_t cmd = Ping_CMD;
  uint8_t buf = 0;

  HAL_I2C_Master_Transmit(&hi2c1, GW_GRAY_ADDR, &cmd, 1, 10);
  HAL_I2C_Master_Receive(&hi2c1, GW_GRAY_ADDR, &buf, 1, 10);

  if (buf == Ping_SUCCESS)
    return 1;
  else
    return 0;
}

uint8_t gw_gray_get_error() {
  uint8_t cmd = Get_error_CMD;
  uint8_t buf = 0;

  HAL_I2C_Master_Receive_DMA(&hi2c1, GW_GRAY_ADDR, &buf, 1);

  return buf;
}

void set_gw_gray_mode(uint8_t cmd) {
  HAL_I2C_Master_Transmit_DMA(&hi2c1, GW_GRAY_ADDR, &cmd, 1);
}

#endif
