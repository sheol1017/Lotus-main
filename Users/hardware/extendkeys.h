#ifndef __EXTENDKEY_H__
#define __EXTENDKEY_H__
/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "config.h"
// #ifdef ExtendKeys
/* define ------------------------------------------------------------*/
#define FORXUCHENG

#ifdef FORXUCHENG

#define EXTEND_KEY_POWER_PORT GPIOE //JX4
#define EXTEND_KEY_PLUS_PORT GPIOC  //JX5
#define EXTEND_KEY_MINUS_PORT GPIOC //JX6

#define EXTEND_KEY_POWER GPIO_PIN_5 //JX4 PC1
#define EXTEND_KEY_PLUS GPIO_PIN_1 //JX5 PC2
#define EXTEND_KEY_MINUS GPIO_PIN_2 //JX6 PE5

#else
#define EXTEND_KEY_POWER_PORT GPIOC //JX4
#define EXTEND_KEY_PLUS_PORT GPIOC  //JX5
#define EXTEND_KEY_MINUS_PORT GPIOC //JX6

#define EXTEND_KEY_POWER GPIO_PIN_4 //JX4
#define EXTEND_KEY_PLUS GPIO_PIN_3  //JX5
#define EXTEND_KEY_MINUS GPIO_PIN_2 //JX6

// #define EXTEND_KEY_POWER_H() GPIOC->ODR |= (uint8_t)GPIO_PIN_4
// #define EXTEND_KEY_POWER_L() GPIOC->ODR &= (uint8_t)(~GPIO_PIN_4)

// #define EXTEND_KEY_PLUS_H() GPIOC->ODR |= (uint8_t)GPIO_PIN_3
// #define EXTEND_KEY_PLUS_L() GPIOC->ODR &= (uint8_t)(~GPIO_PIN_3)

// #define EXTEND_KEY_MINUS_H() GPIOC->ODR |= (uint8_t)GPIO_PIN_2
// #define EXTEND_KEY_MINUS_L() GPIOC->ODR &= (uint8_t)(~GPIO_PIN_2)

#endif // FORXUCHENG
/* Private typedef -----------------------------------------------------------*/

/* Output functions ---------------------------------------------------------*/
void ExtendKey_Init(void);
u8 Read_Extend_Key(GPIO_Pin_TypeDef pin);

// #endif // ExtendKeys
#endif