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
                    ϵͳ�����������
 - ��ͨ����FUNC_UARTDBG_EN����������Ϊ0����1����ѡ��
     �Ƿ�򿪵�����Ϣ
**********************************************************/
// #define  BAUD_NUM                   96//384//1152
// #define  FUNC_UARTDBG_EN	        0//���ڵ���,��оƬ֮ǰ��������Ϊ[ռ560 byte]

#if FUNC_UARTDBG_EN
#define DBG(str) Uart_PutString(str)
#define Put_u8(x) Uart_PutChar(x)
#else
#define DBG(str)
#define Put_u8(x)
#endif

// #define TRUE               1
// #define FALSE              0

#define SENDLENTH 10    //���������ܳ���
#define MAX_MAIN_VOL 30 //�������

#define QUERYACKTIME 20
#define SYSRETURNTIME 20

#endif