#ifndef __TASK_H_
#define __TASK_H_

#include <stdbool.h>
#include <stdint.h>

#include "para.h"

#define MAX_TASK_NUM 40

typedef struct task_cell {
  void (*task)(void* para);             // 将要执行的任务函数
  bool (*judge)(float cur, float tar);  // 判断何时执行任务的判断函数
  void* para;                           // 任务函数传递参数
  float cur;                            // 判断条件的当前值
  float tar;                            // 判断条件的目标值
  TYPE type;                            // 表示要判断变量的类型 方便空指针解析
  void* cur_addr;                       // 判断条件的当前值地址
  int32_t timeout;                      // 任务超时时间
} task_cell;

typedef struct TASK {
  uint32_t task_num;             // 记录当前剩余任务数量
  bool waiting[MAX_TASK_NUM];    // 记录每个cell中的任务是否在等待执行 True等待执行 False已经执行
  task_cell cell[MAX_TASK_NUM];  // 存放任务存放单元
  uint64_t task_clock;           // 任务时钟 用于超时检测
} TASK;

/**
 * @brief 用于向任务池里新建一个任务
 *
 * @param ctrl 目标任务控制器
 * @param task 任务函数
 * @param para 任务函数参数
 * @param judge 判断条件函数 构建该 bool函数时 传入两个float变量 前一个为当前值 后一个为目标值
 * @param cur_addr 条件当前值的地址
 * @param offset 条件目标值与当前值的偏移量
 * @param type 要判断的条件值的变量类型
 * @param timeout 超时时间
 * @return true 新建任务成功
 * @return false 新建任务失败
 */
bool add_task(TASK* ctrl, void (*task)(void* para), void* para, bool(*judge), void* cur_addr, float offset, TYPE type, uint32_t timeout);

/**
 * @brief 驱动任务控制器
 *
 * @param ctrl 传入任务控制器
 */
void driver_task(TASK* ctrl);

/**
 * @brief 初始化任务控制器
 *
 * @param ctrl 传入任务控制器
 */
void init_TASK(TASK* ctrl);

/**
 * @brief 更新任务控制器时钟
 *
 * @param ctrl 传入任务控制器
 * @param system_time 传入系统时间
 */
void update_task_clock(TASK* ctrl, uint64_t system_time);

bool Greater_or_Equal(float cur, float tar);
bool Less_or_Equal(float cur, float tar);
bool Error_less_1(float cur, float tar);
bool Error_less_10(float cur, float tar);
bool Error_less_100(float cur, float tar);
bool Error_less_1000(float cur, float tar);
uint32_t add_task_with_time_line(TASK* ctrl, void (*task)(void* para), void* para, uint32_t later);

#endif
