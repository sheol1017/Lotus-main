/**
  ******************************************************************************
  * @file    ColorConvert.c
  * @author  Albert
  * @version V0.0.0
  * @date    2021-4
  * @brief   This file is used to realize RGB2HSV / HSV2RGB
  ******************************************************************************
**/


/** include  ------------------------------------------------------------------*/

#include "ColorConvert.h"
#include "stm8s.h"


/** Private typedef -----------------------------------------------------------*/



/** Private define ------------------------------------------------------------*/

// #define MAX(x,y)   x>y? x:y
// #define MIN(x,y)   x<y? x:y

/** Private variables ---------------------------------------------------------*/

// static color_HSV_t g_hsv;
// static color_RGB_t g_rgb;


/** export variables ----------------------------------------------------------*/

/** Private function prototypes -----------------------------------------------*/

static float min(float a, float b, float c);
static float max(float a, float b, float c);
// static uint16_t min(uint16_t a, uint16_t b, uint16_t c);
// static uint16_t max(uint16_t a, uint16_t b, uint16_t c);


/** Private functions ---------------------------------------------------------*/
static float min(float a, float b, float c)
{
  float m;
  
  m = a < b ? a : b;
  return (m < c ? m : c); 
}

static float max(float a, float b, float c)
{
  float m;
  
  m = a > b ? a : b;
  return (m > c ? m : c); 
}

/** Export functions -- -------------------------------------------------------*/

// void rgb2hsv(uint8_t r, uint8_t g, uint8_t b, float *h, float *s, float *v)
void rgb2hsv(tColor_RGB* pColor_rgb, tColor_HSV* pColor_hsv)
{
  float red, green ,blue;
  float cmax, cmin, delta;
  
  red   = (float)pColor_rgb->r / 255;
  green = (float)pColor_rgb->g / 255;
  blue  = (float)pColor_rgb->b / 255;
  
  cmax = max(red, green, blue);
  cmin = min(red, green, blue);
  delta = cmax - cmin;

  /* H */
  if(delta == 0)
  {
    pColor_hsv->h = 0;
  }
  else
  {
    if(cmax == red)
    {
      if(green >= blue)
      {
        pColor_hsv->h = 60 * ((green - blue) / delta);
      }
      else
      {
        pColor_hsv->h = 60 * ((green - blue) / delta) + 360;
      }
    }
    else if(cmax == green)
    {
      pColor_hsv->h = 60 * ((blue - red) / delta + 2);
    }
    else if(cmax == blue) 
    {
      pColor_hsv->h = 60 * ((red - green) / delta + 4);
    }
  }
  
  /* S */
  if(cmax == 0)
  {
    pColor_hsv->s = 0;
  }
  else
  {
    pColor_hsv->s = delta / cmax;
  }
  
  /* V */
  pColor_hsv->v = cmax;
}
  
// void hsv2rgb(float h, float s, float v, uint8_t *r, uint8_t *g, uint8_t *b)
void hsv2rgb(tColor_HSV* pColor_hsv, tColor_RGB* pColor_rgb)
{
    int hi = ((int)pColor_hsv->h / 60) % 6;
    float f = pColor_hsv->h * 1.0 / 60 - hi;
    float p = pColor_hsv->v * (1 - pColor_hsv->s);
    float q = pColor_hsv->v * (1 - f * pColor_hsv->s);
    float t = pColor_hsv->v * (1- (1 - f) * pColor_hsv->s);
    switch (hi){
        case 0:
            pColor_rgb->r = (uint8_t)(255 * pColor_hsv->v);
            pColor_rgb->g = (uint8_t)(255 * t);
            pColor_rgb->b = (uint8_t)(255 * p);
            break;
        case 1:
            pColor_rgb->r = (uint8_t)(255 * q);
            pColor_rgb->g = (uint8_t)(255 * pColor_hsv->v);
            pColor_rgb->b = (uint8_t)(255 * p);
            break;
        case 2:
            pColor_rgb->r = (uint8_t)(255 * p);
            pColor_rgb->g = (uint8_t)(255 * pColor_hsv->v);
            pColor_rgb->b = (uint8_t)(255 * t);
            break;
        case 3:
            pColor_rgb->r = (uint8_t)(255 * p);
            pColor_rgb->g = (uint8_t)(255 * q);
            pColor_rgb->b = (uint8_t)(255 * pColor_hsv->v);
            break;
        case 4:
            pColor_rgb->r = (uint8_t)(255 * t);
            pColor_rgb->g = (uint8_t)(255 * p);
            pColor_rgb->b = (uint8_t)(255 * pColor_hsv->v);
            break;
        case 5:
            pColor_rgb->r = (uint8_t)(255 * pColor_hsv->v);
            pColor_rgb->g = (uint8_t)(255 * p);
            pColor_rgb->b = (uint8_t)(255 * q);
            break;
    }
}
