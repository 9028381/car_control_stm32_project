// @63
#include "array.h"

#include "log.h"
#include "usart.h"

int array_sum(unsigned len, const short array[len]) {
  int sum = 0;
  for (unsigned int i = 0; i < len; i++)
    sum += array[i];
  return sum;
}

unsigned array_find_min_index(unsigned len, const short array[len]) {
  unsigned min_index = 0;
  short min = array[min_index];
  for (unsigned i = 1; i < len; i++)
    if (array[i] < min) {
      min = array[i];
      min_index = i;
    }

  return min_index;
}

unsigned array_find_max_index(unsigned len, const short array[len]) {
  unsigned max_index = 0;
  short max = array[max_index];
  for (unsigned i = 1; i < len; i++)
    if (array[i] > max) {
      max = array[i];
      max_index = i;
    }

  return max_index;
}

void array_copy(unsigned len, const short src[len], short dest[len]) {
  for (unsigned i = 0; i < len; i++)
    dest[i] = src[i];
}

void array_display(unsigned len, const short array[len]) {
  PRINTF("[");
  for (unsigned int i = 0; i < len - 1; i++)
    PRINTF("%hd, ", array[i]);
  PRINTLN("%hd]", array[len - 1]);
}

unsigned forward_difference(unsigned len, const short src[len],
                            short dest[len - 1]) {
  if (len == 0)
    return 0;

  for (unsigned i = 0; i < len - 1; i++)
    dest[i] = src[i + 1] - src[i];

  return len - 1;
}

unsigned forward_difference_multiple(unsigned len, unsigned forward,
                                     const short src[len],
                                     short dest[len - forward]) {
  if (len == 0)
    return 0;

  if (forward == 0) {
    array_copy(len, src, dest);
    return len;
  }

  if (forward >= len)
    return 0;

  if (forward == 1)
    return forward_difference(len, src, dest);

  unsigned dest_len = len - forward;
  for (unsigned i = 0; i < dest_len; i++)
    dest[i] = src[i + forward] - src[i];
  return dest_len;
}

unsigned array_count_less_than(unsigned len, const short array[len],
                               short compare) {
  unsigned count = 0;
  for (unsigned i = 0; i < len; i++)
    if (array[i] < compare)
      count++;
  return count;
}

unsigned array_count_continue_less_than(unsigned len, const short array[len],
                                        short compare) {
  int max_count = 0;
  int count = 0;

  for (unsigned i = 0; i < len; i++) {
    if (array[i] < compare)
      count++;
    else if (count > 0) {
      if (count > max_count)
        max_count = count;
      count = 0;
    }
  }
  return max_count;
}

struct SumAndCount array_mean_index_less_than(unsigned len,
                                              const short array[len],
                                              short compare) {
  int sum = 0;
  int count = 0;

  for (unsigned i = 0; i < len; i++) {
    if (array[i] < compare) {
      sum += i;
      count += 1;
    }
  }

  struct SumAndCount result = {.sum = sum, .count = count};
  return result;
}

#ifdef __ARRAY_FIND_MIN_INDEX__

#include <stdio.h>

void array_display(unsigned len, const unsigned short array[len]) {
  putchar('[');
  for (unsigned int i = 0; i < len - 1; i++)
    printf("%d, ", array[i]);
  printf("%d", array[len - 1]);
  putchar(']');
  putchar('\n');
}

void test1() {
  unsigned short array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  array_display(10, array);
  printf("%u\n", array_find_min_index(10, array));
}

void test2() {
  unsigned short array[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
  array_display(10, array);
  printf("%u\n", array_find_min_index(10, array));
}

void test3() {
  unsigned short array[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  array_display(10, array);
  printf("%u\n", array_find_min_index(10, array));
}

void test4() {
  unsigned short array[] = {8, 8, 3, 2, 4, 6, 1, 6, 48, 8};
  array_display(10, array);
  printf("%u\n", array_find_min_index(10, array));
}

int main() {
  test1();
  test2();
  test3();
  test4();
}

#endif /* ifdef __ARRAY_FIND_MIN_INDEX__ */