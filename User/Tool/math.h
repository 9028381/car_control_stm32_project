#ifndef __MATH_H
#define __MATH_H


#define ABS(x) ((x) > 0 ? (x) : -(x)) // 宏定义绝对值函数
#define CONFINE(x, a, b) ((x) < (a) ? (a) : ((x) > (b) ? (b) : (x))) // 限制x在[a, b]之间

#endif
