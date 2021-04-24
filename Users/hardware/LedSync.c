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

#define AMITUOFO_4LOOP 4  //阿弥陀佛 4声
#define AMITUOFO_REPEAT 2 //4声重复2次
#define COLORNUM 5        //5个光

#define Rvalue(ulRGB) (u8)((ulRGB & 0x00ff0000) >> 16)
#define Gvalue(ulRGB) (u8)((ulRGB & 0x0000ff00) >> 8)
#define Bvalue(ulRGB) (u8)(ulRGB & 0x000000ff)

//阿弥陀佛
typedef struct _tWords
{
    u8 a;
    u8 mi;
    u8 tuo;
    u8 fo;

} tWords;

//const tWords bbb[AMITUOFO_4LOOP] = {index_4_AMITUOFO_time};
// #define index_4_AMITUOFO_time {INDEX_AMITUOFO1_TIME},   \
//                               {INDEX_AMITUOFO2_TIME},   \
//                               {INDEX_AMITUOFO3_TIME},   \
//                               {INDEX_AMITUOFO4_TIME}
// 4声 阿弥陀佛
typedef struct _tAMiTuoFo
{
    tWords tAMiTuoFoRun[AMITUOFO_4LOOP];
    u8 ucAMiTuoFoWaittime[AMITUOFO_4LOOP];
    bool bAMiTuoFoFinish;
} tAMiTuoFo;

// const tAMiTuoFo tAMiTuoFo_bk[AMITUOFO_REPEAT] = {index_2_4AMITUOFO_time};
// #define index_2_4AMITUOFO_time  {index_4_AMITUOFO_time, INDEX_AMITUOFOWAITTIME,LOOPNOTFINISH},\
// {index_4_AMITUOFO_time, INDEX_AMITUOFOWAITTIME,LOOPNOTFINISH}
//等待 颜色 运行 时间

typedef struct _tColocycle
{
    u32 ulcolordata;
    tAMiTuoFo t4timeAMiTuoFoRun[AMITUOFO_REPEAT];
    u8 uc4timeAMiTuoFoWaitingtime[AMITUOFO_REPEAT];
    u8 ucColorChangewaittime;
    bool b4timeAMiTuoFoFinish;
    // u8 ucRunningtime[AMITUOFO_4LOOP];
    // u8 ucColorChangetime;

} tColocycle;
/** Private define ------------------------------------------------------------*/

//5色光颜色
#define index_COLOR_WHITE COLOR_WHITE
#define index_COLOR_RED COLOR_RED
#define index_COLOR_BLUE COLOR_BLUE
#define index_COLOR_YELLOW COLOR_YELLOW
#define index_COLOR_GREEN COLOR_GREEN

//第一声 10.5s	等待 0.1s	第二声 10.7s	等待 0.1s	第三声10.0s	等待 0.1s	第四声10.s 	等待 0.1s	从 第一声处循环 至 第四声
//100ms
#define index_white_loop_time 105u, 107u, 100u, 100u
#define index_Red_loop_time 105u, 107u, 100u, 100u
#define index_Blue_loop_time 105u, 107u, 100u, 100u
#define index_Glod_loop_time 105u, 107u, 100u, 100u
#define index_Green_loop_time 105u, 107u, 100u, 100u

#define index_color_dely_time 1u, 1u, 1u, 1u

//阿弥陀佛 每声时间
#define INDEX_AMITUOFO1_TIME \
    {                        \
        10u, 30u, 20u, 20u   \
    }
#define INDEX_AMITUOFO2_TIME \
    {                        \
        20u, 30u, 20u, 20u   \
    }
#define INDEX_AMITUOFO3_TIME \
    {                        \
        30u, 30u, 20u, 20u   \
    }
#define INDEX_AMITUOFO4_TIME \
    {                        \
        40u, 30u, 20u, 20u   \
    }
//四声间隔
#define INDEX_AMITUOFOWAITTIME \
    {                          \
        5u, 2u, 2u, 5u         \
    }
// 两个四声间隔
#define INDEX_2_AMITUOFOWAITTIME \
    {                            \
        5u, 2u                   \
    }
// 每个光的间隔
#define index_white_dely_time 5u
#define index_red_dely_time 5u
#define index_blue_dely_time 3u
#define index_glod_dely_time 3u
#define index_green_dely_time 4u
#define LOOPNOTFINISH FALSE

#define index_4_AMITUOFO_time INDEX_AMITUOFO1_TIME, \
                              INDEX_AMITUOFO2_TIME, \
                              INDEX_AMITUOFO3_TIME, \
                              INDEX_AMITUOFO4_TIME

#define index_2_4AMITUOFO_time                                         \
    {{index_4_AMITUOFO_time}, INDEX_AMITUOFOWAITTIME, LOOPNOTFINISH},  \
    {                                                                  \
        {index_4_AMITUOFO_time}, INDEX_AMITUOFOWAITTIME, LOOPNOTFINISH \
    }

#define index_color_data                                                                                               \
    {index_COLOR_WHITE, {index_2_4AMITUOFO_time}, INDEX_2_AMITUOFOWAITTIME, index_white_dely_time, LOOPNOTFINISH},     \
        {index_COLOR_RED, {index_2_4AMITUOFO_time}, INDEX_2_AMITUOFOWAITTIME, index_red_dely_time, LOOPNOTFINISH},     \
        {index_COLOR_BLUE, {index_2_4AMITUOFO_time}, INDEX_2_AMITUOFOWAITTIME, index_blue_dely_time, LOOPNOTFINISH},   \
        {index_COLOR_YELLOW, {index_2_4AMITUOFO_time}, INDEX_2_AMITUOFOWAITTIME, index_glod_dely_time, LOOPNOTFINISH}, \
    {                                                                                                                  \
        index_COLOR_GREEN, {index_2_4AMITUOFO_time}, INDEX_2_AMITUOFOWAITTIME, index_green_dely_time, LOOPNOTFINISH    \
    }

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
static float ucStepRunTime = 1;
static tColocycle BuddistcolorCal[COLORNUM];
const tColocycle Buddistcolor_bk[COLORNUM] = {{index_COLOR_WHITE, {index_2_4AMITUOFO_time}, INDEX_2_AMITUOFOWAITTIME, index_white_dely_time, LOOPNOTFINISH},
                                              {index_COLOR_RED, {index_2_4AMITUOFO_time}, INDEX_2_AMITUOFOWAITTIME, index_red_dely_time, LOOPNOTFINISH},
                                              {index_COLOR_BLUE, {index_2_4AMITUOFO_time}, INDEX_2_AMITUOFOWAITTIME, index_blue_dely_time, LOOPNOTFINISH},
                                              {index_COLOR_YELLOW, {index_2_4AMITUOFO_time}, INDEX_2_AMITUOFOWAITTIME, index_glod_dely_time, LOOPNOTFINISH},
                                              {index_COLOR_GREEN, {index_2_4AMITUOFO_time}, INDEX_2_AMITUOFOWAITTIME, index_green_dely_time, LOOPNOTFINISH}};
static bool blightOn;
static tWords ucAMiTuoFoTimeStore = INDEX_AMITUOFO1_TIME;
const tWords ucAMiTuoFoTime_bk = INDEX_AMITUOFO1_TIME;
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

//增加每个字一光

/** Private function prototypes -----------------------------------------------*/
void Do_StepRunTime(u8 ucStepRunTime_cal);

u8 do_amituofo(tWords *pAMiTuoFo);
u8 do_4_amituofo(tAMiTuoFo *pAMiTuoFo);
u8 do_2x4_amituofo(tColocycle *pAMiTuoFo);
u8 do_colorcycle(tColocycle *pAMiTuoFo);
/** Private functions ---------------------------------------------------------*/

/** Export functions -- -------------------------------------------------------*/

void LS_Init(void)
{
    memcpy(BuddistcolorCal, Buddistcolor_bk, sizeof(Buddistcolor_bk));
    // memcpy(BuddistWordCal, BuddistWord, sizeof(BuddistWord));
    // ColorChange(BuddistcolorCal[count_big_loop + 3].ulcolordata);
    // memset(tAMiTuoFodatabk, 0, sizeof(tAMiTuoFodatabk));
    // memcpy(ucAMiTuoFoTime_bk, ucAMiTuoFoTimeStore, sizeof(ucAMiTuoFoTime_bk));
    ucAMiTuoFoTimeStore.a = ucAMiTuoFoTime_bk.a;
    ucAMiTuoFoTimeStore.mi = ucAMiTuoFoTime_bk.mi;
    ucAMiTuoFoTimeStore.tuo = ucAMiTuoFoTime_bk.tuo;
    ucAMiTuoFoTimeStore.fo = ucAMiTuoFoTime_bk.fo;

    rgb2hsv(Rvalue(BuddistcolorCal[0].ulcolordata),
            Gvalue(BuddistcolorCal[0].ulcolordata),
            Bvalue(BuddistcolorCal[0].ulcolordata),
            &h, &s, &v);
}

/*****************************************************************************************************
 - 函数说明：阿弥陀佛处理
 - 隶属模块：内部
 - 参数说明：pAMiTuoFo 对应处理的计数
 - 返回说明：还剩余时间
 - 注      
*****************************************************************************************************/
u8 do_amituofo(tWords *pAMiTuoFo)
{
    tWords *p;
    p = pAMiTuoFo;
    if (0 != p->a)
    {
        p->a--;
        Do_StepRunTime(ucAMiTuoFoTimeStore.a);
        return (p->a + p->mi + p->tuo + p->fo);
    }
    else if (0 != p->mi)
    {
        p->mi--;
        Do_StepRunTime(ucAMiTuoFoTimeStore.mi);
        return (p->a + p->mi + p->tuo + p->fo);
    }
    else if (0 != p->tuo)
    {
        p->tuo--;
        Do_StepRunTime(ucAMiTuoFoTimeStore.tuo);
        return (p->a + p->mi + p->tuo + p->fo);
    }
    else if (0 != p->fo)
    {
        p->fo--;
        Do_StepRunTime(ucAMiTuoFoTimeStore.fo);
        return (p->a + p->mi + p->tuo + p->fo);
    }
    return 0;
}

/*****************************************************************************************************
 - 函数说明：4声阿弥陀佛处理
 - 隶属模块：内部
 - 参数说明：pAMiTuoFo 对应处理的计数
 - 返回说明：还剩余时间
 - 注      
*****************************************************************************************************/
u8 do_4_amituofo(tAMiTuoFo *pAMiTuoFo)
{
    tAMiTuoFo *p;
    tWords *pwords;
    u8 *pwordswait;
    p = pAMiTuoFo;

    if (FALSE == p->bAMiTuoFoFinish)
    {
        pwords = p->tAMiTuoFoRun;
        pwordswait = p->ucAMiTuoFoWaittime;
        for (u8 i = 0; i < AMITUOFO_4LOOP; i++)
        {
            if (0 == p->ucAMiTuoFoWaittime[i])
            {
                pwords++;
            }
            if (0 != do_amituofo(pwords))
            {
                blightOn = TRUE;
                return 1;
            }
            else if (0 != *pwordswait)
            {
                p->ucAMiTuoFoWaittime[i]--;
                blightOn = FALSE;
                u8 j;
                j = i + 1;
                // memcpy(ucAMiTuoFoTimeStore, pAMiTuoFo->tAMiTuoFoRun[i], sizeof(ucAMiTuoFoTimeStore));
                ucAMiTuoFoTimeStore.a = pAMiTuoFo->tAMiTuoFoRun[j].a;
                ucAMiTuoFoTimeStore.mi = pAMiTuoFo->tAMiTuoFoRun[j].mi;
                ucAMiTuoFoTimeStore.tuo = pAMiTuoFo->tAMiTuoFoRun[j].tuo;
                ucAMiTuoFoTimeStore.fo = pAMiTuoFo->tAMiTuoFoRun[j].fo;
                return 1;
            }
            else
            {
                //pwords++;
                pwordswait++;
            }
        }
        p->bAMiTuoFoFinish = TRUE;
    }
    return 0;
}

/*****************************************************************************************************
 - 函数说明：4声阿弥陀佛处理
 - 隶属模块：内部
 - 参数说明：pAMiTuoFo 对应处理的计数
 - 返回说明：还剩余时间
 - 注      
*****************************************************************************************************/
u8 do_2x4_amituofo(tColocycle *pAMiTuoFo)
{
    tColocycle *p;
    tAMiTuoFo *pwords;
    u8 *pwordswait;
    p = pAMiTuoFo;

    if (FALSE == p->b4timeAMiTuoFoFinish)
    {
        pwords = p->t4timeAMiTuoFoRun;
        pwordswait = p->uc4timeAMiTuoFoWaitingtime;
        for (u8 i = 0; i < AMITUOFO_REPEAT; i++)
        {
            if (0 == p->uc4timeAMiTuoFoWaitingtime[i])
            {
                //memcpy(p->t4timeAMiTuoFoRun,tAMiTuoFo_bk,sizeof(tAMiTuoFo_bk));
                pwords++; //第一声完成到下一声
                          //pwordswait ++;
            }

            if (0 != do_4_amituofo(pwords))
            {
                return 1;
            }
            else if (0 != *pwordswait)
            {
                p->uc4timeAMiTuoFoWaitingtime[i]--;
                blightOn = FALSE;

                u8 j;
                j = i + 1;
                // memcpy(ucAMiTuoFoTimeStore, pAMiTuoFo->tAMiTuoFoRun[i], sizeof(ucAMiTuoFoTimeStore));
                ucAMiTuoFoTimeStore.a = pAMiTuoFo->t4timeAMiTuoFoRun[j].tAMiTuoFoRun[0].a;
                ucAMiTuoFoTimeStore.mi = pAMiTuoFo->t4timeAMiTuoFoRun[j].tAMiTuoFoRun[0].mi;
                ucAMiTuoFoTimeStore.tuo = pAMiTuoFo->t4timeAMiTuoFoRun[j].tAMiTuoFoRun[0].tuo;
                ucAMiTuoFoTimeStore.fo = pAMiTuoFo->t4timeAMiTuoFoRun[j].tAMiTuoFoRun[0].fo;
                return 1;
            }
            else
            {
                //pwords++;
                pwordswait++;
            }
        }

        return 0;
    }
}

/*****************************************************************************************************
 - 函数说明：5色光处理
 - 隶属模块：内部
 - 参数说明：pAMiTuoFo 对应处理的计数
 - 返回说明：还剩余时间
 - 注      
*****************************************************************************************************/
u8 do_colorcycle(tColocycle *pAMiTuoFo)
{
    tColocycle *p;
    u8 pwordswait;
    p = pAMiTuoFo;
    for (u8 i = 0; i < COLORNUM; i++)
    {
        if (TRUE == p->b4timeAMiTuoFoFinish)
        {
            if (i == COLORNUM - 1)
                return 0;
            p++; //下个光
        }
    }

    if (FALSE == p->b4timeAMiTuoFoFinish)
    {
        pwordswait = p->ucColorChangewaittime;

        if (0 != do_2x4_amituofo(p))
        {
            return 1;
        }
        else if (0 != pwordswait)
        {
            p->ucColorChangewaittime--;
            blightOn = FALSE;
            ucAMiTuoFoTimeStore.a = ucAMiTuoFoTime_bk.a;
            ucAMiTuoFoTimeStore.mi = ucAMiTuoFoTime_bk.mi;
            ucAMiTuoFoTimeStore.tuo = ucAMiTuoFoTime_bk.tuo;
            ucAMiTuoFoTimeStore.fo = ucAMiTuoFoTime_bk.fo;
            return 1;
        }
        p->b4timeAMiTuoFoFinish = TRUE;
        p++;
        ColorChange(p->ulcolordata);
        return 0;
    }
}

void BuddistColor_Handler(void)
{
    if (TimeBase_Get100msSystemTimeDelta())
    {
        if (TRUE == BuddistcolorCal[COLORNUM - 1].b4timeAMiTuoFoFinish)
        {
            LS_Init();
        }

        do_colorcycle(BuddistcolorCal);
    }
    if (blightOn == FALSE)
    {
        // ColorChange(COLOR_BLACK);
    }
}

void ColorChange(u32 ulRGB)
{
    //r = (u8)((ulRGB & 0x00ff0000) >> 16);
    //g = (u8)((ulRGB & 0x0000ff00) >> 8);
    //b = (u8)(ulRGB & 0x000000ff);
    //h = s = v = 0;

    // rgb2hsv(r, g, b, &h, &s, &v);
    // v = (float)index_wave[0] / 256;
    // hsv2rgb(h, s, v, &r, &g, &b);
    rgb2hsv(Rvalue(ulRGB),
            Gvalue(ulRGB),
            Bvalue(ulRGB),
            &h, &s, &v);
    count_breath = 0;
}

//100ms
void Do_StepRunTime(u8 ucStepRunTime_cal)
{
    //u16 a = ucStepRunTime_cal;
    ucStepRunTime = 30 / (float)ucStepRunTime_cal;
    //ucStepRunTime = 1;
}

void ColorWave(void)
{
    bool bUpdateColor = FALSE;

    if (TimeBase_Get10msSystemTimeDelta())
    {
        // count_breath++;

        v = (float)index_wave[count_breath] / 256;
        hsv2rgb(h, s, v, &r, &g, &b);
        bUpdateColor = true;

        if (count_breath >= 299)
        {
            count_breath = 0;
        }
        else
        {
            count_breath += (u16)ucStepRunTime;
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