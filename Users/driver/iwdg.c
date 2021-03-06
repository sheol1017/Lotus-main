/**
  **********************************  STM8S  ***********************************
  * @文件名     ： iwdg.c
  * @作者       ： Michael Shen
  * @库版本     ： V2.2.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2019年08月27日
  * @摘要       ： IWDG看门狗源文件
  ******************************************************************************/
/*----------------------------------------------------------------------------
  更新日志:
  2019-08-27 V1.0.0:初始版本
  ----------------------------------------------------------------------------*/
/* 包含的头文件 --------------------------------------------------------------*/
#include "iwdg.h"
#include "stm8s.h"
#include "stm8s_iwdg.h"

/************************************************
函数名称 ： IWDG_Initializes
功    能 ： IWDG始化
参    数 ： 无
返 回 值 ： 无
作    者 ： Michael Shen
*************************************************/
void IWDG_Initializes(void)
{
  IWDG_Enable();                                 //使能看门狗
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  //允许写分频值和重载值
  IWDG_SetPrescaler(IWDG_Prescaler_256);         //分频值 //独立看门口的时钟来源 内部低速时钟 128khz 除以2 即64khz / 64/256 =0.25 kHz 4ms周期
  IWDG_SetReload(250);                           //重载值(8位寄存器 最大255)
  IWDG_ReloadCounter();                          //重载计数值
}


/**** Copyright (C)2017 strongerHuang. All Rights Reserved **** END OF FILE ****/