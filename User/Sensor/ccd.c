// @63 @551

#include "ccd.h"

#include "array.h"
#include "road.h"

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

short get_adc_val(void) {  // 读取ADC的函数
  HAL_ADC_Start(&hadc3);

  delay_us(2);

  unsigned gADCResult = HAL_ADC_GetValue(&hadc3);

  return gADCResult >> GAIN;
}

void start_ccd(void) {
  CLK_DOWN;
  CLK_DOWN;
  delay_us(20);

  CLK_DOWN;
  SI_UP;
  delay_us(20);

  CLK_UP;  // 警告在使用mspm0：实测CLK和SI引脚不能用同一个PORT，否则编译器会改变执行顺序，将该段delay()放置段尾执行，原因未知
  delay_us(20);
  SI_DOWN;

  delay_us(20);

  return;
}

void get_ccd(uint16_t *BUFF) {     // 实测使用该函数读取轮趣科技CCD时前15个数据近似为0
  for (int i = 0; i < 128; i++) {  // 轮趣文档中说因为GPIO速度过快导致
    CLK_DOWN;                      // 当前没有找到可以更改MSPM0G3507的GPIO速度的方法
    delay_us(1);
    BUFF[i] = get_adc_val();
    CLK_UP;
    delay_us(1);
  }
}

void get_ccd_val(void) {
  start_ccd();
  get_ccd(BUFF_DATA_1);

  return;
}

void ccd_compute() {
  static int last = 0;
  get_ccd_val();
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