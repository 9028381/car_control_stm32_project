#ifndef __PID_H
#define __PID_H

typedef struct PID
{
    float kp;
    float ki;
    float kd;
    float T;          //采样周期
    float error;
    float last_error;
    float integral;
    float derivative;
    float integral_max;
}PID;

PID init_pid(float kp, float ki, float kd, float T, float integral_max);
float compute_pid(PID *pid, float error);

#endif
