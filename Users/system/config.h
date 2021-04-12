#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "stm8s.h"
// #include "STK6037.h"
// #include "key.h"
#include "mylib.h"
#include "SystemInit.h"
// #include "uart.h"
// #include "LCD1602.h"
#include "MSG.h"



#define VER 0x10

/**********************************************************
                    系统调试相关设置
 - 、通过将FUNC_UARTDBG_EN这条宏设置为0或者1，来选择
     是否打开调试信息
**********************************************************/
// #define  BAUD_NUM                   96//384//1152
// #define  FUNC_UARTDBG_EN	        0//串口调试,烧芯片之前必须设置为[占560 byte]

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

#define QUERYACKTIME 20
#define SYSRETURNTIME 20

#endif