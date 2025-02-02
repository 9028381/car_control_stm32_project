#include "abslute_angle_sensor.h"

uint8_t buff[20] = {0};  // 接收暂存
float cur_angle = 0;     // 当前角度
float set_angle = 0;     // 设定角度

uint16_t Crc_Count(unsigned char pbuf[], unsigned char num) {
  int i, j;
  unsigned int wcrc = 0xffff;
  for (i = 0; i < num; i++) {
    wcrc ^= (unsigned int)(pbuf[i]);
    for (j = 0; j < 8; j++) {
      if (wcrc & 0x0001) {
        wcrc >>= 1;
        wcrc ^= 0xa001;
      } else
        wcrc >>= 1;
    }
  }
  return wcrc;
}

// 写单个寄存器
void write_abslute_angle_reg(UART_HandleTypeDef huart, uint16_t addr, uint16_t value) {
  uint8_t data[8] = {0};
  data[0] = 0x01;
  data[1] = 0x06;
  data[2] = (addr >> 8) & 0x00FF;
  data[3] = addr & 0x00FF;
  data[4] = (value >> 8) & 0x00FF;
  data[5] = value & 0x00FF;
  uint16_t crc = Crc_Count(data, 6);
  data[6] = (crc >> 8) & 0x00FF;
  data[7] = crc & 0x00FF;

  return;
}

// 读单个寄存器
void read_abslute_angle_reg(UART_HandleTypeDef huart, uint16_t addr, uint16_t *value) {
  uint8_t data[8] = {0};
  data[0] = 0x01;
  data[1] = 0x03;
  data[2] = (addr >> 8) & 0x00FF;
  data[3] = addr & 0x00FF;
  data[4] = 0x00;
  data[5] = 0x01;
  uint16_t crc = Crc_Count(data, 4);
  data[6] = (crc >> 8) & 0x00FF;
  data[7] = crc & 0x00FF;

  HAL_UART_Receive_IT(&huart, buff, 7);

  return;
}

void set_abslute_angle(UART_HandleTypeDef huart, float angle) {
  uint16_t cnt = (uint16_t)(angle / 360 * MAX_CNT);
  uint16_t addr = 0x000B;
  write_abslute_angle_reg(huart, addr, cnt);

  return;
}

// 设置绝对值编码器的模式 0x00: 被动模式 0x01: 主动模式
void set_abslute_angle_sensor_mode(UART_HandleTypeDef huart, uint8_t mode) {
  uint16_t addr = 0x0006;

  write_abslute_angle_reg(huart, addr, mode);

  return;
}

// driver函数放在对应的huart中断函数中
float driver_abslute_angle(UART_HandleTypeDef huart) {
  if (buff[2] == 0x03) {
    cur_angle = (float)((buff[3] << 8) + buff[4]) / (float)MAX_CNT * 360.0;
  } else if (buff[2] == 0x06) {
    set_angle = (float)((buff[4] << 8) + buff[5]) / (float)MAX_CNT * 360.0;
  }
  HAL_UART_Receive_IT(&huart, buff, 7);

  return;
}

float get_abslute_angle() {
  return cur_angle;
}

float get_set_angle() {
  return set_angle;
}
