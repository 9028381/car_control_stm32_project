// @63 @551

#include "ccd.h"

#include "array.h"
#include "road.h"
#include "math_tool.h"

uint16_t BUFF_DATA_1[128] = {0};
uint16_t BUFF_DATA_2[128] = {0};
int16_t ccd_diff = 0;

#define GAIN 4
#define MASK 0  // 遮住一部分的数据，防止前15个数字读不出来的问题
#define CCD_ARRAY &BUFF_DATA_1[MASK]
#define CCD_ARRAY_LEN (128 - 2 * MASK)
#define CCD_BLACK_THRUST 300
#define CCD_COUNT_THRUST 5
#define CCD_BLACK_COUNT_LIMIT 40

void get_ccd_data() {
  HAL_TIM_Base_Start_IT(&htim6);
}

void driver_ccd() {
  static uint16_t cnt = 0;
  if (cnt == 1) {
    CLK_DOWN;
    SI_DOWN;
    cnt++;
    return;
  } else if (cnt == 5) {
    CLK_UP;
    SI_UP;
    cnt++;
    return;
  } else if (cnt == 7) {
    CLK_UP;
    SI_DOWN;
    cnt++;
    return;
  } else if (cnt == 9) {
    CLK_DOWN;
    SI_DOWN;
    cnt++;
    return;
  } else if ((cnt > 9) && ((cnt - 10) % 3 == 0) && (cnt < 521)) {
    HAL_ADC_Start_DMA(&hadc3, &BUFF_DATA_1[(cnt - 10) / 3], 1);
    cnt++;
    return;
  } else if ((cnt > 9) && ((cnt - 10) % 3 == 1) && (cnt < 521)) {
    CLK_UP;
    SI_DOWN;
    cnt++;
    return;
  } else if ((cnt > 9) && ((cnt - 10) % 3 == 2) && (cnt < 521)) {
    CLK_DOWN;
    SI_UP;
    cnt++;
    return;
  } else if (cnt == 521) {
    CLK_DOWN;
    SI_DOWN;
    cnt = 0;
    HAL_TIM_Base_Stop_IT(&htim6);
  } else {
    cnt++;
    return;
  }
}

int16_t ccd_compute() {
  static int last = 0;

  short dest[128];
  /* int len = convolve_unit(CCD_ARRAY_LEN, CCD_KERNEL_LEN, CCD_ARRAY, dest); */
  /* int len = forward_difference_multiple(128 - 15, 6, &CCD_DATA[15], dest); */
  struct SumAndCount sum_count =
      array_mean_index_less_than(CCD_ARRAY_LEN, CCD_ARRAY, CCD_BLACK_THRUST);
  /* array_display(len, dest); */

  if (sum_count.count < CCD_COUNT_THRUST) {
    /* INFO("CCD not found black."); */
    if (ABS(last) < CCD_BLACK_COUNT_LIMIT)
      return NONE;
    return last;
  }

  int diff = sum_count.sum / sum_count.count - CCD_ARRAY_LEN / 2;
  diff = -diff;
  /* INFO("CCD fond black: %d", diff); */

  last = diff;

  ccd_diff = diff;
}

int16_t get_ccd_diff() {
  return ccd_diff;
}