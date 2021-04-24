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



void LS_Init(void);
void ColorWave(void);
void ColorLightStart(void);
void ColorLightoff(void);
void ColorChange(u32 ulRGB);
void BuddistColor_Handler(void); //100ms running
#endif // __LED_SYNC__