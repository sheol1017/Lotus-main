/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/* Output functions ---------------------------------------------------------*/
void ExtendKey_Init(void)
{
    GPIO_Init(GPIOC, GPIO_PIN_2, GPIO_MODE_IN_PU_NO_IT);
    // GPIO_ReadInputPin(GPIOC, GPIO_PIN_2);
    GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_IN_PU_NO_IT);
    // GPIO_ReadInputPin(GPIOC, GPIO_PIN_3);
    GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_IN_PU_NO_IT);
    // GPIO_ReadInputPin(GPIOC, GPIO_PIN_4);
}

u8 Read_Extend_Key(GPIO_Pin_TypeDef pin)
{
    if (GPIO_ReadInputPin(GPIOC, pin))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}