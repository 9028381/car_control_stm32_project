#include "eeprom.h"

#include "i2c.h"
#include "log.h"

uint8_t buff[PAGE_SIZE] = {0};  // 缓冲区

/**
 * @brief  判断AT24C02是否空闲
 * @retval 1: 空闲, 0: 忙碌
 */
uint8_t EEPROM_IsReady(void) {
  HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_ADDRESS, 1, 10);
  return (status == HAL_OK) ? 1 : 0;
}

/**
 * @brief  阻塞直到AT24C02空闲
 */
void EEPROM_WaitUntilReady(void) {
  while (!EEPROM_IsReady()) {
    continue;
  }
}

/**
 * @brief  写入一个字节到AT24C02
 * @param  tar_addr: 目标地址
 * @param  data: 数据
 * @param  size: 数据长度
 */
void EEPROM_WriteByte(uint16_t tar_addr, uint8_t *data, uint16_t size) {
  uint16_t cnt = size;                  // 记录还要写入的字节数
  uint16_t cur_page = tar_addr / size;  // 当前页
  uint16_t cur_addr = tar_addr;         // 当前地址
  uint16_t len = 0;                     // 当前页剩余空间
  while (cnt != 0) {
    if (cur_page >= PAGE_NUMB) {
      WARN("EEPROM_WriteByte: Out of range\n");
    }
    if ((tar_addr + cnt) / PAGE_SIZE != cur_page) {  // 如果需要跨页
      len = PAGE_SIZE - cur_addr % PAGE_SIZE;        // 当前页剩余空间
      EEPROM_WaitUntilReady();
      HAL_I2C_Mem_Write_DMA(&hi2c1, EEPROM_ADDRESS, cur_addr, EEPROM_MEMADD_SIZE, data, len);
      cnt = cnt - len;            // 更新剩余字节数
      cur_addr = cur_addr + len;  // 更新当前地址
      cur_page = cur_addr + 1;    // 更新当前页
    } else {
      EEPROM_WaitUntilReady();
      HAL_I2C_Mem_Write_DMA(&hi2c1, EEPROM_ADDRESS, cur_addr, EEPROM_MEMADD_SIZE, data, cnt);
      cnt = 0;  // 更新剩余字节数
    }
  }

  return;
}

/**
 * @brief  从AT24C02读取一个字节
 * @param  tar_addr: 目标地址
 * @param  data: 数据
 * @param  size: 数据长度
 */
void EEPROM_ReadByte(uint16_t tar_addr, uint8_t *data, uint16_t size) {
  EEPROM_WaitUntilReady();
  HAL_I2C_Mem_Read_DMA(&hi2c1, EEPROM_ADDRESS, tar_addr, EEPROM_MEMADD_SIZE, data, size);
}