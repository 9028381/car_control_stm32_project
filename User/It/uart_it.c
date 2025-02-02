#include "usart.h"

void HAL__UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == &huart1)
    {
        
    }
    else if(huart->Instance == &huart2)
    {
        
    }
    else if(huart->Instance == &huart3)
    {
        
    }
    else if(huart->Instance == &huart4)
    {
        
    }
}