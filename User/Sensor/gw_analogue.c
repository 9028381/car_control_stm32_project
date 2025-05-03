#include "adc.h"
#include "gpio.h"
#include "gw_anagloge.h"
#include "log.h"
#include "main.h"
#include "status.h"

float distance[8] = {-30, -20, -15, -10, 10, 15, 20, 30};

void gw_analogue_gray_show(GW_ANALOGUE *gw_analogue) {
  uint8_t buf = gw_analogue->digital_8bit;
  char str[9];
  str[8] = '\0';
  for (int i = 0; i < 8; i++) {
    str[i] = buf & 0x80 ? '#' : '.';
    buf <<= 1;
  }
  PRINTLN("%s", str);
}

void init_gw_analogue(GW_ANALOGUE *gw_analogue) {
  // Initialize the ADC and GPIO for the analogue channels
  for (int i = 0; i < 8; i++) {
    gw_analogue->channel[i] = 0;  // Initialize channel values to 0
  }
  for (int i = 0; i < 8; i++) {
    gw_analogue->correction_data_w[i] = 0;  // Initialize correction data to 0
    gw_analogue->correction_data_b[i] = 0;  // Initialize correction data to 0
  }
  gw_analogue->digital_high_threshold[0] = 81;   // Initialize high threshold to 0
  gw_analogue->digital_high_threshold[1] = 137;  // Initialize high threshold to 0
  gw_analogue->digital_high_threshold[2] = 77;   // Initialize high threshold to 0
  gw_analogue->digital_high_threshold[3] = 82;   // Initialize high threshold to 0
  gw_analogue->digital_high_threshold[4] = 61;   // Initialize high threshold to 0
  gw_analogue->digital_high_threshold[5] = 115;  // Initialize high threshold to 0
  gw_analogue->digital_high_threshold[6] = 129;  // Initialize high threshold to 0
  gw_analogue->digital_high_threshold[7] = 75;   // Initialize high threshold to 0

  gw_analogue->digital_low_threshold[0] = 49;  // Initialize low threshold to 0
  gw_analogue->digital_low_threshold[1] = 95;  // Initialize low threshold to 0
  gw_analogue->digital_low_threshold[2] = 49;  // Initialize low threshold to 0
  gw_analogue->digital_low_threshold[3] = 55;  // Initialize low threshold to 0
  gw_analogue->digital_low_threshold[4] = 37;  // Initialize low threshold to 0
  gw_analogue->digital_low_threshold[5] = 77;  // Initialize low threshold to 0
  gw_analogue->digital_low_threshold[6] = 89;  // Initialize low threshold to 0
  gw_analogue->digital_low_threshold[7] = 47;  // Initialize low threshold to 0

  for (int i = 0; i < 8; i++) {
    gw_analogue->correction_data_w[i] = 2 * gw_analogue->digital_high_threshold[i] - gw_analogue->digital_low_threshold[i];  // Initialize high threshold to 0
    gw_analogue->correction_data_b[i] = 2 * gw_analogue->digital_low_threshold[i] - gw_analogue->digital_high_threshold[i];  // Initialize low threshold to 0
  }

  gw_analogue->sta = 0;           // Set the state to 0 (normal mode)
  gw_analogue->digital_8bit = 0;  // Initialize the 8-bit digital value to 0

  gw_analogue->diff = 0.0f;  // Initialize the difference value to 0.0f

  select_channel(0);  // Select channel 0 for initial setup
}

void select_channel(uint8_t channel) {
  // Select the ADC channel to read from
  if (channel & 0x01) {
    HAL_GPIO_WritePin(IO2_GPIO_Port, IO2_Pin, GPIO_PIN_SET);  // Set PA0 high
  } else {
    HAL_GPIO_WritePin(IO2_GPIO_Port, IO2_Pin, GPIO_PIN_RESET);  // Set PA0 low
  }
  if (channel & 0x02) {
    HAL_GPIO_WritePin(IO3_GPIO_Port, IO3_Pin, GPIO_PIN_SET);  // Set PA1 high
  } else {
    HAL_GPIO_WritePin(IO3_GPIO_Port, IO3_Pin, GPIO_PIN_RESET);  // Set PA1 low
  }
  if (channel & 0x04) {
    HAL_GPIO_WritePin(IO4_GPIO_Port, IO4_Pin, GPIO_PIN_SET);  // Set PA2 high
  } else {
    HAL_GPIO_WritePin(IO4_GPIO_Port, IO4_Pin, GPIO_PIN_RESET);  // Set PA2 low
  }
}

void get_gw_raw_data(GW_ANALOGUE *gw_analogue) {
  // Read the ADC value for the selected channel
  for (int i = 0; i < 8; i++) {
    select_channel(i);                                   // Select the channel to read from
    HAL_ADC_Start(&hadc3);                               // Start the ADC conversion
    HAL_ADC_PollForConversion(&hadc3, 1);                // Wait for conversion to complete
    gw_analogue->channel[i] = HAL_ADC_GetValue(&hadc3);  // Get the ADC value
    HAL_ADC_Stop(&hadc3);                                // Stop the ADC conversion
  }
}

void correct_gw_analogue(GW_ANALOGUE *gw_analogue) {
  if (gw_analogue->sta == 0) {
    for (int i = 0; i < 8; i++) {
      select_channel(i);                                             // Select the channel to read from
      HAL_ADC_Start(&hadc3);                                         // Start the ADC conversion
      HAL_ADC_PollForConversion(&hadc3, 1);                          // Wait for conversion to complete
      gw_analogue->correction_data_w[i] = HAL_ADC_GetValue(&hadc3);  // Get the ADC value      HAL_ADC_Stop(&hadc3);                                // Stop the ADC conversion
    }
    status.device.led_on_board.on = 1;
    gw_analogue->sta = 1;  // Set the state to calibration mode 1
    return;
  }
  if (gw_analogue->sta == 1) {
    for (int i = 0; i < 8; i++) {
      select_channel(i);                                             // Select the channel to read from
      HAL_ADC_Start(&hadc3);                                         // Start the ADC conversion
      HAL_ADC_PollForConversion(&hadc3, 1);                          // Wait for conversion to complete
      gw_analogue->correction_data_b[i] = HAL_ADC_GetValue(&hadc3);  // Get the ADC value
      HAL_ADC_Stop(&hadc3);                                          // Stop the ADC conversion
    }
    status.device.led_on_board.on = 0;
    gw_analogue->sta = 0;  // Set the state to calibration mode 2
    for (int i = 0; i < 8; i++) {
      gw_analogue->digital_low_threshold[i] = gw_analogue->correction_data_b[i] +
                                              (gw_analogue->correction_data_w[i] - gw_analogue->correction_data_b[i]) * 0.33;
      // Calculate the low threshold
      gw_analogue->digital_high_threshold[i] = gw_analogue->correction_data_b[i] +
                                               (gw_analogue->correction_data_w[i] - gw_analogue->correction_data_b[i]) * 0.66;
      // Calculate the high threshold
    }
    for (int i = 0; i < 8; i++) {
      log_uprintf(&huart1, "%d ", gw_analogue->digital_low_threshold[i]);
    }
    log_uprintf(&huart1, "\n\n");
    for (int i = 0; i < 8; i++) {
      log_uprintf(&huart1, "%d ", gw_analogue->digital_high_threshold[i]);
    }
    return;
  }
}

void get_gw_analoge_digital_data(GW_ANALOGUE *gw_analogue) {
  for (int i = 0; i < 8; i++) {
    if (gw_analogue->channel[i] > gw_analogue->digital_high_threshold[i]) {
      gw_analogue->digital_8bit &= ~(1 << i);
    } else if (gw_analogue->channel[i] < gw_analogue->digital_low_threshold[i]) {
      gw_analogue->digital_8bit |= (1 << i);
    }
  }
}

float normalize_gray_data(uint8_t max, uint8_t min, uint8_t now) {
  return (((float)(now - min) / (float)(max - min)) * 100);
}

void normalize_gray_weight(float *raw_data) {
  float total = 0;
  for (int i = 2; i < 6; i++) {
    total += raw_data[i];
  }
  if (total == 0) {
    return;
  }
  for (int i = 2; i < 6; i++) {
    raw_data[i] = (raw_data[i] / total);
  }

  return;
}

void get_gw_analogue_analogue_diff(GW_ANALOGUE *gw_analogue) {
  float buff[8] = {0};
  float diff = 0;
  for (int i = 2; i < 6; i++) {
    if (gw_analogue->channel[i] < gw_analogue->digital_high_threshold[i])
      buff[i] = 100 - normalize_gray_data(gw_analogue->correction_data_w[i], gw_analogue->correction_data_b[i], gw_analogue->channel[i]);
  }
  normalize_gray_weight(buff);
  for (int i = 2; i < 6; i++) {
    diff += buff[i] * distance[i];
  }

  status.sensor.gw_analogue.diff = diff;
}