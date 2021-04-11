/**
  ******************************************************************************
  * @file    Mp3ModuleControl.c
  * @author  Albert
  * @version V0.0.0
  * @date    2021-4
  * @brief   This file is used to control mp3 module: MP3-TF-16P
  ******************************************************************************
**/

/** include  ------------------------------------------------------------------*/

#include "uart.h"
#include "Mp3ModuleControl.h"

/** Private typedef -----------------------------------------------------------*/

/** Private define ------------------------------------------------------------*/

#define START_BYTE  0x7E
#define END_BYTE    0xEF

/** Private variables ---------------------------------------------------------*/

static u8 ucSendBuff[10];

/** export variables ----------------------------------------------------------*/

/** Private function prototypes -----------------------------------------------*/

static void MMC_DoSum( u8 *Str, u8 len);
static void MMC_SendData(u8 ucLen);


/** Private functions ---------------------------------------------------------*/

/******************************************************************************
- 功能描述：求和校验
- 和校验的思路如下：
发送的指令，去掉起始和结束。将中间的6 个字节进行累加，最后取反码。接收端就将
接收到的一帧数据，去掉起始和结束。将中间的数据累加，再加上接收到的校验字节。刚好
为0.这样就代表接收到的数据完全正确。
******************************************************************************/
static void MMC_DoSum(u8 *Str, u8 len)
{
    u16 xorsum = 0;
    u8 i;
    for(i=1; i<len+1; i++)
    {
        xorsum = xorsum + Str[i];   
        // Str = Str + i;
        // xorsum = xorsum + *Str;
    }
    xorsum = 0 -xorsum;
    *(Str+i) = (u8)(xorsum >>8);
    *(Str+i+1) = (u8)(xorsum & 0x00ff);
}

static void MMC_SendData(u8 ucLen)
{
    u8 i;
    for(i=0;i<ucLen;i++)
    {
        Uart_SendData(ucSendBuff[i]);
        while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
    }
}

/** Export functions -- -------------------------------------------------------*/

void MMC_Init(void)
{
    // init uart1
    Uart_Init();

    // config uart1
    // baud rate: 9600 bps
    // data bit: 8
    // stop bit: 1
    // No Parity
    // sync mode disbled
    // Rx and Tx are enabled
    Uart_ParameterConfig(9600,UART1_WORDLENGTH_8D,UART1_STOPBITS_1,
                            UART1_PARITY_NO,UART1_SYNCMODE_CLOCK_DISABLE,
                            UART1_MODE_TXRX_ENABLE);

    // disable uart interrupt
    // UART_InterruptConfig(UART1_IT_TXE,DISABLE);
    UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);

    // Enable uart
    Uart_Enable(ENABLE);

    // set start byte
    ucSendBuff[0] = START_BYTE;
    // set end byte
    ucSendBuff[9] = END_BYTE;

    // GPIO init
    // PD5 UART TX 
    GPIO_Init(GPIOD , GPIO_PIN_5 , GPIO_MODE_OUT_PP_HIGH_FAST); 
    // PD6 UART RX
    GPIO_Init(GPIOD , GPIO_PIN_6 , GPIO_MODE_IN_PU_NO_IT); 
}

void Uart_ReadByte_RX_IRQ(void)
{
    //
}

void MMC_SendCMD(u8 CMD ,u8 feedback , u16 dat)
{
    ucSendBuff[0] = START_BYTE;     // set start byte
    ucSendBuff[1] = 0xff;           // 保留字节
    ucSendBuff[2] = 0x06;           // 长度
    ucSendBuff[3] = CMD;            // 控制指令
    ucSendBuff[4] = feedback;       // 是否需要反馈
    ucSendBuff[5] = (u8)(dat >> 8); //datah
    ucSendBuff[6] = (u8)(dat);      //datal

    MMC_DoSum(ucSendBuff,6);        //校验

    ucSendBuff[9] = END_BYTE;       // set end byte

    // SendCmd(8); //发送此帧数据
    MMC_SendData(10);                // 发送此帧数据
}

void MMC_Play(void)
{
    MMC_SendCMD(0x0D,0,0);
}

void MMC_Pause(void)
{
    MMC_SendCMD(0x0E,0,0);
}

void MMC_Voice2middle(void)
{
    MMC_SendCMD(0x06,0,10);
}

void MMC_Play_Num_spec(u16 playlist_num)
{
    MMC_SendCMD(0x12,0,playlist_num);
}