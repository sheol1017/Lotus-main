#ifndef __UART_H__
#define __UART_H__

#include "stm8s.h"
#include "stm8s_uart2.h"

#define ON  1
#define OFF 0


void Uart_Init(void);
void Uart_ParameterConfig(uint32_t BaudRate, UART2_WordLength_TypeDef WordLength, 
                UART2_StopBits_TypeDef StopBits, UART2_Parity_TypeDef Parity, 
                UART2_SyncMode_TypeDef SyncMode, UART2_Mode_TypeDef Mode);
void Uart_Enable(FunctionalState NewState);
void UART_InterruptConfig(UART2_IT_TypeDef UART2_IT, FunctionalState NewState);
FlagStatus UART_GetFlagStatus(UART2_Flag_TypeDef UART2_FLAG);

void Uart_SendData(uint8_t Data);
uint8_t Uart_ReadData(void);



// void Uart_SetPwrState(bool bState);
// bool Uart_bPowerState(void);
// void Uart_SetPowr(u8 ucPower);
// u8 Uart_GetPwr(void);
// bool Uart_bRequriredSystemWorkState(void);
// bool Uart_bRequriredFanState(void);
// bool Uart_bRequriredPowerModuleState(void);
// bool Uart_bIGBT2TemperatureLimitLevel2(void);
// bool Uart_bIGBT2TemperatureLimitLevel1(void);
// bool Uart_bIGBT1TemperatureLimitLevel2(void);
// bool Uart_bIGBT1TemperatureLimitLevel1(void);
// bool Uart_bPotTemperatureOverLimit(void);
// u8 Uart_ucPotTemperature(void);
// u8 Uart_ucIGBT1_Temperature(void);
// u8 Uart_ucIGBT2_Temperature(void);
// u8 Uart_ucErrorInfo(void);
// u16 Uart_uiGetSpecifiedData(void);
// u8 Uart_ucGetVoltageCorrectionRefValue(void);
// u8 Uart_ucGetVoltageCorrectionRatValue(void);
// u8 Uart_ucGetPowerCorrectionFactor(void);
// u8 Uart_ucGetStoredInfomationAddr(void);


#endif //#ifndef __UART_H__