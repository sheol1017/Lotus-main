#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "stm8s.h"
#include "key.h"
#include "extendkeys.h"
#include "mylib.h"
#include "SystemInit.h"
#include "Mp3Module.h"
#include "LCD1602.h"
#include "WS2812B.h"
#include "MSG.h"
#include "LedSync.h"
#include "timebase.h"

// #define DEBUG
#define UARTREAD

#define VER 0x10
#define true TRUE 
#define false FALSE 
/**********************************************************
                    系统调试相关设置
 - 、通过将FUNC_UARTDBG_EN这条宏设置为0或者1，来选择
     是否打开调试信息
**********************************************************/
// #define  BAUD_NUM                   96//384//1152
#define  FUNC_UARTDBG_EN	        1//串口调试,烧芯片之前必须设置为[占560 byte]

#if FUNC_UARTDBG_EN
#define DBG(str) Uart_PutString(str)
#define Put_u8(x) Uart_PutChar(x)
#else
#define DBG(str)
#define Put_u8(x)
#endif

// #define TRUE               1
// #define FALSE              0

#define SENDLENTH 10    //发送数据总长度
#define MAX_MAIN_VOL 30 //最大音量
#define Middle_VOL 8 //初始音量
#define QUERYACKTIME 20
#define SYSRETURNTIME 20

/************************************************************************
          LED灯 --- 播放指示灯
*************************************************************************/
#define PLAYLEDCNT          6      //按键有效灯的闪烁时间
#define PLAYLED_ON		    GPIO_WriteHigh(GPIOB,GPIO_PIN_0)
#define PLAYLED_OFF		    GPIO_WriteLow(GPIOB,GPIO_PIN_0)
#define PLAYLED_FLASH	    GPIO_WriteReverse(GPIOB,GPIO_PIN_0)
#endif