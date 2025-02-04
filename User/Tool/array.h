// @63

#ifndef __ARRAY_H__
#define __ARRAY_H__

int array_sum(unsigned len, const short array[len]);
void array_copy(unsigned len, const short src[len], short dest[len]);
void array_display(unsigned len, const short array[len]);

unsigned array_find_min_index(unsigned len, const short array[len]);
unsigned array_find_max_index(unsigned len, const short array[len]);
unsigned array_count_less_than(unsigned len, const short array[len],
                               short compare);
unsigned array_count_continue_less_than(unsigned len, const short array[len],
                                        short compare);

struct SumAndCount {
  int sum;
  int count;
};
struct SumAndCount
array_mean_index_less_than(unsigned len, const short array[len], short compare);

/// Return the length of the convolution array
unsigned convolve_unit(unsigned len, unsigned kernel_len, const short src[len],
                       short dest[len - kernel_len]);

unsigned forward_difference(unsigned len, const short src[len],
                            short dest[len - 1]);
unsigned forward_difference_multiple(unsigned len, unsigned forward,
                                     const short src[len],
                                     short dest[len - forward]);
#endif  // !__ARRAY_H__