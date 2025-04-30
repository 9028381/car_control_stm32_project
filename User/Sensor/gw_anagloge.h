#ifndef __GW_ANALOGUE_H
#define __GW_ANALOGUE_H

#include "stdint.h"

typedef struct GW_ANALOGUE {
  uint8_t channel[8];                 // 0-7
  uint8_t sta;                        // 0工作模式 1校准模式
  uint8_t correction_data_w[8];       // 白色校准数据
  uint8_t correction_data_b[8];       // 黑色校准数据
  uint8_t digital_8bit;               // 8bit数字量
  uint8_t digital_high_threshold[8];  // 8bit高阈值
  uint8_t digital_low_threshold[8];   // 8bit低阈值
  float diff;

} GW_ANALOGUE;

void init_gw_analogue(GW_ANALOGUE *aw_analogue);
void select_channel(uint8_t channel);
void get_gw_angalogue_data(GW_ANALOGUE *aw_analogue);
void correct_gw_analogue(GW_ANALOGUE *gw_analogue);
void get_gw_analoge_digital_data(GW_ANALOGUE *gw_analogue);
void gw_analogue_gray_show(GW_ANALOGUE *gw_analogue);
void get_gw_analogue_analogue_diff(GW_ANALOGUE *gw_analogue);

#endif