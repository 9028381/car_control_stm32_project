#ifndef __ROAD_H__
#define __ROAD_H__

typedef enum LINE_TYPE {
  CROSS,     // 十字路口
  T,         // T型路口
  R,         // 右拐
  L,         // 左拐
  RT,        // 右T型路口
  LT,        // 左T型路口
  STRAIGHT,  // 直行
  NONE,      // 不是特殊路口
} LINE_TYPE;

#endif
