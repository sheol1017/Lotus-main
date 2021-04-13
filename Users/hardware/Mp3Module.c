#include "config.h"

/******************************************************************************************************************

					+--------------------------------------------------+
					                   ����ͨѶģ��
					+--------------------------------------------------+

 - ʵ�ֹ��ܣ�

 - Ŀǰ��չ��

 - ���л�����STC   ����11.0592M   ������:9600   [Ӳ�����ڲ���]

 - ��ע    �������пƼ���51�������ϵ���OK --- STC89C516RD+

******************************************************************************************************************/

static u8 Send_buf[SENDLENTH] = {0} ;
static u8 Recv_buf[SENDLENTH] = {0} ;

static u8 SendDataLen = 0 ;
static u8 ResendDataLen = 0 ;

const u8 hex[]={"0123456789ABCDEF"};

void DoSum( u8 *Str, u8 len) ;
void Uart_Task(u8 *pr);
/********************************************************************************************
 - ���������� ���ڷ���һ���ֽ�
 - ����ģ�飺 �ⲿ
 - ����˵����
 - ����˵����
 - ע��	      
********************************************************************************************/
void Uart_PutByte(u8 ch)
{
    // SBUF  = ch;
    // while(!TI){;}
    // TI = 0;
}

/********************************************************************************************
 - ���������� ���ڷ����ַ���
 - ����ģ�飺 �ⲿ
 - ����˵����
 - ����˵����
 - ע��	      
********************************************************************************************/
#if FUNC_UARTDBG_EN
void Uart_PutString(u8* value)
{
	while((*value) != '\0')
	{
		Uart_PutByte(*value) ;
		value ++ ;
	}
}
#endif
/********************************************************************************************
 - ���������� ���ڷ���һ���ֽ�[������]
 - ����ģ�飺 �ⲿ
 - ����˵����
 - ����˵����
 - ע��	      
*******************************************************************************************/
#if FUNC_UARTDBG_EN
void Uart_PutChar(u8 dat)
{
	u8 ch = 0 ;
	ch =(dat>>4) & 0x0f;
	Uart_PutByte(hex[ch]);
	ch = dat & 0x0f;
	Uart_PutByte(hex[ch]);
}
#endif

/*****************************************************************************************************
 - ���������� ���ڷ�������
 - ����ģ�飺 �ڲ� 
 - ����˵���� 
 - ����˵���� 
 - ע����     
*****************************************************************************************************/
void SendCmd(u8 len,bool flag,u8 time)
{
    bool Status = 0 ;
    u8 i = 0 ;

    // Status   = ES;
    // ES       = 0;
    // TI       = 0;
    Uart_PutByte(0x7E); //��ʼ
    for(i=0; i<len; i++)//����
    {
		Uart_PutByte(Send_buf[i]) ;
    }
    Uart_PutByte(0xEF) ;//����
    // ES  = Status;
    if(flag)                  //�����ط���־  ���ʱ��
    {
        Resend_Flag  = 1; SendDataLen = len;  OutTimeCnt = time;
    }
    else
	{
        Resend_Flag  = 0; SendDataLen = 0; 	  ResendCount  = 1;
	}
}

/*******************************************************************************************
 - ���������������ж�
 - ����ģ�飺
 - ����˵����
 - ����˵����
 - ע��	    ���ڽ����ǲ���״̬���ķ�ʽ
*******************************************************************************************/
void Serial_0_interrupt_IRQ()
{
    
	u8 UartTemp = 0 ;
	if(UART1_GetFlagStatus(UART1_FLAG_RXNE))
	{	
		// RI = 0;
		// UartTemp  = SBUF;
        if(Busy_Flag)
        {
            RecvBusy_Flag = 1 ;
        }
        switch(UartRecvStatus)
        {
            case UART_RECV_IDLE:
                                if(0x7E == UartTemp)
                                {
                                    UartRecv_100Ms = 10 ;
                                    UartRecvStatus = UART_RECV_VER ;
                                }
                                else
                                {
                                    UartRecvStatus = UART_RECV_IDLE ;
                                }
                                break ;
            case UART_RECV_VER:
                                Recv_buf[0] = UartTemp ;
                                UartRecvStatus = UART_RECV_LENTH ;
                                break ;
            case UART_RECV_LENTH:
                                Recv_buf[1] = UartTemp ;
                                UartRecvStatus = UART_RECV_CMD ;
                                break ;
            case UART_RECV_CMD:
                                Recv_buf[2] = UartTemp ;
                                UartRecvStatus = UART_RECV_FEEDBACK ;
                                break ;
            case UART_RECV_FEEDBACK:
                                Recv_buf[3] = UartTemp ;
                                UartRecvStatus = UART_RECV_DATAH ;
                                break ;
            case UART_RECV_DATAH:
                                Recv_buf[4] = UartTemp ;
                                UartRecvStatus = UART_RECV_DATAL ;
                                break ;
            case UART_RECV_DATAL:
                                Recv_buf[5] = UartTemp ;
                                UartRecvStatus = UART_RECV_CHECKSUMH ;
                                break ;
            case UART_RECV_CHECKSUMH:
                                Recv_buf[6] = UartTemp ;
                                UartRecvStatus = UART_RECV_CHECKSUML ;
                                break ;
            case UART_RECV_CHECKSUML:
                                Recv_buf[7] = UartTemp ;
                                UartRecvStatus = UART_RECV_OVER ;
                                break ;
            case UART_RECV_OVER :
                                if(0xEF == UartTemp)
                                {
                                    UartRecv_100Ms = 0 ;//���ʱ��
                                    RecvOver_Flag = 1 ;
                                    UartRecvStatus = UART_RECV_IDLE ;
                                }
                                else
                                {
                                    RecvError_Flag = 1 ;//������
                                    UartRecvStatus = UART_RECV_IDLE ;
                                }
                                break ;
            default :
                                break ;
        }
	}
}

/********************************************************************************************
 - ���������� �ط�������
 - ����ģ�飺 �ⲿ
 - ����˵����
 - ����˵����
 - ע��
********************************************************************************************/
void UartSend_Collide_Task(void)
{
    if(1 == Resend_Enable_Flag)
    {
        if(!Resend_Flag) return;	    //��� Resend_Flag1 = 1 ʱ�������û���յ�ACK ���ط�

        ResendCount ++;

    	if(ResendCount>2)
        {
    	    ResendCount  = 1;
            Resend_Flag  = 0;
            return;
        }
    	SendCmd(ResendDataLen,1,25);    //�ط�����
	}
}
/********************************************************************************************
 - �������������Խ��յ�������д���
 - ����ģ�飺�ⲿ
 - ����˵������
 - ����˵������
 - ע��      
********************************************************************************************/
void Uart_communication(void)
{
	u8 i, *pi ;

	u16  xorsum =0,xorsum1=0 ;

    if(1 == RecvError_Flag)//���մ�����
    {
        RecvError_Flag = 0 ;
        UartRecvStatus = UART_RECV_IDLE ;
        Uart_SendCMD(0x40 , 0 , 1) ;//�����������ط�
        return ;
    }
    if(1 == RecvOver_Flag)//һ֡���ݽ������
    {
        RecvOver_Flag = 0 ;
    	pi = Recv_buf;
    	for(i=0; i<(*(pi+1)); i++)
    	{
    	    xorsum  = xorsum + pi[i] ;
    	}
    	xorsum1 = ((u16)((*(pi+i))<<8)) | (*(pi+i+1));
    	xorsum  = xorsum + xorsum1;

    	if(!xorsum)
    	{
    	    Uart_Task(pi);//���ڴ���
    	}
        else
        {
            RecvError_Flag = 1 ;//������
        }
        UartRecvStatus = UART_RECV_IDLE ;
    }
}

/**********************************************************************************************
 - ���������� ���ڴ�����
 - ����ģ�飺 �ڲ�
 - ����˵���� *pr ������0��Ҫ���յ����ݵĵ�ַָ��
 - ����˵���� ��
 - ע��	
**********************************************************************************************/
void Uart_Task(u8 *pr)
{
    u16 Temp16 = 0 ;
    u8 *pi ,tempH ,tempL , CMD , FeedBack;		
    pi = pr;

    tempH = *(pi+4) ;
    tempL = *(pi+5) ;
    Temp16 = (((u16)(tempH))<<8)|tempL ;
    CMD   = *(pi+2) ;
    FeedBack = *(pi+3) ;

    switch(CMD )
    {
        case (0x41)://�յ�Ӧ��ACK
                    UartRecvACK = 0 ;
                    PlayledCnt = PLAYLEDCNT ;
            		PlayledStatus = SET_PLAYLED_ON;
                    break ;
    /**********************************************************************
    - 1���豸����γ���Ϣ
    **********************************************************************/   
        case (0x3A)://�豸����
                    if(tempL == 0x01)
                    {
                        put_msg_lifo(MSG_UDISK_IN);
                    }
                    else if(tempL == 0x02)
                    {
                        put_msg_lifo(MSG_TF_IN);                    
                    }
                    else if(tempL == 0x04)
                    {
                        put_msg_lifo(MSG_PC_IN);
                    }
                    break;
        case (0x3B)://�豸�γ�
                    if(tempL == 0x01)
                    {
                        put_msg_lifo(MSG_UDISK_OUT);
                    }
                    else if(tempL == 0x02)
                    {
                        put_msg_lifo(MSG_TF_OUT);                    
                    }
                    else if(tempL == 0x04)
                    {
                        put_msg_lifo(MSG_PC_OUT);
                    }
                    break;
                    
    /**********************************************************************
    - 1���յ���ǰ��Ŀ���������Ϣ
    **********************************************************************/   
        case (0x3C)://U�̵�ǰ��Ŀ�������
                    if(PLAYDEVICE_UDISK == PlayDevice)
                    {
                        put_msg_lifo(MSG_MUSIC_OVER);
                    }
                    break;
        case (0x3D)://TF��ǰ��Ŀ�������
                    if(PLAYDEVICE_TFCARD == PlayDevice)
                    {
                        put_msg_lifo(MSG_MUSIC_OVER);
                    }
                    break ;
        case (0x3E)://FLASH��ǰ��Ŀ�������
                    if(PLAYDEVICE_FLASH == PlayDevice)
                    {
                        put_msg_lifo(MSG_MUSIC_OVER);
                    }
                    break ;
    /**********************************************************************
    - 1��ģ���ϵ緵�ص�����
    **********************************************************************/  
        case (0x3F):
                    OnlineDevice = tempL ;
                    put_msg_lifo(MSG_RETURN_MINIT);
                    break ;

    /**********************************************************************
    - 1��ģ�鷵�صĴ���
    **********************************************************************/  
        case (0x40):
                    ucErrorStatus = tempL ;
                    put_msg_lifo(MSG_RECV_ERROR);
                    break ;
                    
    /**********************************************************************
    - 1����ѯ����
    **********************************************************************/
        case (0x42)://��ѯ��ǰ״̬
                  
                    break;
        case (0x43)://��ѯ��ǰϵͳ����
                    input_vol = tempL ;
                    put_msg_lifo(MSG_RETURN_VOL);  
                    break;
        case (0x44)://��ѯ��ǰEQ
                    CurrentEQ = tempL ;
                    put_msg_lifo(MSG_RETURN_EQ);  
                    break;
        case (0x45)://��ѯ��ǰ����ģʽ
                    PlayMode = tempL ;
                    put_msg_lifo(MSG_RETURN_PLAYMODE);  
                    break;
        case (0x46)://��ѯ��ǰ����汾

                    break;
    /**********************************************************************
    - 1����ѯ���� --- �豸���ļ���
    **********************************************************************/
        case (0x47)://��ѯUDISK�ļ�����
                    UDiskTotal = Temp16 ;
                    put_msg_lifo(MSG_RETURN_NUMTOTAL); 
                    break ;
        case (0x48)://��ѯTFCARD�ļ�����
                    TFTotal = Temp16 ;
                    put_msg_lifo(MSG_RETURN_NUMTOTAL);
                    break ;
        case (0x49)://��ѯFLASH�ļ�����
                    FlashTotal = Temp16 ;
                    put_msg_lifo(MSG_RETURN_NUMTOTAL); 
                    break;

    /**********************************************************************
    - 1����ѯ���� --- �豸��ǰ���ŵ��ļ���
    **********************************************************************/   
        case (0x4B)://��ѯUDISK�ĵ�ǰ��Ŀ
                    UDiskCurFile = Temp16 ;
                    put_msg_lifo(MSG_RETURN_CURNUM);
                    break;
    
        case (0x4C)://��ѯTF���ĵ�ǰ��Ŀ
                    TFCurFile = Temp16 ;
                    put_msg_lifo(MSG_RETURN_CURNUM);
                    break ;
                  
        case (0x4D)://��ѯFLASH�ĵ�ǰ��Ŀ
                    FlashCurFile = Temp16 ;
                    put_msg_lifo(MSG_RETURN_CURNUM);
                    break;
        default:
                    break;
    }
}



/********************************************************************************************
 - ���������� �������ⷢ������[�������ƺͲ�ѯ]
 - ����ģ�飺 �ⲿ
 - ����˵���� CMD:��ʾ����ָ������ָ�����������ѯ�����ָ��
              feedback:�Ƿ���ҪӦ��[0:����ҪӦ��1:��ҪӦ��]
              data:���͵Ĳ���
 - ����˵����
 - ע��       
********************************************************************************************/
void Uart_SendCMD(u8 CMD ,u8 feedback , u16 dat)
{
    Send_buf[0] = 0xff;    //�����ֽ� 
    Send_buf[1] = 0x06;    //����
    Send_buf[2] = CMD;     //����ָ��
    Send_buf[3] = feedback;//�Ƿ���Ҫ����
    Send_buf[4] = (u8)(dat >> 8);//datah
    Send_buf[5] = (u8)(dat);     //datal
    DoSum(&Send_buf[0],6);        //У��
    SendCmd(8,1,30);       //���ʹ�֡����
    UartRecvACK = 3 ;//�趨�ȴ�Ӧ���ʱ��[300ms]
}


/********************************************************************************************
 - �������������У��
 - ����ģ�飺
 - ����˵����
 - ����˵����
 - ע��      ��У���˼·����
             ���͵�ָ�ȥ����ʼ�ͽ��������м��6���ֽڽ����ۼӣ����ȡ����
             ���ն˾ͽ����յ���һ֡���ݣ�ȥ����ʼ�ͽ��������м�������ۼӣ��ټ��Ͻ��յ���У��
             �ֽڡ��պ�Ϊ0.�����ʹ�����յ���������ȫ��ȷ��
********************************************************************************************/
void DoSum( u8 *Str, u8 len)
{
    u16 xorsum = 0;
    u8 i;

    for(i=0; i<len; i++)
    {
        xorsum  = xorsum + Str[i];
    }
	xorsum     = 0 -xorsum;
	*(Str+i)   = (u8)(xorsum >>8);
	*(Str+i+1) = (u8)(xorsum & 0x00ff);
}

































































