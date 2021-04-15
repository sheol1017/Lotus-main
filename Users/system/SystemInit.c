#include "config.h"
#include "stm8s.h"
/******************************************************************************************************************
        
			+--------------------------------------------------+
		                      初始化+中断处理部分
		    +--------------------------------------------------+

 - 实现功能：

 - 目前进展：

 - 备注    ：
******************************************************************************************************************/

void RAM_Init(void) ;
/*****************************************************************************************************
 - 功能描述：定时器0的初始化
 - 参数说明：无
 - 返回参数：无												  
 - 注：		 
*****************************************************************************************************/
void Timer0_Init(void)
{
	// TMOD |= 0x01; 
	// EA = 1 ;
	// ET0= 1 ;
	// TR0= 1 ;
	// TH0= TIMER0_H;
	// TL0= TIMER0_L;
}

/*****************************************************************************************************
 - 功能描述： 串口1初始化
 - 隶属模块： 外部
 - 参数说明： 无
 - 返回说明： 无
 - 注：	      都是9600波特率
*****************************************************************************************************/
void Serial_init(void)
{
	// TMOD = 0x20;                // 设置 T1 为波特率发生器
	// SCON = 0x50;                // 0101,0000 8位数据位, 无奇偶校验
				   		
	// PCON = 0x00;                //PCON=0;

	// TH1=256-(SYS_Fosc/COMM_BAUD_RATE/32/12);//设置为9600波特率
	// TL1=256-(SYS_Fosc/COMM_BAUD_RATE/32/12);

    // TR1     = 1; 			   //定时器1打开
    // REN     = 1;			   //串口1接收使能
    // ES      = 1;			   //串口1中断使能
}

/*****************************************************************************************************
 - 功能描述：初始化
 - 参数说明：无
 - 返回参数：无
 - 注：无
*****************************************************************************************************/
void System_Init(void)
{
	Serial_init() ;
	Timer0_Init() ;
	RAM_Init() ;
	Key_Init();
    itoa1(0);
    itoa2(0);
    itoa3(0);
    itoa4(0);
    TrigEnable = 1 ;
    // PointFunc = POINTNUM_ONE ;
    // input_vol = MAX_MAIN_VOL ;//上电默认最大音量
    // System_Status = SYS_STATUS_WORK ;
    // SysReturnTime = 30 ;//定义3S的时候等待模块初始化完成
}

/*****************************************************************************************************
 - 功能描述： 按键的初始化
 - 隶属模块： 外部
 - 参数说明： 无
 - 返回说明： 无
 - 注：
*****************************************************************************************************/
void RAM_Init(void)
{
	System_Flag_2Ms    = 0 ;
	System_Flag_10Ms   = 0 ;
	System_Flag_100Ms  = 0 ;
	System_Flag_500Ms	   = 0 ;
	RecvOver_Flag	   = 0 ;
	Resend_Flag_Enable = 0 ;
	MainEnable  = TRUE ;  
    // SysReturnTime = 0 ;
    // PlayledCnt = 0 ;
    // PlayledStatus = 0;
    // PlayledStatus = SET_PLAYLED_OFF;
}

/*****************************************************************************************************
 - 功能描述：定时器0中断(任务调度)
 - 参数说明：无
 - 返回参数：无
 - 注：      该定时器起到系统调度的作用，产生一个2ms的时基。详见mylib.c文件
*****************************************************************************************************/
// void Timer_0_interrupt(void) interrupt  1
// {
// 	TH0= TIMER0_H ;
// 	TL0= TIMER0_L ;
// 	System_Flag_2Ms = 1 ;
// }

