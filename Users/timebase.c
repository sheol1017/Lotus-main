/**
  ******************************************************************************
  * @file     timebase.c
  * @author   
  * @version 
  * @date     
  * @brief    
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
#include "timebase.h"
#include "stm8s_tim4.h"



/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TIM4_PERIOD       124
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

// used to record system working time, unit: sec
static __IO u8 ucSystemTime;

//
static __IO u8 uc1msSystemTimeDelta;
static __IO u8 uc10msSystemTimeDelta;
static __IO u8 uc100msSystemTimeDelta;
static __IO u8 uc1sSystemTimeDelta;


/* Private function prototypes -----------------------------------------------*/

static void TimeBase_Config(void);


/* Private functions ----------------------------------------------------------*/

/**
  * @brief  Configure TIM4 to generate an update interrupt each 1ms 
  * @param  None
  * @retval None
  */
static void TimeBase_Config(void)
{
  /* TIM4 configuration:
   - TIM4CLK is set to 16 MHz, the TIM4 Prescaler is equal to 128 so the TIM1 counter
   clock used is 16 MHz / 128 = 125 000 Hz
  - With 125 000 Hz we can generate time base:
      max time base is 2.048 ms if TIM4_PERIOD = 255 --> (255 + 1) / 125000 = 2.048 ms
      min time base is 0.016 ms if TIM4_PERIOD = 1   --> (  1 + 1) / 125000 = 0.016 ms
  - In this example we need to generate a time base equal to 1 ms
   so TIM4_PERIOD = (0.001 * 125000 - 1) = 124 */

  /* Time base configuration */
  TIM4_TimeBaseInit(TIM4_PRESCALER_128, TIM4_PERIOD);
  /* Clear TIM4 update flag */
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
  /* Enable update interrupt */
  TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
  
  /* enable interrupts */
  //enableInterrupts();

  /* Enable TIM4 */
  TIM4_Cmd(ENABLE);
}

/* Public functions ----------------------------------------------------------*/


void TimeBase_Init(void){
  TIM4_DeInit();
  TimeBase_Config();
}



void TimeBase_IncerementCounter_UPD_IRQ(void){
  ucSystemTime++;
  //if( uc1msDelayTimeCounter >= 10)
  //{
   // uc1msDelayTimeCounter = 0;
    //uc10msDelayTimeCounter++;
    //bIS10msMillisencondReached = TRUE;
  //}
  /*
  ucCNT++;
  if( ucCNT >= 100 )
  {
    GPIO_WriteHigh(GPIOD,GPIO_PIN_4);
    ucCNT = 0;
  }
  else
  {
    GPIO_WriteLow(GPIOD,GPIO_PIN_4);
  }
  */
}


void TimeBase_HandleTimeBaseCouter(void){
    static u8 uc1msBaseTime;
    static u8 uc10msBaseTime;
    static u16 ui100msBaseTime;
    static u16 ui1sBaseTime;
    //
    if( ucSystemTime != 0 )
    {
      uc1msBaseTime++;
      uc10msBaseTime++;
      ui100msBaseTime++;
      ui1sBaseTime++;
      ucSystemTime--;
    }
    
    uc1msSystemTimeDelta = 0;
    if( uc1msBaseTime != 0 )
    {
      uc1msBaseTime--;
      uc1msSystemTimeDelta = 1;
    }
    
    uc10msSystemTimeDelta = 0;
    if( uc10msBaseTime >= 10 )
    {
      uc10msBaseTime -= 10;
      uc10msSystemTimeDelta = 1;
    }
    
    uc100msSystemTimeDelta = 0;
    if( ui100msBaseTime >= 100 )
    {
      ui100msBaseTime -= 100;
      uc100msSystemTimeDelta = 1;
    }
    
    uc1sSystemTimeDelta = 0;
    if( ui1sBaseTime >= 1000 )
    {
      ui1sBaseTime -= 1000;
      uc1sSystemTimeDelta = 1;
    }
}


u8 TimeBase_Get1msSystemTimeDelta(void){
  return uc1msSystemTimeDelta;
}

u8 TimeBase_Get10msSystemTimeDelta(void){
  return uc10msSystemTimeDelta;
}
    
u8 TimeBase_Get100msSystemTimeDelta(void){
  return uc100msSystemTimeDelta;
}

u8 TimeBase_Get1sSystemTimeDelta(void){
  return uc1sSystemTimeDelta;
}



