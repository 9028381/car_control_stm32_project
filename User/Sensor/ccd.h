// @63 @551

#ifndef __CCD_H_
#define __CCD_H_

#define STM32

#ifdef MSPM0

#define CLK_UP DL_GPIO_writePins(CCD_CLK_PORT, CCD_CLK_PIN)
#define CLK_DOWN DL_GPIO_clearPins(CCD_CLK_PORT, CCD_CLK_PIN)
#define SI_UP DL_GPIO_writePins(CCD_SI_PORT, CCD_SI_PIN)
#define SI_DOWN DL_GPIO_clearPins(CCD_SI_PORT, CCD_SI_PIN)

#endif

#ifdef STM32

#include "adc.h"
#include "main.h"
#include "tim.h"

#define CLK_UP HAL_GPIO_WritePin(CCD_CLK_GPIO_Port, CCD_CLK_Pin, 1)
#define CLK_DOWN HAL_GPIO_WritePin(CCD_CLK_GPIO_Port, CCD_CLK_Pin, 0)
#define SI_UP HAL_GPIO_WritePin(CCD_SI_GPIO_Port, CCD_SI_Pin, 1)
#define SI_DOWN HAL_GPIO_WritePin(CCD_SI_GPIO_Port, CCD_SI_Pin, 0)

#endif

void driver_ccd();
void get_ccd_data();
int16_t ccd_compute();

#endif
