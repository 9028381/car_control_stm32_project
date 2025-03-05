// @551
#include "gw_find_line.h"

#include "i2c.h"
#include "log.h"
#include "main.h"
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

#define GW_GRAY_CROSSROAD_MIN_RETURN_TIMES 20
#define INTEGRAL_TIMES 10

void init_gw_8bit(GW_8BIT *gw_8bit) {
  gw_8bit->gw_bit_weight[0] = 0;
  gw_8bit->gw_bit_weight[1] = -1500;
  gw_8bit->gw_bit_weight[2] = -500;
  gw_8bit->gw_bit_weight[3] = -200;
  gw_8bit->gw_bit_weight[4] = 200;
  gw_8bit->gw_bit_weight[5] = 500;
  gw_8bit->gw_bit_weight[6] = 1500;
  gw_8bit->gw_bit_weight[7] = 0;

  gw_8bit->integral = 0;
  gw_8bit->maybe = 0;
  gw_8bit->cross_cnt = 0;
  gw_8bit->cross = Straight;

  gw_8bit->data_buf = 0;

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

enum Road road_new_from_bit(bool L, bool F, bool R) {
  uint8_t left = L ? 0b100 : 0;
  uint8_t font = F ? 0b010 : 0;
  uint8_t right = R ? 0b001 : 0;

  return left | font | right;
}

void gw_gray_decision(GW_8BIT *gw_8bit, uint8_t integral, uint8_t line) {
  bool left = (integral >> 6) == 0x03;     // 0b1100_0000
  bool right = (integral & 0x03) == 0x03;  // 0b0000_0011
  bool font = line & 0x3C;                 // 0b0011_1100
  enum Road road = road_new_from_bit(left, font, right);
  gw_8bit->cross = road;
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

short get_line_value(GW_8BIT *gw_8bit) {
  corvet_black_is_1(gw_8bit);
  // gw_gray_show(gw_8bit);
  if (gw_8bit->maybe) {
    if (gw_8bit->maybe == 1) {
      if (gw_8bit->cross == Straight) {
        gw_gray_decision(gw_8bit, gw_8bit->integral, gw_8bit->data_buf);
      }
      switch (gw_8bit->cross) {
        case UnknowRoad:
          log_uprintf(&huart1, "Unknow road\n");
          gw_8bit->cross = Straight;
          gw_8bit->maybe = 0;
          return 0;
        case CrossRoad:
          log_uprintf(&huart1, "Cross road\n");
          if (gw_8bit->cross_cnt == 0)
            gw_8bit->cross_cnt = GW_GRAY_CROSSROAD_MIN_RETURN_TIMES;

          if (gw_8bit->cross_cnt >= 2) {
            gw_8bit->cross_cnt -= 1;
            return ROAD_CROSS;
          }

          if (gw_8bit->data_buf & 0b00111100) {
            gw_8bit->cross = Straight;
            gw_8bit->maybe = 0;
            gw_8bit->cross_cnt = 0;
            return gw_gray_diff(gw_8bit, gw_8bit->data_buf & 0x7E);
          }

          return ROAD_CROSS;
        case TBRoad:
          log_uprintf(&huart1, "T B road\n");
          if (gw_8bit->data_buf & 0b00111100) {
            gw_8bit->cross = Straight;
            gw_8bit->maybe = 0;
            return gw_gray_diff(gw_8bit, gw_8bit->data_buf & 0x7E);
          }
          return ROAD_TB;
        case TLRoad:
          log_uprintf(&huart1, "T L road\n");
          gw_8bit->cross = Straight;
          gw_8bit->maybe = 0;
          return ROAD_TL;
        case TRRoad:
          log_uprintf(&huart1, "T R road\n");
          gw_8bit->cross = Straight;
          gw_8bit->maybe = 0;
          return ROAD_TR;
        case LeftRoad:
          log_uprintf(&huart1, "Left road\n");
          if (gw_8bit->data_buf & 0b00111100) {
            gw_8bit->cross = Straight;
            gw_8bit->maybe = 0;
            return gw_gray_diff(gw_8bit, gw_8bit->data_buf & 0x7E);
          }
          return ROAD_LEFT;
        case RightRoad:
          log_uprintf(&huart1, "Right road\n");
          if (gw_8bit->data_buf & 0b00111100) {
            gw_8bit->cross = Straight;
            gw_8bit->maybe = 0;
            return gw_gray_diff(gw_8bit, gw_8bit->data_buf & 0x7E);
          }
          return ROAD_RIGHT;
        case Straight:
          log_uprintf(&huart1, "Straight road\n");
          gw_8bit->maybe = 0;
          return gw_gray_diff(gw_8bit, gw_8bit->data_buf & 0x7E);
      }
    }

    gw_8bit->integral = gw_8bit->integral | gw_8bit->data_buf;
    gw_8bit->maybe--;
  } else if (gw_8bit->data_buf & 0x81) {
    gw_8bit->maybe = INTEGRAL_TIMES;
    gw_8bit->integral = 0;
  }

  return gw_gray_diff(gw_8bit, gw_8bit->data_buf & 0x7E);  // 0b0111_1110
}

void get_gw_8bit_data(I2C_HandleTypeDef *hi2c, GW_8BIT *gw_8bit) {
  uint8_t cmd = Digital_Output_CMD;
  uint8_t buf = 0;

  HAL_I2C_Mem_Read_DMA(hi2c, GW_GRAY_ADDR, cmd, I2C_MEMADD_SIZE_8BIT, &gw_8bit->data_buf, 1);

  return;
}
