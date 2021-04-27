/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "extendkeys.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/* Output functions ---------------------------------------------------------*/
void ExtendKey_Init(void)
{

    // GPIO_Init(GPIOC, GPIO_PIN_2, GPIO_MODE_IN_PU_NO_IT);
    // // GPIO_ReadInputPin(GPIOC, GPIO_PIN_2);
    // GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_IN_PU_NO_IT);
    // // GPIO_ReadInputPin(GPIOC, GPIO_PIN_3);
    // GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_IN_PU_NO_IT);
    // // GPIO_ReadInputPin(GPIOC, GPIO_PIN_4);
GPIO_Init(EXTEND_KEY_POWER_PORT, EXTEND_KEY_POWER, GPIO_MODE_IN_PU_NO_IT);
GPIO_Init(EXTEND_KEY_PLUS_PORT, EXTEND_KEY_PLUS, GPIO_MODE_IN_PU_NO_IT);
GPIO_Init(EXTEND_KEY_MINUS_PORT, EXTEND_KEY_MINUS, GPIO_MODE_IN_PU_NO_IT);
}

u8 Read_Extend_Key(GPIO_Pin_TypeDef pin)
{
if (pin == EXTEND_KEY_POWER)
{
    if (GPIO_ReadInputPin(EXTEND_KEY_POWER_PORT,EXTEND_KEY_POWER))
    {
        return 0;
    }
    else return 1;
    
}

if (pin == EXTEND_KEY_PLUS)
{
    if (GPIO_ReadInputPin(EXTEND_KEY_PLUS_PORT,EXTEND_KEY_PLUS))
    {
        return 0;
    }
    else return 1;
    
}
if (pin == EXTEND_KEY_MINUS)
{
    if (GPIO_ReadInputPin(EXTEND_KEY_MINUS_PORT,EXTEND_KEY_MINUS))
    {
        return 0;
    }
    else return 1;
    
}

    // if (GPIO_ReadInputPin(GPIOC, pin))
    // {
    //     return 0;
    // }
    // else
    // {
    //     return 1;
    // }


}