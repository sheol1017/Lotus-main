#ifndef __TIMEBASE_H__
#define __TIMEBASE_H__

#include "stm8s.h"




void TimeBase_Init(void);
void TimeBase_IncerementCounter_UPD_IRQ(void);
void TimeBase_HandleTimeBaseCouter(void);
//
u8 TimeBase_Get1msSystemTimeDelta(void);
u8 TimeBase_Get10msSystemTimeDelta(void);
u8 TimeBase_Get250msSystemTimeDelta(void);
u8 TimeBase_Get1sSystemTimeDelta(void);



#endif //#ifndef __TIMEBASE_H__