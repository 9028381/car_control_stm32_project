#ifndef __PARA_H_
#define __PARA_H_

#include "para.h"
#include "stdbool.h"
#include "stdint.h"

typedef enum TYPE {
  FLOAT = 1,
  DOUBLE,
  INT,
  UINT8,
  INT8,
  UINT16,
  INT16,
  UINT32,
  INT32,
  UINT64,
  INT64,
  UNKNOWN,
} TYPE;

/**
 * @brief 将对应地址的值提取出来
 *
 * @param cur_addr 需要提取的具体地址
 * @param type 地址对应的数据类型
 * @return float
 */
float get_cur_val(void* cur_addr, TYPE type);

/**
 * @brief 将目标地址的值转化为一个uint32的二进制数据
 *
 * @param addr 目标变量地址
 * @param type 变量的类型
 * @return uint32_t
 */
uint32_t get_4byte_val(void* addr, TYPE type);
bool set_4byte_val(void* addr, TYPE type, uint32_t value);

#endif
