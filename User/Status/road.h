#ifndef __ROAD_H__
#define __ROAD_H__

#include "stdint.h"

#define ROAD_CROSS -30000
#define ROAD_TB 30000
#define ROAD_TL -25000
#define ROAD_TR 25000
#define ROAD_LEFT -20000
#define ROAD_RIGHT 20000

typedef enum Road {    // L F R
  CrossRoad = 0b111,   // 1 1 1
  TBRoad = 0b101,      // 1 0 1
  TLRoad = 0b011,      // 1 1 0
  TRRoad = 0b110,      // 0 1 1
  LeftRoad = 0b001,    // 1 0 0
  RightRoad = 0b100,   // 0 0 1
  Straight = 0b010,    // 0 1 0
  UnknowRoad = 0b000,  // 0 0 0
} Road;

typedef struct RoadDetermine {
  uint8_t data_buf;
  uint8_t integral;
  uint8_t maybe;
  uint8_t cross_cnt;
  Road cross;
  uint8_t integral_times;

} RoadDetermine;

// 对uint8_t数据进行路口判断
// 传入参数: 巡线传感器的0-7通道数字量
// 判断到有路口时进入
void get_road_type(RoadDetermine *roaddetermine, uint8_t road_data);

#endif
