#include "ccd.h"

short CCD_DATA[128] = {0};

#define GAIN 4
#define CCD_ARRAY &CCD_DATA[15]
#define CCD_ARRAY_LEN (128 - 15 * 2)
#define CCD_BLACK_THRUST 300
#define CCD_COUNT_THRUST 5
#define CCD_BLACK_COUNT_LIMIT 40

short get_adc_val(void) { // 读取ADC的函数
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

  CLK_UP; // 警告在使用mspm0：实测CLK和SI引脚不能用同一个PORT，否则编译器会改变执行顺序，将该段delay()放置段尾执行，原因未知
  delay_us(20); 
  SI_DOWN;

  delay_us(20);

  return;
}

void get_ccd(void) {                // 实测使用该函数读取轮趣科技CCD时前15个数据近似为0
  for (int i = 0; i < 128; i++) {   // 轮趣文档中说因为GPIO速度过快导致
    CLK_DOWN;                       // 当前没有找到可以更改MSPM0G3507的GPIO速度的方法
    delay_us(1);
    CCD_DATA[i] = get_adc_val();
    CLK_UP;
    delay_us(1);
  }
}

void get_ccd_val(void) {
  start_ccd();
  get_ccd();

  return;
}

