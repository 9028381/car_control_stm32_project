#ifndef __MATH_H
#define __MATH_H

#define ABS(x) ((x) > 0 ? (x) : -(x))  // 宏定义绝对值函数
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define LIMIT_MAX(x, max) MIN(x, max)
#define LIMIT_MIN(x, min) MAX(x, min)
#define LIMIT(x, min, max) LIMIT_MIN(LIMIT_MAX(x, max), min)
#define CLAMP(x, range) LIMIT(x, -(range), range)
#define CONFINE(x, a, b) ((x) < (a) ? (a) : ((x) > (b) ? (b) : (x)))

#define POW(base, exp) ({              \
  int _result = 1;                     \
  for (int _i = 0; _i < (exp); ++_i) { \
    _result *= (base);                 \
  }                                    \
  _result;                             \
})  // 宏定义幂函数

#endif
