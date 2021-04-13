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
    POINTNUM_ONE,    //ָ����������
    POINTNUM_LOOP,   //ָ������ѭ������
    POINTNUM_FOLDER, //ָ���ļ�����
    POINTNUM_MAX,
};

enum
{
    UDISK = 0,
    TFCARD,
    PC,
    bFLASH,
};

//�����ż�ֵ

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
    ERROR_Busy, //����æ
    ERROR_SLEEP,
    ERROR_RecvError,    //���ش��ڽ��մ���
    ERROR_CheckSError,  //����У�����
    ERROR_FolPointOver, //����ָ���ļ��������[���ݳ���Χ]
    ERROR_FolPointNo,   //δ�ҵ�ָ�����ļ�
};

/************************* �� �� �� �� �� **************************/
extern u8 UartRecvStatus;
extern u8 PlayledStatus;
extern u8 UartRecv_100Ms;
extern u8 UartRecvACK;
extern u8 System_Status;
extern u8 FEEDBACK; //�Ƿ���Ҫ����[=1:ҪӦ��][=0:��ҪӦ��]
extern u8 OutTimeCnt;
extern u8 ResendCount;   //�ط�����
extern u8 PlayledCnt;    //����led������ʱ��
extern u8 CurrentEQ;     //��ǰ��EQ
extern u8 PlayMode;      //��ǰ�Ĳ���ģʽ
extern u8 input_vol;     //��ǰ������
extern u8 PlayDevice;    //��ǰ���ŵ��豸
extern u8 OnlineDevice;  //��⵽ģ�����ߵ��豸
extern u8 CurrentModule; //��ǰ��ģ���ͺ�
extern u8 SysReturnTime;
extern u8 QueryACKTime; //�ȴ�Ӧ���ʱ
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

/************************* �� �� �� �� �� **************************/

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
