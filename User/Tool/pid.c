#include "pid.h"
#include "math.h"

PID init_pid(float kp, float ki, float kd, float T, float integral_max)
{
    PID pid;
    pid.kp = kp;
    pid.ki = ki;
    pid.kd = kd;
    pid.T = T;
    pid.integral_max = integral_max;
    pid.error = 0;
    pid.last_error = 0;
    pid.integral = 0;
    pid.derivative = 0;
    
    return pid;
}

float compute_pid(PID *pid, float error)
{
    pid->error = error;
    pid->integral += pid->error * pid->T;
    pid->integral = CONFINE(pid->integral, -pid->integral_max, pid->integral_max);
    pid->derivative = (pid->error - pid->last_error) / pid->T;
    pid->last_error = pid->error;
    
    return pid->kp * pid->error + pid->ki * pid->integral + pid->kd * pid->derivative;
}
