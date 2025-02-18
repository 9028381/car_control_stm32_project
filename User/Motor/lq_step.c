#include "lq_step.h"
#include "math_tool.h"

uint8_t BCC(uint8_t *data, uint16_t length) {
  uint8_t i;
  uint8_t bcc = 0;  // Initial value
  while (length--) {
    bcc ^= *data++;
  }
  return bcc;
}

void set_lq_step_abslute_angle(UART_HandleTypeDef huart, float angle) {
  uint8_t cmd[11] = {0};
  cmd[0] = 0x7B;
  cmd[1] = 0x01;
  cmd[2] = 0x04;
  cmd[3] = 0x00;
  cmd[4] = 0x20;
  int16_t turn_angle = (uint16_t)(angle * 10);
  turn_angle = CONFINE(turn_angle, 0, 3600);
  cmd[5] = ((turn_angle >> 8) & 0x00FF);
  cmd[6] = (turn_angle & 0x00FF);
  cmd[7] = 0xff;
  cmd[8] = 0x00;
  cmd[9] = BCC(cmd, 9);
  cmd[10] = 0x7D;
  HAL_UART_Transmit_DMA(&huart, cmd, 11);

  return;
}

void trun_lq_step_angle(UART_HandleTypeDef huart, float angle, uint8_t dir) {
  uint8_t cmd[11] = {0};
  cmd[0] = 0x7B;
  cmd[1] = 0x01;
  cmd[2] = 0x02;
  cmd[3] = dir;  // 0x00: 逆时针, 0x01: 顺时针
  cmd[4] = 0x20;
  int16_t turn_angle = (uint16_t)(angle * 10);
  cmd[5] = ((turn_angle >> 8) & 0x00FF);
  cmd[6] = (turn_angle & 0x00FF);
  cmd[7] = 0xff;
  cmd[8] = 0x00;
  cmd[9] = BCC(cmd, 9);
  cmd[10] = 0x7D;
  HAL_UART_Transmit_DMA(&huart, cmd, 11);

  return;
}

void trun_lq_step_speed(UART_HandleTypeDef huart, float speed, uint8_t dir)  // speed 单位rad/s
{
  uint8_t cmd[11] = {0};
  cmd[0] = 0x7B;
  cmd[1] = 0x01;
  cmd[2] = 0x01;
  cmd[3] = dir;  // 0x00: 逆时针, 0x01: 顺时针
  cmd[4] = 0x20;
  cmd[5] = 0;
  cmd[6] = 0;
  int16_t turn_speed = (uint16_t)(speed * 10);
  cmd[7] = (turn_speed >> 8) & 0x00FF;
  cmd[8] = turn_speed & 0x00FF;
  cmd[9] = BCC(cmd, 9);
  cmd[10] = 0x7D;
  HAL_UART_Transmit_DMA(&huart, cmd, 11);

  return;
}

void turn_lq_step_current(UART_HandleTypeDef huart, uint16_t current, uint8_t dir)  // current 电流单位mA
{
  uint8_t cmd[11] = {0};
  cmd[0] = 0x7B;
  cmd[1] = 0x01;
  cmd[2] = 0x03;
  cmd[3] = dir;  // 0x00: 逆时针, 0x01: 顺时针
  cmd[4] = 0x20;
  cmd[5] = (current >> 8) & 0x00FF;
  cmd[6] = current & 0x00FF;
  cmd[7] = 0xff;
  cmd[8] = 0xff;
  cmd[9] = BCC(cmd, 9);
  cmd[10] = 0x7D;
  HAL_UART_Transmit_DMA(&huart, cmd, 11);

  return;
}