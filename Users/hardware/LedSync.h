/**
  ******************************************************************************
  * @file    LedSync.h
  * @author  Albert
  * @version V0.0.0
  * @date    2021-4
  * @brief   This file is used to synchronize LEDs and audio.
  ******************************************************************************
**/

#ifndef __LED_SYNC__
#define __LED_SYNC__

#include "stm8s.h"
#include "config.h"

/** Private define ------------------------------------------------------------*/
#define MAX_LED_NUMBER 50u
#define LIGHT_RATE 0.7
#define BrightnessTable 1 * LIGHT_RATE *LIGHT_RATE *LIGHT_RATE *LIGHT_RATE *LIGHT_RATE, \
                        1 * LIGHT_RATE *LIGHT_RATE *LIGHT_RATE *LIGHT_RATE , \
                        1 * LIGHT_RATE *LIGHT_RATE *LIGHT_RATE ,             \
                        1 * LIGHT_RATE *LIGHT_RATE ,                         \
                        1 * LIGHT_RATE ,                                     \
                        1  
/** Private typedef -----------------------------------------------------------*/
typedef enum _tBrightnesslevel
{
  eBrightnessLevel_1,
  eBrightnessLevel_2,
  eBrightnessLevel_3,
  eBrightnessLevel_4,
  eBrightnessLevel_5,
  eBrightnessLevel_6,
  eBrightnessLevel_Max
} tBrightnesslevel;
/** Private variables ---------------------------------------------------------*/

/** export variables ----------------------------------------------------------*/
/** Private function prototypes -----------------------------------------------*/
/** Export functions -- -------------------------------------------------------*/
void LS_Init(void);
void LS_UpdateColor(const uint32_t ulColor);
void LS_HandleColorCycle(void);
void ColorWave(void);
void ColorLightStart(void);
void ColorLightoff(void);
void ColorChange(u32 ulRGB);
void BuddistColor_Handler(void); //100ms running
u8 Change_Brightness(bool inc);

bool LS_StartColorCycle(void);
#endif // __LED_SYNC__