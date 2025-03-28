#include "wheel.h"

#include "gpio.h"
#include "log.h"
#include "math_tool.h"
#include "status.h"
#include "tim.h"

int16_t get_wheel_speed(WHEEL *wheel) {
  int16_t speed = 0;
  if (wheel->which == 1) {
    speed = TIM1->CNT - 30000;
    TIM1->CNT = 30000;
  } else if (wheel->which == 2) {
    speed = TIM2->CNT - 30000;
    TIM2->CNT = 30000;
  } else if (wheel->which == 3) {
    speed = TIM3->CNT - 30000;
    TIM3->CNT = 30000;
  } else if (wheel->which == 4) {
    speed = TIM4->CNT - 30000;
    TIM4->CNT = 30000;
  }

  return speed * wheel->dir;
}

void set_wheel_dir(WHEEL *wheel, int16_t trust) {
  if (wheel->which == 1) {
    if (wheel->trust * wheel->dir > 0) {
      HAL_GPIO_WritePin(M1D1_GPIO_Port, M1D1_Pin, 1);
      HAL_GPIO_WritePin(M1D2_GPIO_Port, M1D2_Pin, 0);
    } else {
      HAL_GPIO_WritePin(M1D1_GPIO_Port, M1D1_Pin, 0);
      HAL_GPIO_WritePin(M1D2_GPIO_Port, M1D2_Pin, 1);
    }
  } else if (wheel->which == 2) {
    if (wheel->trust * wheel->dir < 0) {
      HAL_GPIO_WritePin(M2D1_GPIO_Port, M2D1_Pin, 1);
      HAL_GPIO_WritePin(M2D2_GPIO_Port, M2D2_Pin, 0);
    } else {
      HAL_GPIO_WritePin(M2D1_GPIO_Port, M2D1_Pin, 0);
      HAL_GPIO_WritePin(M2D2_GPIO_Port, M2D2_Pin, 1);
    }
  } else if (wheel->which == 3) {
    if (wheel->trust * wheel->dir > 0) {
      HAL_GPIO_WritePin(M3D1_GPIO_Port, M3D1_Pin, 1);
      HAL_GPIO_WritePin(M3D2_GPIO_Port, M3D2_Pin, 0);
    } else {
      HAL_GPIO_WritePin(M3D1_GPIO_Port, M3D1_Pin, 0);
      HAL_GPIO_WritePin(M3D2_GPIO_Port, M3D2_Pin, 1);
    }
  } else if (wheel->which == 4) {
    if (wheel->trust * wheel->dir > 0) {
      HAL_GPIO_WritePin(M4D1_GPIO_Port, M4D1_Pin, 1);
      HAL_GPIO_WritePin(M4D2_GPIO_Port, M4D2_Pin, 0);
    } else {
      HAL_GPIO_WritePin(M4D1_GPIO_Port, M4D1_Pin, 0);
      HAL_GPIO_WritePin(M4D2_GPIO_Port, M4D2_Pin, 1);
    }
  }
}

void driver_wheel(WHEEL *wheel) {
  wheel->trust += compute_pid(&wheel->wheel_pid, wheel->tar_speed - wheel->cur_speed);
  wheel->trust = CONFINE(wheel->trust, -TRUST_CONFINE, TRUST_CONFINE);

  if (wheel->tar_speed == 0 && wheel->cur_speed == 0) {
    wheel->trust = 0;
  }

  if (ABS(wheel->cur_speed) < 10) {
    wheel->trust = CONFINE(wheel->trust, -1500, 1500);
  }

  set_wheel_dir(wheel, wheel->trust);

  if (wheel->which == 1) {
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, ABS(wheel->trust));
  } else if (wheel->which == 2) {
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, ABS(wheel->trust));
  } else if (wheel->which == 3) {
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_3, ABS(wheel->trust));
  } else if (wheel->which == 4) {
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_4, ABS(wheel->trust));
  }

  return;
}

void init_wheel(WHEEL *wheel, uint8_t which, int8_t dir) {
  wheel->which = which;
  wheel->trust = 0;
  wheel->cur_speed = 0;
  wheel->tar_speed = 0;
  wheel->dir = dir;
  wheel->wheel_pid = init_pid(1.5, 0.5, 1.1, 1, 10);  //(1.5, 0.5, 1.3, 1, 10)

  if (wheel->which == 1) {
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
  } else if (wheel->which == 2) {
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
  } else if (wheel->which == 3) {
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
  } else if (wheel->which == 4) {
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_4);
  }

  if (wheel->which == 1) {
    HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
    TIM1->CNT = 30000;
  } else if (wheel->which == 2) {
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
    TIM2->CNT = 30000;
  } else if (wheel->which == 3) {
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
    TIM3->CNT = 30000;
  } else if (wheel->which == 4) {
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
    TIM4->CNT = 30000;
  }

  return;
}
