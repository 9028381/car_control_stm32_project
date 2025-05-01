// @551
#include "gw_find_line.h"

#include "i2c.h"
#include "log.h"
#include "main.h"
#include "pid.h"
#include "road.h"
#include "status.h"
#include "stdbool.h"
#include "usart.h"

#define GW_GRAY_ADDR 0x4C << 1

#define Ping_CMD 0xAA
#define Digital_Output_CMD 0xDD
#define Analog_Output_CMD 0xB0
#define Get_error_CMD 0xDE

#define Ping_SUCCESS 0x66

#define INTEGRAL_TIMES 10

void serve_gw_8bit_road(GW_8BIT *gw_8bit, Road road) {
  if (road == UnknowRoad) {
    return;  // 没认出来的路口直接忽略
  }
  if (gw_8bit->cross == Straight) {
    gw_8bit->cross = road;                                                       // 更新路口状态
  } else {                                                                       // 如果当前是特殊路口就判断是否回到直线
    if (gw_8bit->data_buf & 0b00111100) {                                        // 由特殊路口再次回到直线
      gw_8bit->gw_find_line_pid = init_pid(1.5, 0.5, 1.5, status.state.T, 200);  // 到直线后重新初始化巡线PID
      gw_8bit->cross = Straight;
    }
  }
  return;
}

void init_gw_8bit(GW_8BIT *gw_8bit) {
  gw_8bit->gw_find_line_pid = init_pid(1.5, 0.5, 1.5, status.state.T, 200);

  gw_8bit->gw_bit_weight[0] = 0;
  gw_8bit->gw_bit_weight[1] = -1800;
  gw_8bit->gw_bit_weight[2] = -700;
  gw_8bit->gw_bit_weight[3] = -200;
  gw_8bit->gw_bit_weight[4] = 200;
  gw_8bit->gw_bit_weight[5] = 700;
  gw_8bit->gw_bit_weight[6] = 1800;
  gw_8bit->gw_bit_weight[7] = 0;

  gw_8bit->integral = 0;
  gw_8bit->maybe = 0;
  gw_8bit->cross_cnt = 0;
  gw_8bit->cross = Straight;

  gw_8bit->data_buf = 0;

  gw_8bit->gw_diff = 0;

  return;
}

void corvet_black_is_1(GW_8BIT *gw_8bit) {
  gw_8bit->data_buf = ~gw_8bit->data_buf;
}

void gw_gray_show(GW_8BIT *gw_8bit) {
  uint8_t buf = gw_8bit->data_buf;
  char str[9];
  str[8] = '\0';
  for (int i = 0; i < 8; i++) {
    str[i] = buf & 0x80 ? '#' : '.';
    buf <<= 1;
  }
  PRINTLN("%s", str);
}

enum Road gw_road_new_from_bit(bool L, bool F, bool R) {
  uint8_t left = L ? 0b100 : 0;
  uint8_t font = F ? 0b010 : 0;
  uint8_t right = R ? 0b001 : 0;

  return left | font | right;
}

Road gw_gray_decision(GW_8BIT *gw_8bit) {
  bool left = (gw_8bit->integral >> 6) == 0x03;     // 0b1100_0000
  bool right = (gw_8bit->integral & 0x03) == 0x03;  // 0b0000_0011
  bool font = gw_8bit->data_buf & 0x3C;             // 0b0011_1100
  Road road = gw_road_new_from_bit(left, font, right);
  return road;
}

short gw_gray_diff(GW_8BIT *gw_8bit, uint8_t line) {
  short diff = 0;
  unsigned char cnt = 0;

  for (int i = 0; i < 8; i++) {
    if (((gw_8bit->data_buf >> i) & 0x01)) {
      cnt++;
      diff += gw_8bit->gw_bit_weight[i];
    }
  }
  if (cnt != 0) {
    return diff / cnt;
  } else {
    return 0;
  }
}

int32_t gw_get_line_diff(GW_8BIT *gw_8bit) {
  corvet_black_is_1(gw_8bit);
  // gw_gray_show(gw_8bit);
  if (gw_8bit->data_buf & 0x81) {
    if (gw_8bit->maybe == 0) {
      gw_8bit->maybe = INTEGRAL_TIMES;
    }
  }
  if (gw_8bit->maybe > 1) {
    gw_8bit->integral = gw_8bit->integral | gw_8bit->data_buf;
    gw_8bit->maybe--;
    // log_uprintf(&huart1, "maybe: %d\n", gw_8bit->maybe);
  } else if (gw_8bit->maybe == 1) {
    switch (gw_gray_decision(gw_8bit)) {
      case UnknowRoad:
        log_uprintf(&huart1, "Unknow road\n");
        serve_gw_8bit_road(gw_8bit, UnknowRoad);
        break;
      case CrossRoad:  // 十字路口
        log_uprintf(&huart1, "Cross road\n");
        serve_gw_8bit_road(gw_8bit, CrossRoad);
        break;
      case TBRoad:  // T型路口
        log_uprintf(&huart1, "T B road\n");
        serve_gw_8bit_road(gw_8bit, TBRoad);
        break;
      case TLRoad:  // T型左路口
        log_uprintf(&huart1, "T L road\n");
        serve_gw_8bit_road(gw_8bit, TLRoad);
        break;
      case TRRoad:  // T型右路口
        log_uprintf(&huart1, "T R road\n");
        serve_gw_8bit_road(gw_8bit, TRRoad);
        break;
      case LeftRoad:  // 左路口
        log_uprintf(&huart1, "Left road\n");
        serve_gw_8bit_road(gw_8bit, LeftRoad);
        break;
      case RightRoad:  // 右路口
        log_uprintf(&huart1, "Right road\n");
        serve_gw_8bit_road(gw_8bit, RightRoad);
        break;
      case Straight:  // 直路
        // log_uprintf(&huart1, "Straight road\n");
        serve_gw_8bit_road(gw_8bit, Straight);
        break;
    }
    gw_8bit->maybe = 0;
    gw_8bit->integral = 0;
  }
  if (gw_8bit->cross == Straight) {
    if (gw_8bit->maybe != 0) {
      return compute_pid(&gw_8bit->gw_find_line_pid, gw_gray_diff(gw_8bit, gw_8bit->data_buf & 0b00111100));  // 如果可能有路口就屏蔽掉左右外边的两个传感器
    } else {
      return compute_pid(&gw_8bit->gw_find_line_pid, gw_gray_diff(gw_8bit, gw_8bit->data_buf & 0b01111110));  // 如果可能有路口就屏蔽掉左右外边的两个传感器
    }
  }
  if (gw_8bit->cross == LeftRoad)
    return ROAD_LEFT;
  if (gw_8bit->cross == RightRoad)
    return ROAD_RIGHT;
}

void get_gw_8bit_data(I2C_HandleTypeDef *hi2c, GW_8BIT *gw_8bit) {
  uint8_t cmd = Digital_Output_CMD;
  uint8_t buf = 0;

  HAL_I2C_Mem_Read(hi2c, GW_GRAY_ADDR, cmd, I2C_MEMADD_SIZE_8BIT, &gw_8bit->data_buf, 1, 5);

  return;
}
