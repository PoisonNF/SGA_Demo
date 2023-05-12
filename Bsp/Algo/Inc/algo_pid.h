#ifndef __ALGO_PID_H_
#define __ALGO_PID_H_

#include "drv_hal_conf.h"

typedef struct pid
{
    float fKp;          /* 比例系数 */
    float fKi;          /* 微分系数 */
    float fKd;          /* 积分系数 */

    float fMax_Out;     /* PID最大输出 */
    float fMax_Iout;    /* PID最大积分输出 */

    float fExp_Value;   /* 设定的期望值 */
    float fCurr_Value;  /* 当前PID值 */

    float fPre_Value;   /* 前一次PID值 */
    float fCtrl_Out;    /* 三项叠加输出值 */
    
    float fPout;        /* 比例项输出 */
    float fIout;        /* 积分项输出 */
    float fDout;        /* 微分项输出 */

    float fDbuf[3];     /* 微分项最近三个值 0为最新 1上一次 2上上次 */
    float fError[3];    /* 误差项最近三个值 0为最新 1上一次 2上上次 */

}tagPID_T;

void Algo_PID_Update(tagPID_T *_tPid,float _faPID[3]);
float Algo_PID_Calculate(tagPID_T *_tPid,float _fCurrValue,float _fExpValue);
void Algo_PID_Clear(tagPID_T *_tPid);
void Algo_PID_Init(tagPID_T *_tPid);

#endif
