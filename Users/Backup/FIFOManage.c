#include "config.h"
#include <stdio.h>
#include "FIFOManage.h"
#include "stm8s.h"
/*****************************************************************************************************
 - ����������ϵͳ��Ϣ�������û���Ϣ
 - ����˵������
 - ���ز�������
 - ע��
*****************************************************************************************************/
u8 app_get_msg(void) 
{
    u8 i = MESS_ARRAY_LEN -1;
    while(i >= 0)
    {
        if (MessageArray[i].priority == SYSTEM_MESS && 
            MessageArray[i].Messable == MESS_UNUSABLE)/*ϵͳ��Ϣ����ȳ�*/
        {  
            MessageArray[i].Messable = MESS_USABLE;
//            return ((u8)(MessageArray[i]));
        }
        i--;
    }
    i  = 0;
    while(i < MESS_ARRAY_LEN)
    {
        if (MessageArray[i].priority == USER_MESS &&
            MessageArray[i].Messable == MESS_UNUSABLE)/*�û���Ϣ�Ƚ��ȳ�*/
        {
            MessageArray[i].Messable = MESS_USABLE;
 //          return ((u8)(MessageArray[i]));
        }
        i++;
    }  
    return NULL;
}


/*****************************************************************************************************
 - ���������������ȼ��Ƚ��ȳ�
 - ����˵������
 - ���ز�������
 - ע��
*****************************************************************************************************/
void put_msg_fifo(u8 msg)
{
    u8 i = 0;
    while(i < MESS_ARRAY_LEN)
    {
        if(MessageArray[i].Messable == MESS_USABLE)
        {
            memcpy(&MessageArray[i], &msg, sizeof(MessageDef));
            MessageArray[i].priority = USER_MESS;
            MessageArray[i].Messable = MESS_UNUSABLE;
        }
        i++;
    }  
}


/*****************************************************************************************************
 - ���������������ȼ�Ϊ����ȳ�
 - ����˵������
 - ���ز�������
 - ע��
*****************************************************************************************************/
void put_msg_lifo(u8 msg)
{
    u16 i = MESS_ARRAY_LEN -1;
    while(i >= 0)
    {
        if(MessageArray[i].Messable == MESS_USABLE)
        {
            memcpy(&MessageArray[i], &msg, sizeof(MessageDef));   
            MessageArray[i].Messable = MESS_UNUSABLE;
            MessageArray[i].priority = SYSTEM_MESS;
        }
        i--;
    }
}

void flush_all_msg(void)
{
    memset(MessageArray, 0, sizeof(MessageArray));
}

