/**
  ******************************************************************************
  * @file    ColorConvert.h
  * @author  Albert
  * @version V0.0.0
  * @date    2021-4
  * @brief   This file is used to realize RGB2HSV / HSV2RGB
  ******************************************************************************
**/
#ifndef __COLOR_CONVERT__
#define __COLOR_CONVERT__
#include "stm8s.h"

typedef struct _tColor_HSV
{
    float h;
    float s;
    float v;
}tColor_HSV;

typedef struct _tColor_RGB
{
    u8 r;
    u8 g;
    u8 b;
}tColor_RGB;

// void rgb2hsv(uint8_t r, uint8_t g, uint8_t b, float *h, float *s, float *v);
// void hsv2rgb(float h, float s, float v, uint8_t *r, uint8_t *g, uint8_t *b);

void rgb2hsv(tColor_RGB* pColor_rgb, tColor_HSV* pColor_hsv);
void hsv2rgb(tColor_HSV* pColor_hsv, tColor_RGB* pColor_rgb);

// void rgb2hsv(uint8_t r, uint8_t g, uint8_t b, uint16_t h, uint16_t s, uint16_t v);
// void hsv2rgb(uint16_t h, uint16_t s, uint16_t v, uint8_t *r, uint8_t *g, uint8_t *b);




#endif // __COLOR_CONVERT__