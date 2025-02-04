// @551 @LQ
// 轮趣科技N10P雷达的驱动
#include "radar.h"

extern uint16_t receive_cnt;
int data_flag = 0, data_process_flag = 0;
PointDataProcessDef PointDataProcess[1200];  // 更新225个数据
LiDARFrameTypeDef Pack_Data;
PointDataProcessDef
    Dataprocess[1200];  // 用于小车避障、跟随、走直线、ELE雷达避障的雷达数据
int data_cnt = 0, data_error_cnt = 0;
uint8_t temp_data;
uint16_t radar_data[360] = {0};

void data_process(void)  // 数据处理
{
  //	static int data_cnt = 0;
  int i, m, n;
  uint32_t distance_sum[32] = {0};  // 2个点的距离和的数组
  float start_angle =
      (((uint16_t)Pack_Data.start_angle_h << 8) + Pack_Data.start_angle_l) /
      100.0;  // 计算32个点的开始角度
  float end_angle =
      (((uint16_t)Pack_Data.end_angle_h << 8) + Pack_Data.end_angle_l) /
      100.0;  // 计算32个点的结束角度
  float area_angle[32] = {0};

  if (start_angle > end_angle)  // 结束角度和开始角度被0度分割的情况
    end_angle += 360;

  data_process_flag = 0;  // 标志位清零
  for (m = 0; m < 32; m++) {
    area_angle[m] = start_angle + (end_angle - start_angle) / 32 * m;
    if (area_angle[m] > 360)
      area_angle[m] -= 360;
  }

  for (i = 0; i < 32; i++) {
    distance_sum[i] += ((uint16_t)Pack_Data.point[i].distance_h << 8) +
                       Pack_Data.point[i].distance_l;  // 数据高低8位合并
  }

  for (n = 0; n < 32; n++) {
    PointDataProcess[data_cnt + n].angle = area_angle[n];
    PointDataProcess[data_cnt + n].distance =
        distance_sum[n];  // 一帧数据为32个点
  }
  data_cnt += 32;
  if (data_cnt >=
      1152)  // 雷达转一圈大概有1152个点（大概数值，每圈的点数都不固定，一帧大约为10度，一圈大约是36帧数据=32*36=1152）
  {
    for (i = 0; i < 1152; i++) {
      Dataprocess[i].angle = PointDataProcess[i].angle;
      Dataprocess[i].distance =
          PointDataProcess[i]
              .distance;  // 将数组PointDataProcess的数据转移到数组Dataprocess中，避免覆盖数据
    }
    data_cnt = 0;
    data_flag = 1;
  }
}

void Ladar_drive(uint8_t temp_data) {
  static uint8_t Count = 0;

  static uint8_t state = 0;    // 状态位
  static uint8_t crc_sum = 0;  // 校验和
  static uint8_t cnt = 0;      // 用于一帧16个点的计数

  switch (state) {
    case 0:
      if (temp_data == HEADER_0)  // 头固定
      {
        Pack_Data.header_0 = temp_data;
        state++;
        // 校验
        crc_sum += temp_data;
      } else
        state = 0, crc_sum = 0;
      break;
    case 1:
      if (temp_data == HEADER_1)  // 头固定
      {
        Pack_Data.header_1 = temp_data;
        state++;
        crc_sum += temp_data;
      } else
        state = 0, crc_sum = 0;
      break;
    case 2:
      if (temp_data == Length_)  // 字长固定
      {
        Pack_Data.ver_len = temp_data;
        state++;
        crc_sum += temp_data;
      } else
        state = 0, crc_sum = 0;
      break;
    case 3:
      Pack_Data.speed_h = temp_data;  // 速度高八位
      state++;
      crc_sum += temp_data;
      break;
    case 4:
      Pack_Data.speed_l = temp_data;  // 速度低八位
      state++;
      crc_sum += temp_data;
      break;
    case 5:
      Pack_Data.start_angle_h = temp_data;  // 开始角度高八位
      state++;
      crc_sum += temp_data;
      break;
    case 6:
      Pack_Data.start_angle_l = temp_data;  // 开始角度低八位
      state++;
      crc_sum += temp_data;
      break;

    case 7:
    case 10:
    case 13:
    case 16:
    case 19:
    case 22:
    case 25:
    case 28:
    case 31:
    case 34:
    case 37:
    case 40:
    case 43:
    case 46:
    case 49:
    case 52:

    case 55:
    case 58:
    case 61:
    case 64:
    case 67:
    case 70:
    case 73:
    case 76:
    case 79:
    case 82:
    case 85:
    case 88:
    case 91:
    case 94:
    case 97:
    case 100:
      Pack_Data.point[cnt].distance_h = temp_data;  // 16个点的距离数据，高字节
      state++;
      crc_sum += temp_data;
      break;

    case 8:
    case 11:
    case 14:
    case 17:
    case 20:
    case 23:
    case 26:
    case 29:
    case 32:
    case 35:
    case 38:
    case 41:
    case 44:
    case 47:
    case 50:
    case 53:

    case 56:
    case 59:
    case 62:
    case 65:
    case 68:
    case 71:
    case 74:
    case 77:
    case 80:
    case 83:
    case 86:
    case 89:
    case 92:
    case 95:
    case 98:
    case 101:
      Pack_Data.point[cnt].distance_l = temp_data;  // 16个点的距离数据，低字节
      state++;
      crc_sum += temp_data;
      break;

    case 9:
    case 12:
    case 15:
    case 18:
    case 21:
    case 24:
    case 27:
    case 30:
    case 33:
    case 36:
    case 39:
    case 42:
    case 45:
    case 48:
    case 51:
    case 54:

    case 57:
    case 60:
    case 63:
    case 66:
    case 69:
    case 72:
    case 75:
    case 78:
    case 81:
    case 84:
    case 87:
    case 90:
    case 93:
    case 96:
    case 99:
    case 102:
      Pack_Data.point[cnt].Strong = temp_data;  // 16个点的强度数据
      state++;
      crc_sum += temp_data;
      cnt++;
      break;
    case 103:
    case 104:
      state++;
      crc_sum += temp_data;
      cnt++;
      break;
    case 105:
      Pack_Data.end_angle_h = temp_data;  // 结束角度的高八位
      state++;
      crc_sum += temp_data;
      break;
    case 106:
      Pack_Data.end_angle_l = temp_data;  // 结束角度的低八位
      state++;
      crc_sum += temp_data;
      break;
    case 107:
      Pack_Data.crc = temp_data;  // 校验
      state = 0;
      cnt = 0;
      if (crc_sum == Pack_Data.crc) {
        // data_process();//数据处理，校验正确不断刷新存储的数据
        data_process_flag = 1;
      } else {
        memset(&Pack_Data, 0, sizeof(Pack_Data));  // 清零
        data_error_cnt++;
      }
      crc_sum = 0;  // 校验和清零
      break;
    default:
      break;
  }
}

void radar_data_process() {
  uint16_t angle = 0;
  uint16_t sum = 0;
  uint16_t total = 0;
  if (data_process_flag)
    data_process();  // 处理一帧数据
  if (data_flag)     // 处理一圈数据，一圈只打印一次
  {
    for (int i = 0; i < 1152; i++)  // 遍历打印
    {
      if ((Dataprocess[i].angle >= angle - 1) &&
          (Dataprocess[i].angle <= angle + 1)) {
        if (Dataprocess[i].distance != 0) {
          total += Dataprocess[i].distance;
          sum++;
        }
      } else {
        if ((sum != 0) && ((total / sum) != 0)) {  // 这个雷达会有时候会读取出0距离
          radar_data[angle] = (total / sum);
          total = 0;
          sum = 0;
        }
        angle++;
        if (angle == 360)
          angle = 0;
      }
    }
    data_flag = 0;  // 标志位清零
    // printf("%hu\r\n", radar_data[180]);
  }
}

float float_abs(float input) {
  if (input < 0)
    return -input;
  else
    return input;
}

uint16_t get_radar_value(uint16_t angle) {
  return radar_data[CONFINE(angle, 0, 359)];
}
