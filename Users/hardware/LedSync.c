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
#include "string.h"

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

static u16 count_breath = 0;
static u8 color_count = 0;
static u8 count_big_loop;
static u8 count_small_loop;
static bool bwait;

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

const u32 index_color_loop[] = {COLOR_WHITESMOKE,
                                COLOR_PINK,
                                COLOR_SKYBLUE,
                                COLOR_LIGHTGOLDENRODYELLOW,
                                COLOR_TURQUOISE};

#define index_colorloop COLOR_WHITESMOKE,           \
                        COLOR_PINK,                 \
                        COLOR_SKYBLUE,              \
                        COLOR_LIGHTGOLDENRODYELLOW, \
                        COLOR_TURQUOISE
//第一声 10.5s	等待 0.1s	第二声 10.7s	等待 0.1s	第三声10.0s	等待 0.1s	第四声10.s 	等待 0.1s	从 第一声处循环 至 第四声
//100ms
#define index_white_loop_time 105u, 107u, 100u, 100u
#define index_Red_loop_time 105u, 107u, 100u, 100u
#define index_Blue_loop_time 105u, 107u, 100u, 100u
#define index_Glod_loop_time 105u, 107u, 100u, 100u
#define index_Green_loop_time 105u, 107u, 100u, 100u

#define index_color_dely_time 1u, 1u, 1u, 1u

#define index_a_char_time 20u, 30u, 20u, 20u
#define index_a_wait_time 5u, 2u, 2u, 5u

#define index_4_chars_time index_a_char_time, index_a_wait_time

#define index2_a_char_time 20u, 30u, 20u, 20u
#define index2_a_wait_time 5u, 2u, 2u, 5u

#define index2_4_chars_time index2_a_char_time, index2_a_wait_time

#define index_white_dely_time 5u
#define index_red_dely_time 5u
#define index_blue_dely_time 3u
#define index_glod_dely_time 3u
#define index_green_dely_time 4u

#define index_white_data COLOR_WHITE,           \
                         index_white_loop_time, \
                         index_color_dely_time, \
                         index_white_dely_time

#define index_red_data COLOR_RED,             \
                       index_Red_loop_time,   \
                       index_color_dely_time, \
                       index_red_dely_time

#define index_blue_data COLOR_BLUE,            \
                        index_Blue_loop_time,  \
                        index_color_dely_time, \
                        index_blue_dely_time

#define index_glod_data COLOR_YELLOW,          \
                        index_Glod_loop_time,  \
                        index_color_dely_time, \
                        index_glod_dely_time

#define index_green_data COLOR_GREEN,           \
                         index_Green_loop_time, \
                         index_color_dely_time, \
                         index_green_dely_time

const u8 index_loop_time[] = {index_white_loop_time, index_Red_loop_time, index_Blue_loop_time, index_Glod_loop_time, index_Green_loop_time};

#define LOOPNUM 4 //阿弥陀佛 4声
#define WORDREPEAT 2
#define COLORNUM 5 //5个光
//开始 等待 0.5s	白光	等待 0.5s	红光 （时序可以重复上个光）	等待 0.3s	蓝光（时序可以重复上个光）		金光（时序可以重复上个光）	等待 0.4s	绿光（时序可以重复上个光）等待 0.4s

const u8 index_loop_wait_time[] = {5u, 5u, 3u, 4u, 4u};

typedef struct _tWords
{
    u8 a;
    u8 mi;
    u8 tuo;
    u8 fo;
} tWords;

typedef struct _t4Words
{
    u8 Fristword[LOOPNUM];
    u8 ucWordWaitingtime[LOOPNUM];
    // u8 Secondword[LOOPNUM];
    // u8 ucWordWaitingtime2[LOOPNUM];
} t4Words;

const t4Words BuddistWord[WORDREPEAT] = {index_4_chars_time,
                                         index2_4_chars_time};

static t4Words BuddistWordCal[WORDREPEAT];
//等待 颜色 运行 时间
typedef struct _tColocycle
{
    u32 ulcolordata;
    u8 ucRunningtime[LOOPNUM];
    u8 ucWaitingtime[LOOPNUM];
    u8 ucColorChangetime;

} tColocycle;

const tColocycle Buddistcolor_bk[COLORNUM] = {index_white_data,
                                              index_red_data,
                                              index_blue_data,
                                              index_glod_data,
                                              index_green_data};

static tColocycle BuddistcolorCal[COLORNUM];

#define Rvalue(ulRGB) (u8)((ulRGB & 0x00ff0000) >> 16)
#define Gvalue(ulRGB) (u8)((ulRGB & 0x0000ff00) >> 8)
#define Bvalue(ulRGB) (u8)(ulRGB & 0x000000ff)

/** Private function prototypes -----------------------------------------------*/

/** Private functions ---------------------------------------------------------*/

/** Export functions -- -------------------------------------------------------*/

void LS_Init(void)
{
    memcpy(BuddistcolorCal, Buddistcolor_bk, sizeof(Buddistcolor_bk));
    memcpy(BuddistWordCal, BuddistWord, sizeof(BuddistWord));
    // ColorChange(BuddistcolorCal[count_big_loop + 3].ulcolordata);

    rgb2hsv(Rvalue(BuddistcolorCal[count_big_loop].ulcolordata),
            Gvalue(BuddistcolorCal[count_big_loop].ulcolordata),
            Bvalue(BuddistcolorCal[count_big_loop].ulcolordata),
            &h, &s, &v);
}

void BuddistColor_Handler(void)
{
    if (TimeBase_Get10msSystemTimeDelta())
    {

        if (0 != BuddistcolorCal[count_big_loop].ucColorChangetime)
        {
            BuddistcolorCal[count_big_loop].ucColorChangetime--;
            bwait = TRUE;
        }
        //第一次等待 后 开始
        // else if (0 != BuddistcolorCal[count_big_loop].ucRunningtime[count_small_loop])

        else if (0 != BuddistcolorCal[count_big_loop].ucRunningtime[count_small_loop])
        {
            BuddistcolorCal[count_big_loop].ucRunningtime[count_small_loop]--;
            bwait = FALSE;
            //阿 ~ 弥 ~ 陀 ~佛
            static u8 count_4words,count_word_loop;
            if (0 != BuddistWordCal[count_4words].Fristword[count_word_loop])
            {
                BuddistWordCal[count_4words].Fristword[count_word_loop]--;
                bwait = FALSE;
            }
            //顿
            else if (0 != BuddistWordCal[count_4words].ucWordWaitingtime[count_word_loop])
            {
                bwait = TRUE;
                BuddistWordCal[count_4words].ucWordWaitingtime[count_word_loop]--;
                
            }
            else if (LOOPNUM - 1 == count_word_loop)
            {
                count_4words++;
                count_word_loop = 0;
                count_breath = 0;
            }
            else
            {
                count_word_loop++;
            }



        }
        //每个光，小等待
        else if (0 != BuddistcolorCal[count_big_loop].ucWaitingtime[count_small_loop])
        {
            BuddistcolorCal[count_big_loop].ucWaitingtime[count_small_loop]--;
            bwait = TRUE;
        }
        // 一种颜色结束
        else if (LOOPNUM - 1 == count_small_loop)
        {
            count_big_loop++;
            count_small_loop = 0;
            count_breath = 0;
            ColorChange(BuddistcolorCal[count_big_loop].ulcolordata);
        }
        else
        {
            count_small_loop++;
        }
        //5个光结束
        if (COLORNUM == count_big_loop)
        {
            count_big_loop = 0;
            count_small_loop = 0;
            count_breath = 0;
            memcpy(BuddistcolorCal, Buddistcolor_bk, sizeof(Buddistcolor_bk));
            ColorChange(BuddistcolorCal[count_big_loop].ulcolordata);
        }
    }
}

void ColorChange(u32 ulRGB)
{
    //r = (u8)((ulRGB & 0x00ff0000) >> 16);
    //g = (u8)((ulRGB & 0x0000ff00) >> 8);
    //b = (u8)(ulRGB & 0x000000ff);
    //h = s = v = 0;

    // rgb2hsv(r, g, b, &h, &s, &v);
    //v = (float)1/ 256;
    // hsv2rgb(h, s, v, &r, &g, &b);
    rgb2hsv(Rvalue(ulRGB),
            Gvalue(ulRGB),
            Bvalue(ulRGB),
            &h, &s, &v);
}

u16 ucStepRunTime(void);
u16 ucStepRunTime(void)
{
    u16 a = (u16)Buddistcolor_bk[count_big_loop].ucRunningtime[count_small_loop];
    a = 299 * a / 30; // 299 in 3s,
    return a;
}

void ColorWave(void)
{
    bool bUpdateColor = FALSE;

    if (TimeBase_Get10msSystemTimeDelta())
    {
        // count_breath++;

        v = (float)index_wave[count_breath * 299 / ucStepRunTime()] / 256;
        hsv2rgb(h, s, v, &r, &g, &b);
        bUpdateColor = TRUE;

        if (count_breath >= ucStepRunTime())
        {
            count_breath = 0;
        }
        else
        {
            count_breath++;
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