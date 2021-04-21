/**
  ******************************************************************************
  * @file    LedSync.c
  * @author  Albert
  * @version V0.0.0
  * @date    2021-4
  * @brief   This file is used to synchronize LEDs and audio.
  ******************************************************************************
**/


/** include  ------------------------------------------------------------------*/

#include "LedSync.h"
#include "Stm8s.h"
#include "timebase.h"
#include "WS2812B.h"
#include "ColorConvert.h"

/** Private typedef -----------------------------------------------------------*/

/** Private define ------------------------------------------------------------*/

/** Private variables ---------------------------------------------------------*/

// static color_HSV_t g_hsv;
// static color_HSV_t g_hsv_bak;
// static color_RGB_t g_rgb = {0x639C,0x0000,0x0000};
static uint8_t r;
static uint8_t g;
static uint8_t b;

static float h;
static float s;
static float v;

static float h_bak;
static float s_bak;
static float v_bak;

static uint16_t count = 0;




/** export variables ----------------------------------------------------------*/

/* 呼吸灯曲线表 */
const uint16_t index_wave[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
                                3, 3, 3, 3, 3, 3, 3, 3, 3, 
                                4, 4, 4, 4, 4, 4, 4, 
                                5, 5, 5, 5, 5, 
                                6, 6, 6, 6, 6, 
                                7, 7, 7, 7, 
                                8, 8, 8, 
                                9, 9, 9, 
                                10, 10, 10, 
                                11, 11, 
                                12, 12, 
                                13, 13, 
                                14, 14, 
                                15, 15, 
                                16, 16, 
                                17, 
                                18, 18, 
                                19, 
                                20, 20, 
                                21, 22, 23, 24, 
                                25, 25, 
                                26, 27, 28, 30, 31, 32, 33, 34, 36, 37, 38, 40, 41, 43, 45, 46, 48, 50, 52, 54, 56, 58, 60, 62, 65, 67, 70, 72, 75, 78, 81, 84, 87, 90, 
                                94, 97, 101, 105, 109, 113, 117, 122, 126, 131, 136, 141, 146, 152, 158, 164, 170, 176, 183, 190, 197, 205, 
                                213, 221, 229, 238, 247, 255, 255, 247, 238, 229, 221, 213, 205, 197, 190, 183, 176, 170, 164, 158, 152, 146, 
                                141, 136, 131, 126, 122, 117, 113, 109, 105, 101, 97, 94, 90, 87, 84, 81, 78, 75, 72, 70, 67, 65, 62, 60, 58, 
                                56, 54, 52, 50, 48, 46, 45, 43, 41, 40, 38, 37, 36, 34, 33, 32, 31, 30, 28, 27, 26, 25, 25, 24, 23, 22, 21, 20, 
                                20, 19, 18, 18, 17, 16, 16, 15, 15, 14, 14, 13, 13, 12, 12, 11, 11, 10, 10, 10, 9, 9, 9, 8, 8, 8, 7, 7, 7, 7, 6, 
                                6, 6, 6, 6, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
                                2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
                                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

/** Private function prototypes -----------------------------------------------*/


/** Private functions ---------------------------------------------------------*/

/** Export functions -- -------------------------------------------------------*/

void LS_Init(void)
{
    rgb2hsv(255,0,0,&h_bak,&s_bak,&v_bak);
}


void ColorWave(void)
{
    bool bUpdateColor = FALSE;

    if(TimeBase_Get10msSystemTimeDelta())
    {
        // count++;
        v = (float)index_wave[count] / 256;
        hsv2rgb(h, s, v, &r, &g, &b);
        bUpdateColor = TRUE;

        if(count >= 299)
        {
            count = 0;
        }
        else
        {
            count++;
        }

        // printf("r = %d\r\n", r);
        // printf("g = %d\r\n", g);
        // printf("b = %d\r\n", b);
        
    }

    if (bUpdateColor == TRUE)
    {
        u32 ulRGB = 0;
        // ulRGB = (((u32)(r)) << 16) | ((u32)(g << 8)) | ((u32)(b));

        ulRGB |= r;
        ulRGB = (ulRGB << 8) | g;
        ulRGB = (ulRGB << 8) | b;

        RGB_Refresh(ulRGB, LED_Num);
    }
}