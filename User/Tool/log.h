#ifndef __LOG_H__
#define __LOG_H__

#define LOG_UART huart1
void log_uprintf(UART_HandleTypeDef huart, const char *format, ...);

#define PRINTF(fmt, ...) log_uprintf(LOG_UART, fmt, ##__VA_ARGS__);
#define PRINTLN(fmt, ...) PRINTF(fmt "\r\n", ##__VA_ARGS__)

/// minute:second:frequency
#define LOG_TIME_FMT_TYPE "%02u:%02u:%02u"
#define LOG_TIME_FMT(t)                                                        \
  ((t / STATUS_FREQ) / 60), ((t / STATUS_FREQ) % 60), (t % STATUS_FREQ)

#define LOG_EVENT(level, fmt, ...)                                             \
  PRINTLN(level " " LOG_TIME_FMT_TYPE " " fmt, LOG_TIME_FMT(status.times),     \
          ##__VA_ARGS__)

#define LOG_SPAN(level, fmt, ...)                                              \
  LOG_EVENT(level, "%s:%u " fmt, __func__, __LINE__, ##__VA_ARGS__)

#define ERROR(fmt, ...) LOG_EVENT("E", fmt, ##__VA_ARGS__)
#define WARN(fmt, ...) LOG_EVENT("W", fmt, ##__VA_ARGS__)
#define INFO(fmt, ...) LOG_EVENT("I", fmt, ##__VA_ARGS__)
#define DEBUG(fmt, ...) LOG_SPAN("D", fmt, ##__VA_ARGS__)
#define TRACE(var, fmt) LOG_SPAN("T", #var "=" fmt, var)

#define THROW_ERROR(fmt, ...) LOG_SPAN("E", fmt, ##__VA_ARGS__)
#define THROW_WARN(fmt, ...) LOG_SPAN("W", fmt, ##__VA_ARGS__)

#ifdef DEV
#define LOG_ENABLE
#endif // DEV

#ifndef LOG_ENABLE
#undef LOG_EVENT
#define LOG_EVENT(level, fmt, ...)
#endif // !LOG_ENABLE

#endif // !__LOG_H__