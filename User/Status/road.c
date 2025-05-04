#include "road.h"

#include "log.h"
#include "status.h"
#include "stdbool.h"
#include "stdint.h"
#include "usart.h"

uint8_t cross_cnt = 0;  // 路口计数器
uint8_t left_cnt = 0;
extern int32_t rw_time_cur;      // 临时使用的时间变量
extern int32_t rw_time_tar;      // 临时使用的时间变量
extern uint8_t cross_delay;      // 路口延时计数器
extern uint8_t speed_show_flag;  // 显示速度标志位

void init_road_determine(RoadDetermine *roaddetermine) {
  roaddetermine->integral = 0;
  roaddetermine->data_buf = 0;
  roaddetermine->cross = Straight;
  roaddetermine->cross_cnt = 0;

  return;
}

enum Road road_new_from_bit(bool L, bool F, bool R) {
  uint8_t left = L ? 0b100 : 0;
  uint8_t font = F ? 0b010 : 0;
  uint8_t right = R ? 0b001 : 0;

  return left | font | right;
}

Road road_decision(RoadDetermine *roaddetermine) {
  bool left = (roaddetermine->integral >> 6) == 0x03;     // 0b1100_0000
  bool right = (roaddetermine->integral & 0x03) == 0x03;  // 0b0000_0011
  bool font = roaddetermine->data_buf & 0x3C;             // 0b0011_1100
  Road road = road_new_from_bit(left, font, right);
  return road;
}

void serve_road(RoadDetermine *roaddetermine, Road road) {
  if (status.state.motion == FIND_LINE) {
    if (road == CrossRoad) {
      if (cross_cnt < 3) {
        rw_time_cur = status.state.time;
      }
      if (cross_cnt == 3) {
        cross_delay = 55;
      }
      status.state.road_determine.cross = CrossRoad;
      status.state.base_speed = 0;
      cross_cnt++;
    }
    if (road == LeftRoad) {
      left_cnt++;
      status.state.road_determine.cross = LeftRoad;
      if (left_cnt == 1) {
        speed_show_flag = 1;
      }
    }
    if (road == RightRoad) {
      status.state.road_determine.cross = RightRoad;
    }
    if (road == Straight) {
      if (cross_cnt == 4) {
        rw_time_tar = status.state.time;
      }
      status.state.road_determine.cross = Straight;
      status.state.status_pid.follow_line_pid = init_pid(1.5, 0.1, 500, 20, 20);
    }
  }
}

void get_road_type(RoadDetermine *roaddetermine, uint8_t road_data) {
  roaddetermine->data_buf = road_data;  // 更新路口数据
  if (roaddetermine->cross == Straight) {
    if ((roaddetermine->data_buf & 0x81)) {
      if (roaddetermine->maybe == 0) {
        roaddetermine->maybe = roaddetermine->integral_times;
        roaddetermine->integral = roaddetermine->integral | roaddetermine->data_buf;
      }
    }
  } else if ((status.sensor.gw_analogue.digital_8bit & 0x7E) && (cross_delay == 0)) {
    log_uprintf(&huart1, "Straight road\n");
    serve_road(roaddetermine, Straight);
  }
  if (roaddetermine->maybe > 1) {
    roaddetermine->integral = roaddetermine->integral | roaddetermine->data_buf;
    roaddetermine->maybe--;
  } else if (roaddetermine->maybe == 1) {
    switch (road_decision(roaddetermine)) {
      case UnknowRoad:
        log_uprintf(&huart1, "Unknow road\n");
        serve_road(roaddetermine, UnknowRoad);
        break;
      case CrossRoad:  // 十字路口
        log_uprintf(&huart1, "Cross road\n");
        serve_road(roaddetermine, CrossRoad);
        break;
      case TBRoad:  // T型路口
        log_uprintf(&huart1, "T B road\n");
        serve_road(roaddetermine, TBRoad);
        break;
      case TLRoad:  // T型左路口
        log_uprintf(&huart1, "T L road\n");
        serve_road(roaddetermine, TLRoad);
        break;
      case TRRoad:  // T型右路口
        log_uprintf(&huart1, "T R road\n");
        serve_road(roaddetermine, TRRoad);
        break;
      case LeftRoad:  // 左路口
        log_uprintf(&huart1, "Left road\n");
        serve_road(roaddetermine, LeftRoad);
        break;
      case RightRoad:  // 右路口
        log_uprintf(&huart1, "Right road\n");
        serve_road(roaddetermine, RightRoad);
        break;
      case Straight:  // 直路
        // log_uprintf(&huart1, "Straight road\n");
        serve_road(roaddetermine, Straight);
        break;
    }
    roaddetermine->maybe = 0;
    roaddetermine->integral = 0;
  }
}