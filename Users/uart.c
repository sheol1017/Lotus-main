#include "uart.h"
#include "stm8s_UART2.h"


/**
  * @brief  Deinitializes the UART peripheral.
  * @param  None
  * @retval None
	*/
void Uart_Init(void){
  // Deinitializes the UART peripheral.
  UART2_DeInit();
  // UART2_HalfDuplexCmd(DISABLE);
  UART2_IrDACmd(DISABLE);
  UART2_LINCmd(DISABLE);
  UART2_SmartCardCmd(DISABLE);
  UART2_SmartCardNACKCmd(DISABLE);
}


/**
  * @brief  Initializes the UART2 according to the specified parameters.
  * @note   Configure in Push Pull or Open Drain mode the Tx pin by setting the
  *         correct I/O Port register according the product package and line
  *         configuration
  * @param  BaudRate: The baudrate.
  * @param  WordLength : This parameter can be any of the 
  *         @ref UART2_WordLength_TypeDef enumeration.
  * @param  StopBits: This parameter can be any of the 
  *         @ref UART2_StopBits_TypeDef enumeration.
  * @param  Parity: This parameter can be any of the 
  *         @ref UART2_Parity_TypeDef enumeration.
  * @param  SyncMode: This parameter can be any of the 
  *         @ref UART2_SyncMode_TypeDef values.
  * @param  Mode: This parameter can be any of the @ref UART2_Mode_TypeDef values
  * @retval None
  */
void Uart_ParameterConfig(uint32_t BaudRate, UART2_WordLength_TypeDef WordLength, 
                UART2_StopBits_TypeDef StopBits, UART2_Parity_TypeDef Parity, 
                UART2_SyncMode_TypeDef SyncMode, UART2_Mode_TypeDef Mode)
{
  UART2_Init(BaudRate, WordLength, StopBits, Parity, SyncMode, Mode);
}


void Uart_Enable(FunctionalState NewState){
  UART2_Cmd(NewState);
}


/**
  * @brief  Enables or disables the specified USART interrupts.
  * @param  UART2_IT specifies the USART interrupt sources to be enabled or disabled.
  *         This parameter can be one of the following values:
  *         - UART2_IT_TXE:  Transmit Data Register empty interrupt
  *         - UART2_IT_TC:   Transmission complete interrupt
  *         - UART2_IT_RXNE_OR: Receive Data register not empty and Overrun interrupt
  *         - UART2_IT_IDLE: Idle line detection interrupt
  *         - USRT1_IT_ERR:  Error interrupt
  * @param  NewState new state of the specified USART interrupts.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_InterruptConfig(UART2_IT_TypeDef UART2_IT, FunctionalState NewState){
  UART2_ITConfig(UART2_IT, NewState);
}

/**
  * @brief  Checks whether the specified UART2 flag is set or not.
  * @param  UART2_FLAG specifies the flag to check.
  *         This parameter can be any of the @ref UART2_Flag_TypeDef enumeration.
  * @retval FlagStatus (SET or RESET)
  */
FlagStatus UART_GetFlagStatus(UART2_Flag_TypeDef UART2_FLAG)
{
    return UART2_GetFlagStatus(UART2_FLAG);
}


void Uart_SendData(uint8_t Data){
  UART2_SendData8(Data);
}


uint8_t Uart_ReadData(void){
  uint8_t ucReturnValue = UART2_ReceiveData8();
  return ucReturnValue;
}


