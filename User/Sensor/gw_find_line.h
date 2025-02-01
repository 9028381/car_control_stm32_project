#ifndef __GW_FIND_LINE_H
#define __GW_FIND_LINE_H

#include "main.h"

typedef enum  LINE_TYPE
{
    CROSS,      //十字路口
    T,          //T型路口
    R,          //右拐
    L,          //左拐
    RT,         //右T型路口
    LT,         //左T型路口
    STRAIGHT,   //直行
    NONE,       //不是特殊路口
}LINE_TYPE;

uint8_t gw_gray_get_line_digital_is_black();
int16_t compute_gw_gray_diff(uint8_t gray[8]);

#endif
