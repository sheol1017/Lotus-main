#ifndef __EXTENDKEY_H__
#define __EXTENDKEY_H__
/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "config.h"
// #ifdef ExtendKeys
/* define ------------------------------------------------------------*/
#define EXTEND_KEY_POWER GPIO_PIN_4     //JX4
#define EXTEND_KEY_PLUS GPIO_PIN_3      //JX5
#define EXTEND_KEY_MINUS GPIO_PIN_2     //JX6

#define EXTEND_KEY_MINUS_H() GPIOC->ODR |= (uint8_t)GPIO_PIN_2;
#define EXTEND_KEY_MINUS_L() GPIOC->ODR &= (uint8_t)(~GPIO_PIN_2)
/* Private typedef -----------------------------------------------------------*/


/* Output functions ---------------------------------------------------------*/
void ExtendKey_Init(void);
u8 Read_Extend_Key(GPIO_Pin_TypeDef pin);


// #endif // ExtendKeys
#endif