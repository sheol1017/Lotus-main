#include "uart.h"
#include "stm8s_uart1.h"


/**
  * @brief  Deinitializes the UART peripheral.
  * @param  None
  * @retval None
	*/
void Uart_Init(void){
  // Deinitializes the UART peripheral.
  UART1_DeInit();
  UART1_HalfDuplexCmd(DISABLE);
  UART1_IrDACmd(DISABLE);
  UART1_LINCmd(DISABLE);
  UART1_SmartCardCmd(DISABLE);
  UART1_SmartCardNACKCmd(DISABLE);
}


/**
  * @brief  Initializes the UART1 according to the specified parameters.
  * @note   Configure in Push Pull or Open Drain mode the Tx pin by setting the
  *         correct I/O Port register according the product package and line
  *         configuration
  * @param  BaudRate: The baudrate.
  * @param  WordLength : This parameter can be any of the 
  *         @ref UART1_WordLength_TypeDef enumeration.
  * @param  StopBits: This parameter can be any of the 
  *         @ref UART1_StopBits_TypeDef enumeration.
  * @param  Parity: This parameter can be any of the 
  *         @ref UART1_Parity_TypeDef enumeration.
  * @param  SyncMode: This parameter can be any of the 
  *         @ref UART1_SyncMode_TypeDef values.
  * @param  Mode: This parameter can be any of the @ref UART1_Mode_TypeDef values
  * @retval None
  */
void Uart_ParameterConfig(uint32_t BaudRate, UART1_WordLength_TypeDef WordLength, 
                UART1_StopBits_TypeDef StopBits, UART1_Parity_TypeDef Parity, 
                UART1_SyncMode_TypeDef SyncMode, UART1_Mode_TypeDef Mode)
{
  UART1_Init(BaudRate, WordLength, StopBits, Parity, SyncMode, Mode);
}


void Uart_Enable(FunctionalState NewState){
  UART1_Cmd(NewState);
}


/**
  * @brief  Enables or disables the specified USART interrupts.
  * @param  UART1_IT specifies the USART interrupt sources to be enabled or disabled.
  *         This parameter can be one of the following values:
  *         - UART1_IT_TXE:  Transmit Data Register empty interrupt
  *         - UART1_IT_TC:   Transmission complete interrupt
  *         - UART1_IT_RXNE_OR: Receive Data register not empty and Overrun interrupt
  *         - UART1_IT_IDLE: Idle line detection interrupt
  *         - USRT1_IT_ERR:  Error interrupt
  * @param  NewState new state of the specified USART interrupts.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_InterruptConfig(UART1_IT_TypeDef UART1_IT, FunctionalState NewState){
  UART1_ITConfig(UART1_IT, NewState);
}

/**
  * @brief  Checks whether the specified UART1 flag is set or not.
  * @param  UART1_FLAG specifies the flag to check.
  *         This parameter can be any of the @ref UART1_Flag_TypeDef enumeration.
  * @retval FlagStatus (SET or RESET)
  */
FlagStatus UART_GetFlagStatus(UART1_Flag_TypeDef UART1_FLAG)
{
    return UART1_GetFlagStatus(UART1_FLAG);
}


void Uart_SendData(uint8_t Data){
  UART1_SendData8(Data);
}


uint8_t Uart_ReadData(void){
  uint8_t ucReturnValue = UART1_ReceiveData8();
  return ucReturnValue;
}


