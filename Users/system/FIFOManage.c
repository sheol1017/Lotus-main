#include "config.h"
#include <stdio.h>
#include "FIFOManage.h"
#include "stm8s.h"
/*****************************************************************************************************
 - 功能描述：系统消息优先与用户消息
 - 参数说明：无
 - 返回参数：无
 - 注：
*****************************************************************************************************/
u8 app_get_msg(void) 
{
    u8 i = MESS_ARRAY_LEN -1;
    while(i >= 0)
    {
        if (MessageArray[i].priority == SYSTEM_MESS && 
            MessageArray[i].Messable == MESS_UNUSABLE)/*系统消息后进先出*/
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
            MessageArray[i].Messable == MESS_UNUSABLE)/*用户消息先进先出*/
        {
            MessageArray[i].Messable = MESS_USABLE;
 //          return ((u8)(MessageArray[i]));
        }
        i++;
    }  
    return NULL;
}


/*****************************************************************************************************
 - 功能描述：低优先级先进先出
 - 参数说明：无
 - 返回参数：无
 - 注：
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
 - 功能描述：高优先级为后进先出
 - 参数说明：无
 - 返回参数：无
 - 注：
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

