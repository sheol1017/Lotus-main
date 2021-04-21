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

void rgb2hsv(uint8_t r, uint8_t g, uint8_t b, float *h, float *s, float *v)
{
  float red, green ,blue;
  float cmax, cmin, delta;
  
  red = (float)r / 255;
  green = (float)g / 255;
  blue = (float)b / 255;
  
  cmax = max(red, green, blue);
  cmin = min(red, green, blue);
  delta = cmax - cmin;

  /* H */
  if(delta == 0)
  {
    *h = 0;
  }
  else
  {
    if(cmax == red)
    {
      if(green >= blue)
      {
        *h = 60 * ((green - blue) / delta);
      }
      else
      {
        *h = 60 * ((green - blue) / delta) + 360;
      }
    }
    else if(cmax == green)
    {
      *h = 60 * ((blue - red) / delta + 2);
    }
    else if(cmax == blue) 
    {
      *h = 60 * ((red - green) / delta + 4);
    }
  }
  
  /* S */
  if(cmax == 0)
  {
    *s = 0;
  }
  else
  {
    *s = delta / cmax;
  }
  
  /* V */
  *v = cmax;
}
  
void hsv2rgb(float h, float s, float v, uint8_t *r, uint8_t *g, uint8_t *b)
{
    int hi = ((int)h / 60) % 6;
    float f = h * 1.0 / 60 - hi;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1- (1 - f) * s);
    switch (hi){
        case 0:
            *r = (uint8_t)(255 * v);
            *g = (uint8_t)(255 * t);
            *b = (uint8_t)(255 * p);
            break;
        case 1:
            *r = (uint8_t)(255 * q);
            *g = (uint8_t)(255 * v);
            *b = (uint8_t)(255 * p);
            break;
        case 2:
            *r = (uint8_t)(255 * p);
            *g = (uint8_t)(255 * v);
            *b = (uint8_t)(255 * t);
            break;
        case 3:
            *r = (uint8_t)(255 * p);
            *g = (uint8_t)(255 * q);
            *b = (uint8_t)(255 * v);
            break;
        case 4:
            *r = (uint8_t)(255 * t);
            *g = (uint8_t)(255 * p);
            *b = (uint8_t)(255 * v);
            break;
        case 5:
            *r = (uint8_t)(255 * v);
            *g = (uint8_t)(255 * p);
            *b = (uint8_t)(255 * q);
            break;
    }
}
