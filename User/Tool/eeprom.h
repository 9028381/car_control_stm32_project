// @551

#ifndef __EEPROM_H__
#define __EEPROM_H__

#define EEPROM_ADDRESS 0xA0  // EEPROM地址
#define PAGE_SIZE 128        // 页大小
#define PAGE_NUMB 16         // 页数
#define ADDR_SIZE_16BIT      // 地址长度

#ifdef ADDR_SIZE_8BIT
#define EEPROM_MEMADD_SIZE I2C_MEMADD_SIZE_8BIT
#endif

#ifdef ADDR_SIZE_16BIT
#define EEPROM_MEMADD_SIZE I2C_MEMADD_SIZE_16BIT
#endif

#include "main.h"

void EEPROM_WriteByte(uint16_t tar_addr, uint8_t *data, uint16_t size);  // 向eeprom写入数据
void EEPROM_ReadByte(uint16_t tar_addr, uint8_t *data, uint16_t size);   // 从eeprom读取数据

#endif
