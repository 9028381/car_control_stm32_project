#include "motor.h"

void init_motor()
{
    TIM1->CNT = 3000;
    TIM2->CNT = 3000;
    TIM3->CNT = 3000;
    TIM4->CNT = 3000;

    return;
}

int16_t get_motor_speed(MOTOR *motor)
{
    int16_t speed = 0;
    if(motor->which == 1)
    {
        speed = TIM1->CNT - 3000;
        TIM1->CNT = 3000;
    }
    else if(motor->which == 2)
    {
        speed = TIM2->CNT - 3000;
        TIM2->CNT = 3000;
    }
    else if(motor->which == 3)
    {
        speed = TIM3->CNT - 3000;
        TIM3->CNT = 3000;
    }
    else if(motor->which == 4)
    {
        speed = TIM4->CNT - 3000;
        TIM4->CNT = 3000;
    }

    return speed * motor->dir;
}

void set_motor_dir(MOTOR *motor, int16_t trust)
{
    if(motor->which == 1)
    {
        if(motor->trust * motor->dir > 0)
        {
            HAL_GPIO_WritePin(M1D1_GPIO_Port, M1D1_Pin, 1);
            HAL_GPIO_WritePin(M1D2_GPIO_Port, M1D2_Pin, 0);
        }
        else
        {
            HAL_GPIO_WritePin(M1D1_GPIO_Port, M1D1_Pin, 0);
            HAL_GPIO_WritePin(M1D2_GPIO_Port, M1D2_Pin, 1);
        }
    }
    else if(motor->which == 2)
    {
        if(motor->trust * motor->dir > 0)
        {
            HAL_GPIO_WritePin(M2D1_GPIO_Port, M2D1_Pin, 1);
            HAL_GPIO_WritePin(M2D2_GPIO_Port, M2D2_Pin, 0);
        }
        else
        {
            HAL_GPIO_WritePin(M2D1_GPIO_Port, M2D1_Pin, 0);
            HAL_GPIO_WritePin(M2D2_GPIO_Port, M2D2_Pin, 1);
        }
    }
    else if(motor->which == 3)
    {
        if(motor->trust * motor->dir > 0)
        {
            HAL_GPIO_WritePin(M3D1_GPIO_Port, M3D1_Pin, 1);
            HAL_GPIO_WritePin(M3D2_GPIO_Port, M3D2_Pin, 0);
        }
        else
        {
            HAL_GPIO_WritePin(M3D1_GPIO_Port, M3D1_Pin, 0);
            HAL_GPIO_WritePin(M3D2_GPIO_Port, M3D2_Pin, 1);
        }
    }
    else if(motor->which == 4)
    {
        if(motor->trust * motor->dir > 0)
        {
            HAL_GPIO_WritePin(M4D1_GPIO_Port, M4D1_Pin, 1);
            HAL_GPIO_WritePin(M4D2_GPIO_Port, M4D2_Pin, 0);
        }
        else
        {
            HAL_GPIO_WritePin(M4D1_GPIO_Port, M4D1_Pin, 0);
            HAL_GPIO_WritePin(M4D2_GPIO_Port, M4D2_Pin, 1);
        }
    }
}

void driver_motor(MOTOR *motor)
{
    motor->trust += compute_pid(&motor->motor_pid, motor->tar_speed - motor->cur_speed);
    motor->trust = CONFINE(motor->trust, -TRUST_CONFINE, TRUST_CONFINE);

    if(motor->tar_speed == 0 && motor->cur_speed == 0)
    {
        motor->trust = 0;
    }

    set_motor_dir(motor, motor->trust);

    if(motor->which == 1)
    {
        __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, motor->trust);
    }
    else if(motor->which == 2)
    {
        __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, motor->trust);
    }
    else if(motor->which == 3)
    {
        __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_3, motor->trust);
    }
    else if(motor->which == 4)
    {
        __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_4, motor->trust);
    }

    return;
}
