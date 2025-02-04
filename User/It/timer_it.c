#include "ccd.h"
#include "tim.h"

void HAL_TIM_periodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == &htim5)  // 周期 1ms
  {
  }
  if (htim->Instance == &htim6) {  // 周期 5us
    driver_ccd();
  }
}