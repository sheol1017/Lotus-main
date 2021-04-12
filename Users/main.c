/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @version V2.0.4
  * @date    26-April-2018
  * @brief   This file contains the main function for the ADC2 Continuous Conversion example.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "config.h"
#include "timebase.h"
#include "Mp3ModuleControl.h"
#include "WS2812B.h"
#include "FIFOManage.h"


/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint16_t Conversion_Value = 0;
static u8 ucTestCounter;
static bool bplayed;
// enum errorcode error = NOERROR;
/* Private function prototypes -----------------------------------------------*/
// static void ADC_Config(void);
static void GPIO_Config(void);
static void CLK_Config(void);
// static void DoMs1(void);
// static void DoMs10(void);
// static void DoMs250(void);
// static void Dosecond(void);

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
  * @brief  Configure ADC Continuous Conversion with End Of Conversion interrupt 
  *         enabled .
  * @param  None
  * @retval None
********************************************************************************/

// static void ADC_Config()
// {
//   ADC_Init();
// }

/********************************************************************************
  * @brief  Configure GPIO for LEDs and buttons available on the evaluation board
  * @param  None
  * @retval None
********************************************************************************/
static void GPIO_Config()
{
  GPIO_Init(GPIOD, (GPIO_Pin_TypeDef)GPIO_PIN_2, GPIO_MODE_OUT_PP_HIGH_FAST);//debug
}

/********************************************************************************
  * @brief  Configure system clock to run at 16Mhz
  * @param  None
  * @retval None
********************************************************************************/
static void CLK_Config(void)
{
  /* Initialization of the clock */
  /* Clock divider to HSI/1 */
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
}

/********************************************************************************
  * @brief  Main function
  * @param  None
  * @retval None
********************************************************************************/
void main(void)
{
  /* Disable general interrupts -----------------------------------------*/
  disableInterrupts();

  /* Clock configuration -----------------------------------------*/
  CLK_Config();

  /* Watchdog Init -------------------------------------------*/
  // IWDG_Initializes();

  /* GPIO configuration ------------------------------------------*/
  GPIO_Config();

  /* TimeBase Init -----------------------------------------------*/
  TimeBase_Init();

  /* Mp3 module control init ------------------------------------*/
  MMC_Init();

  /* led init ---------------------------------------------------*/
  Ws2812b_Configuration();

  /* Enable general interrupts -----------------------------------------*/
  enableInterrupts();

  while (1)
  {

    // IWDG_ReloadCounter(); //喂狗(理论小于4ms内喂狗都不会复位，由于HSI有偏差，我们设定在xxxms喂狗一次)

    TimeBase_HandleTimeBaseCouter(); // Timebase reset

    if (TimeBase_Get1sSystemTimeDelta())
    {
      ucTestCounter += TimeBase_Get1sSystemTimeDelta();
    }

    if (TimeBase_Get1sSystemTimeDelta())
    {
app_get_msg();
    }

    if (ucTestCounter == 2)
    {
      // RGB_Wave(COLOR_LIGHTPINK,5,50000);
      RGB_Refresh(COLOR_WHITE, LED_Num);
      ucTestCounter++;

            

      if (!bplayed)
      {
        bplayed = TRUE;
        MMC_SELECT_SONG(0001u);
        MMC_SET_VOLUMN_Mid();
        MMC_SET_SINGLE_CYCLING();
      }
    }
    else if (ucTestCounter == 5)
    {
      RGB_Refresh(COLOR_GREEN, LED_Num);
      ucTestCounter++;
    }
    else if (ucTestCounter == 8)
    {
      RGB_Refresh(COLOR_BLUE, LED_Num);
      ucTestCounter++;
    }
    else if (ucTestCounter == 11)
    {
      RGB_Refresh(COLOR_RED, LED_Num);
      ucTestCounter++;
    }
    else if (ucTestCounter == 14)
    {
      RGB_Refresh(COLOR_GOLD, LED_Num);
      ucTestCounter++;
    }
    else if (ucTestCounter == 17)
    {
      RGB_Refresh(COLOR_PURPLE, LED_Num);
      ucTestCounter = 0;
    }
  }
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
