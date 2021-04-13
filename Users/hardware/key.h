#ifndef __KEY_H__
#define __KEY_H__
#include "config.h"
#include "stm8s.h"
#include "stm8s_gpio.h"
enum
{
    SYS_STATUS_IDLE = 0,
    SYS_STATUS_INIT,
    SYS_STATUS_WORK,
    SYS_STATUS_SLEEP,
};

enum //
{
    EQ_NORMAL = 0,
    EQ_POP,
    EQ_ROCK,
    EQ_JAZZ,
    EQ_CLASSIC,
    EQ_BASS,
};

enum
{
    REPEAT_ALL = 0,
    REPEAT_FOLDER,
    REPEAT_ONE,
    REPEAT_RANDOM,
    MAX_PLAY_MODE,
};

enum
{
    PLAYDEVICE_NULL = 0,
    PLAYDEVICE_UDISK = 1,
    PLAYDEVICE_TFCARD,
    PLAYDEVICE_PC,
    PLAYDEVICE_FLASH,
    PLAYDEVICE_SLEEP,
    PLAYDEVICE_MAX,
};

enum
{
    ONLINE_NULL = 0,
    ONLINE_UDIDK = 0x01,
    ONLINE_TFCARD = 0x02,
    ONLINE_PC = 0x04,
    ONLINE_FLASH = 0x08,
};

enum
{
    POINTNUM_NULL = 0,
    POINTNUM_ONE,    //指定单曲播放
    POINTNUM_LOOP,   //指定单曲循环播放
    POINTNUM_FOLDER, //指定文件播放
    POINTNUM_MAX,
};

enum
{
    UDISK = 0,
    TFCARD,
    PC,
    bFLASH,
};

//按键门槛值

#define KEY_BASE_CNT 4
#define KEY_LONG_CNT 75
#define KEY_HOLD_CNT 15

enum
{
    KEY_SHORT_UP,
    KEY_LONG,
    KEY_HOLD,
    KEY_LONG_UP,
};

#define KEYIO GPIO_ReadInputData(GPIOC)
#define NO_KEY 0xff

enum
{
    KEY_NULL = 0,
    KEY_VALUE1 = 1,
    KEY_VALUE2 = 2,
    KEY_VALUE3 = 3,
    KEY_VALUE4 = 4,
    KEY_VALUE5 = 5,
    KEY_VALUE6 = 6
};

enum
{
    SET_PLAYLED_NONE,
    SET_PLAYLED_ON,
    SET_PLAYLED_OFF,
    SET_PLAYLED_100MS_FLASH,
    SET_PLAYLED_500MS_FLASH,
    SET_PLAYLED_900MS_ON_100MS_OFF,
    SET_PLAYLED_100MS_ON_900MS_OFF,
};

enum
{
    ERROR_IDLE = 0,
    ERROR_Busy, //返回忙
    ERROR_SLEEP,
    ERROR_RecvError,    //返回串口接收错误
    ERROR_CheckSError,  //返回校验出错
    ERROR_FolPointOver, //返回指定文件输入错误[数据超范围]
    ERROR_FolPointNo,   //未找到指定的文件
};

/************************* 对 外 的 变 量 **************************/
extern u8 UartRecvStatus;
extern u8 PlayledStatus;
extern u8 UartRecv_100Ms;
extern u8 UartRecvACK;
extern u8 System_Status;
extern u8 FEEDBACK; //是否需要反馈[=1:要应答][=0:不要应答]
extern u8 OutTimeCnt;
extern u8 ResendCount;   //重发次数
extern u8 PlayledCnt;    //测试led点亮的时间
extern u8 CurrentEQ;     //当前的EQ
extern u8 PlayMode;      //当前的播放模式
extern u8 input_vol;     //当前的音量
extern u8 PlayDevice;    //当前播放的设备
extern u8 OnlineDevice;  //检测到模块在线的设备
extern u8 CurrentModule; //当前的模块型号
extern u8 SysReturnTime;
extern u8 QueryACKTime; //等待应答计时
extern u8 DelayTime;
extern u8 ucErrorStatus;
extern u8 PointFunc;

extern u16 input_number;
extern u16 CurDevTotal;
extern u16 TFTotal;
extern u16 UDiskTotal;
extern u16 FlashTotal;

extern u16 CurDevNUM;
extern u16 UDiskCurFile;
extern u16 TFCurFile;
extern u16 FlashCurFile;

/************************* 对 外 的 接 口 **************************/

void Time_Task(void);
void key_scan(void);
void Key_Task(void);
void Indicator_Scan(void);
void MSG_Task(void);
void put_msg_lifo(u8 msg);
u8 app_get_msg(void);
void GoInitDevice(u8 Online) ;
void GotoNextDevice(u8 device) ;
void ChangeDevice(u8 dev);

#endif
