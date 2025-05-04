#include "dma.h"
#include "log.h"
#include "usart.h"

#define BUFFER_SIZE 255

uint8_t uart1_buf[BUFFER_SIZE] = {0};
uint8_t uart2_buf[BUFFER_SIZE] = {0};
uint8_t uart3_buf[BUFFER_SIZE] = {0};
uint8_t uart4_buf[BUFFER_SIZE] = {0};

extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_uart4_rx;

void start_uart_idle_it(UART_HandleTypeDef *huart, uint8_t *buf) {
  __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
  HAL_UART_Receive_DMA(huart, buf, 255);
}

void init_uart_idle_it() {
  start_uart_idle_it(&huart1, uart1_buf);
  start_uart_idle_it(&huart2, uart2_buf);
  start_uart_idle_it(&huart3, uart3_buf);
  start_uart_idle_it(&huart4, uart4_buf);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if (RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE)) {  // 清除空闲中断标志（否则会一直不断进入中断）
    __HAL_UART_CLEAR_IDLEFLAG(huart);
    HAL_UART_DMAStop(huart);  // 停止DMA接收
  }
  if (huart == &huart1) {
    log_uprintf(&huart1, "uart1 idle\n");
    start_uart_idle_it(&huart1, uart1_buf);
    uint8_t data_length = BUFFER_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);  // 获取接收到的数据长度
  } else if (huart == &huart2) {
    log_uprintf(&huart1, "uart2 idle\n");
    start_uart_idle_it(&huart2, uart2_buf);
    uint8_t data_length = BUFFER_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);  // 获取接收到的数据长度
  } else if (huart == &huart3) {
    log_uprintf(&huart1, "uart3 idle\n");
    start_uart_idle_it(&huart3, uart3_buf);
    uint8_t data_length = BUFFER_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart3_rx);  // 获取接收到的数据长度
  } else if (huart == &huart4) {
    log_uprintf(&huart1, "uart3 idle\n");
    start_uart_idle_it(&huart4, uart4_buf);
    uint8_t data_length = BUFFER_SIZE - __HAL_DMA_GET_COUNTER(&hdma_uart4_rx);  // 获取接收到的数据长度
  }
}