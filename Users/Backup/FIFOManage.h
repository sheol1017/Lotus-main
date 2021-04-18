#ifndef _MESSAGE_H
#define _MESSAGE_H

#include "stm8s.h"
#include <stdio.h>
#include <string.h>
// #include <stdlib.h>


#define MESS_ARRAY_LEN       32

#define MESS_USABLE          0x00
#define MESS_UNUSABLE        0x01

#define COMMON_MESS          0
#define SYSTEM_MESS          1
#define USER_MESS            2

typedef struct TagMessageDef
{
    u16 priority:2;
    u8  Messable:1;
    u8  MessData:5;
}MessageDef;

static MessageDef MessageArray[MESS_ARRAY_LEN] = { 0 };

u8 app_get_msg(void);

void put_msg_fifo(u8 msg);

void put_msg_lifo(u8 msg);

void flush_all_msg(void);

#endif 