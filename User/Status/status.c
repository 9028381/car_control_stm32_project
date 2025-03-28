// @63 @551

/*
椤圭洰鐘舵€佹爲浠嬬粛
status鐘舵€佹爲鐨勭洰鐨勬槸灏嗗皬杞︾殑鎵€鏈夌姸鎬�(鍖呮嫭浼犳劅鍣ㄣ€佽繍鍔ㄧ姸鎬併€佷互鍙妉ed銆佽渹楦ｅ櫒绛夎澶�)灏佽鍦ㄤ竴涓粨鏋勪綋涓�
灏唖tatus浣滀负涓€涓叏灞€鍙橀噺锛屽叧浜庡皬杞︾殑鎵€鏈夊弬鏁扮殑鑾峰彇浜庤缃潎閫氳繃璇ョ粨鏋勪綋杩涜
渚嬪
  璁剧疆灏忚溅涓€涓洿娴佺數鏈虹殑閫熷害锛� status.motor.wheel[0].tar_speed;
  寮€鍚竴涓狶ED鐏細 status.device.led_1.on = 1;
瀵规瘡涓澶囩殑鏇存柊鏁版嵁浜庨┍鍔ㄥ潎浠ュ崟涓璁捐繘琛岋紝濡傚崟涓數鏈恒€佸崟涓埖鏈恒€佸崟涓猯ed
渚嬪
  鑾峰彇闄€铻轰华鐨勫師濮嬫暟鎹� get_gyr_data(&status->sensor.gy901);
  椹卞姩status.motor.servo[0] driver_servo(&status->motor.servo[0]);
瀵逛簬姣忎釜璁惧鐨勫垵濮嬪寲銆佽幏鍙栧師濮嬫暟鎹€侀┍鍔�(杩欎笁涓牴鎹璁句笉鍚屽彲鑳戒笉鍏ㄩ兘闇€瑕�)鎻愪緵缁熶竴鐨勬帴鍙�
渚嬪
  init_xxx(XXX *xxx) 鍒濆鍖杧xx璁惧
  update_xxx(XXX *xxx) 鑾峰彇xxx鐨勫師濮嬫暟鎹�
  driver_xxx(XXX *xxx) 椹卞姩xxx璁惧
杩欎笁涓嚱鏁板叿浣撴斁鍦ㄥ摢閲岃鐪嬫瘡涓璁剧殑.h鏂囦欢
*/

#include "status.h"

#include "button.h"
#include "buzzer.h"
#include "gw_find_line.h"
#include "i2c.h"
#include "led.h"
#include "log.h"
#include "math_tool.h"
#include "servo.h"
#include "wheel.h"

STATUS status;

PID balance_pid;

void init_motor() {  // 鐢垫満鍒濆鍖�
  init_servo(&status.motor.servo[0], 1, 180);
  init_servo(&status.motor.servo[1], 2, 270);

  init_wheel(&status.motor.wheel[0], 1, -1);
  init_wheel(&status.motor.wheel[1], 2, 1);

  return;
}

void init_device() {  // 璁惧鍒濆鍖�
  init_button(&status.device.button_D2, 1, 0);
  init_button(&status.device.button_B11, 2, 0);
  init_LED(&status.device.led_on_board, 1, 1);
  init_LED(&status.device.led1, 2, 1);
  init_LED(&status.device.led2, 3, 1);
  init_BUZZER(&status.device.buzzer, 1, 1);

  return;
}

void init_sensor(STATUS *status) {  // 浼犳劅鍣ㄥ垵濮嬪寲
  init_gyr(&status->sensor.gy901);
  init_gw_8bit(&status->sensor.gw_8bit);
}

void init_state(STATUS *status, uint8_t T)  // 鐘舵€佸垵濮嬪寲
{
  status->state.T = T;
  status->state.time = 0;
  status->state.motion = STOP;
  status->state.cur_angle = 0;
  status->state.tar_angle = 90;

  status->state.gw_8bit = 0x00;  // 8浣嶇伆搴︿紶鎰熷櫒鏁版嵁
  balance_pid = init_pid(300, 0, 0, 1, 10);

  return;
}

void init_status(STATUS *status, uint8_t T) {  // 鐘舵€佹爲鍒濆鍖�

  init_state(status, T);

  init_sensor(status);

  init_motor();

  init_device();

  return;
}

void update_status(STATUS *status) {  // 鐘舵€佹爲鏇存柊鏁版嵁
  status->motor.wheel[0].cur_speed = get_wheel_speed(&status->motor.wheel[0]);
  status->motor.wheel[1].cur_speed = get_wheel_speed(&status->motor.wheel[1]);
  status->motor.wheel[2].cur_speed = get_wheel_speed(&status->motor.wheel[2]);
  status->motor.wheel[3].cur_speed = get_wheel_speed(&status->motor.wheel[3]);

  get_gyr_data(&hi2c1, &status->sensor.gy901);
  get_gw_8bit_data(&hi2c1, &status->sensor.gw_8bit);

  return;
}

void driver_status(STATUS *status) {  // 鐘舵€佹暟椹卞姩

  status->state.cur_angle = get_gyr_value(&status->sensor.gy901, gyr_z_yaw);

  // if (status->state.time == 1000) {
  //   status->state.motion = KEEP_ANGLE;
  // }

  // if (status->state.motion == STOP) {
  //   status->motor.wheel[0].tar_speed = 0;
  //   status->motor.wheel[1].tar_speed = 0;
  //   if (status->motor.wheel[0].cur_speed == 0)
  //     status->motor.wheel[0].trust = 0;
  //   if (status->motor.wheel[1].cur_speed == 0)
  //     status->motor.wheel[1].trust = 0;
  // } else if (status->state.motion == KEEP_ANGLE) {
  //   float target_angle = (status->state.tar_angle + status->state.initial_angle);
  //   float diff_angle = target_angle - status->state.cur_angle;
  //   // 灏嗚搴﹀樊鍊艰皟鏁村埌 [-180掳, 180掳] 鑼冨洿鍐�
  //   if (diff_angle > 180.0) {
  //     diff_angle -= 360.0;
  //   } else if (diff_angle < -180.0) {
  //     diff_angle += 360.0;
  //   } else if (status->state.motion == FIND_LINE) {
  //     int32_t line_diff = gw_get_line_diff(&status->sensor.gw_8bit);
  //     status->motor.wheel[0].tar_speed = 2000 - line_diff;
  //     status->motor.wheel[1].tar_speed = 2000 + line_diff;
  //     if (ABS(line_diff) > 5000) {
  //       status->motor.wheel[0].tar_speed = CLAMP(status->motor.wheel[0].tar_speed, 1000);
  //       status->motor.wheel[1].tar_speed = CLAMP(status->motor.wheel[1].tar_speed, 1000);
  //     }
  //   }
  //   int16_t diff = compute_pid(&status->sensor.gy901.gy901_keep_angle_pid, diff_angle);
  //   status->motor.wheel[0].tar_speed = diff;
  //   status->motor.wheel[1].tar_speed = -diff;
  //   log_uprintf(&huart1, "diff_angle %5.2f\r\n", diff_angle);
  float error_angle = get_gyr_value(&status->sensor.gy901, gyr_y_pitch);

  int16_t diff = compute_pid(&balance_pid, error_angle);

  diff = CONFINE(diff, -150, 150);

  status->motor.wheel[0].tar_speed = diff;
  status->motor.wheel[1].tar_speed = diff;
  log_uprintf(&huart1, "kp : %f\r\nkd : %f\r\n\r\n\r\n", balance_pid.kp, balance_pid.kd);

  // }
  driver_button(&status->device.button_D2);
  driver_button(&status->device.button_B11);

  driver_LED(&status->device.led_on_board);
  driver_LED(&status->device.led1);
  driver_LED(&status->device.led2);

  driver_servo(&status->motor.servo[0]);
  driver_servo(&status->motor.servo[1]);

  driver_BUZZER(&status->device.buzzer);

  driver_wheel(&status->motor.wheel[0]);
  driver_wheel(&status->motor.wheel[1]);

  status->device.led1.on = 1;
  status->device.led2.on = 1;

  return;
}

void after_init_state() {
  get_gyr_data(&hi2c1, &status.sensor.gy901);
  HAL_Delay(50);
  status.state.initial_angle = get_gyr_value(&status.sensor.gy901, gyr_z_yaw);
  log_uprintf(&huart1, "ok\n");
}