/****************************************************************************

* Sigma团队

* 文件名: algo_pid.c

* 内容简述：PID相关函数

* 文件历史：

* 版本号		日期	    作者		    说明
*  2.4 	    2023-05-12	  鲍程璐		 创建该文件

****************************************************************************/
#include "algo_pid.h"

/**
 * @brief 更新PID系数函数
 * @param _tPid-PID结构体指针
 * @param _faPID-PID系数，要求存放顺序为P,I,D
 * @retval Null
*/
void Algo_PID_Update(tagPID_T *_tPid,float _faPID[3])
{
    /* 更新PID系数 */
    _tPid->fKp = _faPID[0];
    _tPid->fKi = _faPID[1];
    _tPid->fKd = _faPID[2];
}

/**
 * @brief PID计算函数
 * @param _tPid-PID结构体指针
 * @param _fCurrValue-当前值
 * @param _fExpValue-期望值
 * @retval float 经过PID计算后的输出值
*/
float Algo_PID_Calculate(tagPID_T *_tPid,float _fCurrValue,float _fExpValue)
{
    int Delt_Out; /* PID输出增量值 */

    /* 设定期望值和当前值 */
    _tPid->fExp_Value  = _fExpValue;
    _tPid->fCurr_Value = _fCurrValue;
    
    _tPid->fError[0] = _fExpValue - _fCurrValue;

    /* 采用增量式PID */

    /* 以本次误差与上次误差的差值作为比例项的输入带入计算 */
    _tPid->fPout = _tPid->fKp * (_tPid->fError[0] - _tPid->fError[1]);

    /* 以本次误差作为积分项带入计算 */
    _tPid->fIout = _tPid->fKi * _tPid->fError[0];

    /* 迭代微分项的数组 */
    _tPid->fDbuf[2] = _tPid->fDbuf[1];
    _tPid->fDbuf[1] = _tPid->fDbuf[0];

    /* 以本次误差与上次误差的差值减去上次误差与上上次误差的差值作为微分项的输入带入计算 */
    _tPid->fDbuf[0] = (_tPid->fError[0] - 2.0f * _tPid->fError[1] + _tPid->fError[2]);
    _tPid->fDout = _tPid->fKd * _tPid->fDbuf[0];
    
    /* 结果处理 */
    Delt_Out = (int)(_tPid->fPout + _tPid->fIout + _tPid->fDout);    /* 计算增量输出 */
    _tPid->fCtrl_Out = _tPid->fPre_Value + Delt_Out;                 /* 与前值累加 */
    _tPid->fPre_Value = _tPid->fCtrl_Out;                            /* 记录数值，为下次PID做准备 */

    /* 存放过去两次误差值 */
    _tPid->fError[2] = _tPid->fError[1];
    _tPid->fError[1] = _tPid->fError[0];

    return _tPid->fCtrl_Out;
}

/**
 * @brief PID参数清空函数
 * @param _tPid-PID结构体指针
 * @retval Null
*/
void Algo_PID_Clear(tagPID_T *_tPid)
{
	/* 当前误差清零 */
    _tPid->fError[0] = _tPid->fError[1] = _tPid->fError[2] = 0.0f;

    /* 微分项清零 */
    _tPid->fDbuf[0] = _tPid->fDbuf[1] = _tPid->fDbuf[2] = 0.0f;

    /* 输出清零 */
    _tPid->fCtrl_Out = _tPid->fPout = _tPid->fIout = _tPid->fDout = 0.0f;

    /* 目标值和当前值清零 */
    _tPid->fCurr_Value = _tPid->fExp_Value = 0.0f;
}

/**
 * @brief PID初始函数
 * @param _tPid-PID结构体指针
 * @param _faPID-PID系数，要求存放顺序为P,I,D
 * @retval Null
*/
void Algo_PID_Init(tagPID_T *_tPid)
{
    /* 参数有误 */
    if(_tPid == NULL)
    {
        Drv_HAL_Error(__FILE__,__LINE__);
        while(1);
    }

    /* 初始化各个参数 */
    _tPid->fDbuf[0] = _tPid->fDbuf[1] = _tPid->fDbuf[2] = 0.0f;
    _tPid->fError[0] = _tPid->fError[1] = _tPid->fError[2] = _tPid->fPout = _tPid->fIout = _tPid->fDout = _tPid->fCtrl_Out = 0.0f;
}

