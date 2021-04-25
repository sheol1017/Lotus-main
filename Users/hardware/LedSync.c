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
#include <math.h>

/** Private typedef -----------------------------------------------------------*/

#define AMITUOFO_4LOOP 4  //阿弥陀佛 4声
#define AMITUOFO_REPEAT 2 //4声重复2次
#define COLORNUM 5        //5个光
#define INC_DEC_RATE 0.5  //<1

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
//COLOR_WHITE,COLOR_AMARANTH,COLOR_BLUE,COLOR_GOLD,COLOR_LIME
//5色光颜色
#define index_COLOR_WHITE COLOR_WHITE
#define index_COLOR_RED COLOR_AMARANTH
#define index_COLOR_BLUE COLOR_BLUE
#define index_COLOR_YELLOW COLOR_GOLD
#define index_COLOR_GREEN COLOR_LIME

//第一声 10.5s	等待 0.1s	第二声 10.7s	等待 0.1s	第三声10.0s	等待 0.1s	第四声10.s 	等待 0.1s	从 第一声处循环 至 第四声
//100ms
#define index_white_loop_time 105u, 107u, 100u, 100u
#define index_Red_loop_time 105u, 107u, 100u, 100u
#define index_Blue_loop_time 105u, 107u, 100u, 100u
#define index_Glod_loop_time 105u, 107u, 100u, 100u
#define index_Green_loop_time 105u, 107u, 100u, 100u

#define index_color_dely_time 1u, 1u, 1u, 1u

//阿弥陀佛 每声时间 100ms
#define INDEX_AMITUOFO1_TIME \
    {                        \
        20u, 24u, 24u, 33u   \
    }
#define INDEX_AMITUOFO2_TIME \
    {                        \
        30u, 32u, 25u, 19u   \
    }
#define INDEX_AMITUOFO3_TIME \
    {                        \
        35u, 16u, 23u, 26u   \
    }
#define INDEX_AMITUOFO4_TIME \
    {                        \
        30u, 13u, 28u, 30u   \
    }
//四声间隔
#define INDEX_AMITUOFOWAITTIME \
    {                          \
        3u, 2u, 2u, 3u         \
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
// static uint8_t r;
// static uint8_t g;
// static uint8_t b;

// static float h;
// static float s;
// static float v;

static tColor_RGB s_rgb;
static tColor_HSV s_hsv;
static tColor_HSV s_hsv_bak, s_hsv_save;
//static float h_bak;
//static float s_bak;
//static float v_bak;

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
static bool bLightBreathStart;

static bool bLightIncrease = false;
static bool bLightDecrease = false;
static u8 Maxtime_bk;
static float Brightcal = 1;
const float BrightnessLevel[eBrightnessLevel_Max] = {BrightnessTable};
static float k;
/** export variables ----------------------------------------------------------*/

/** Private function prototypes -----------------------------------------------*/
void Do_StepRunTime(u8 ucStepRunTime_cal);
void Do_increaseLight(u8 a, u8 b);
u8 do_amituofo(tWords *pAMiTuoFo);
u8 do_4_amituofo(tAMiTuoFo *pAMiTuoFo);
u8 do_2x4_amituofo(tColocycle *pAMiTuoFo);
u8 do_colorcycle(tColocycle *pAMiTuoFo);
void LS_LedOff(void);

/** Private functions ---------------------------------------------------------*/
static void LS_Calculat_k(float v, float Time)
{
    //float WordsCycleTime[eCycle]
    // k = (float)(v / sqrt(WordsCycleTimeBrighten[eCycle]));
    // k = (float)(v / sqrt(Time*INC_DEC_RATE));
    //y = -1/(t*r)^2 (x- (t*r))^2 +v
    k = (float)(-v / pow((Time * INC_DEC_RATE), 2));
}

static void LS_LedOff(void)
{
    RGB_Refresh(COLOR_BLACK, MAX_LED_NUMBER);
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

        Do_increaseLight(p->a, ucAMiTuoFoTimeStore.a);
        p->a--;
        Do_StepRunTime(ucAMiTuoFoTimeStore.a);
        return (p->a + p->mi + p->tuo + p->fo);
    }
    else if (0 != p->mi)
    {
        Do_increaseLight(p->mi, ucAMiTuoFoTimeStore.mi);
        p->mi--;

        Do_StepRunTime(ucAMiTuoFoTimeStore.mi);
        return (p->a + p->mi + p->tuo + p->fo);
    }
    else if (0 != p->tuo)
    {
        Do_increaseLight(p->tuo, ucAMiTuoFoTimeStore.tuo);
        p->tuo--;

        Do_StepRunTime(ucAMiTuoFoTimeStore.tuo);
        return (p->a + p->mi + p->tuo + p->fo);
    }
    else if (0 != p->fo)
    {
        Do_increaseLight(p->fo, ucAMiTuoFoTimeStore.fo);
        p->fo--;

        Do_StepRunTime(ucAMiTuoFoTimeStore.fo);
        return (p->a + p->mi + p->tuo + p->fo);
    }
    // count_breath = 0;
    return 0;
}

/*****************************************************************************************************
 - 函数说明：4声阿弥陀佛处理
 - 隶属模块：内部
 - 参数说明：pAMiTuoFo 对应处理的计数
 - 返回说明：0-> 完成
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
                //下组4字时间时间保存
                u8 j;
                j = i + 1;
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
 - 返回说明：0-> 完成
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
                pwords++; //第一声完成到下一声
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
                ucAMiTuoFoTimeStore.a = pAMiTuoFo->t4timeAMiTuoFoRun[j].tAMiTuoFoRun[0].a;
                ucAMiTuoFoTimeStore.mi = pAMiTuoFo->t4timeAMiTuoFoRun[j].tAMiTuoFoRun[0].mi;
                ucAMiTuoFoTimeStore.tuo = pAMiTuoFo->t4timeAMiTuoFoRun[j].tAMiTuoFoRun[0].tuo;
                ucAMiTuoFoTimeStore.fo = pAMiTuoFo->t4timeAMiTuoFoRun[j].tAMiTuoFoRun[0].fo;
                return 1;
            }
            else
            {
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
 - 返回说明：0-> 完成
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
/*****************************************************************************************************
 - 函数说明：光颜色变化处理
 - 隶属模块：内部
 - 参数说明：
 - 返回说明：
 - 注      
*****************************************************************************************************/
void ColorChange(u32 ulColor)
{
    //r = (u8)((ulRGB & 0x00ff0000) >> 16);
    //g = (u8)((ulRGB & 0x0000ff00) >> 8);
    //b = (u8)(ulRGB & 0x000000ff);
    //h = s = v = 0;
    s_rgb.r = (uint8_t)(ulColor >> 16);
    s_rgb.g = (uint8_t)(ulColor >> 8);
    s_rgb.b = (uint8_t)(ulColor);
    rgb2hsv(&s_rgb, &s_hsv);
    s_hsv_bak = s_hsv;
    s_hsv_save = s_hsv;
    s_hsv_bak.v = Brightcal * s_hsv_save.v;
}

void Do_increaseLight(u8 a, u8 b)
{
    if (a == b)
    {
        bLightIncrease = true;
    }
}

/*****************************************************************************************************
 - 函数说明：呼吸速度变化
 - 隶属模块：内部
 - 参数说明：
 - 返回说明：
 - 注      
*****************************************************************************************************/
void Do_StepRunTime(u8 ucStepRunTime_cal)
{
    //u16 a = ucStepRunTime_cal;
    ucStepRunTime = (float)ucStepRunTime_cal;

    LS_Calculat_k(s_hsv_bak.v, ucStepRunTime);

    Maxtime_bk = ucStepRunTime_cal;

    // Put_u8((u8)ucStepRunTime);
    // DBG("\n");
    //ucStepRunTime = 1;
}

/** Export functions -- -------------------------------------------------------*/
/*****************************************************************************************************
 - 函数说明：初始化
 - 隶属模块：外部
 - 参数说明：
 - 返回说明：
 - 注      
*****************************************************************************************************/
void LS_Init(void)
{
    memcpy(BuddistcolorCal, Buddistcolor_bk, sizeof(Buddistcolor_bk));

    ucAMiTuoFoTimeStore.a = ucAMiTuoFoTime_bk.a;
    ucAMiTuoFoTimeStore.mi = ucAMiTuoFoTime_bk.mi;
    ucAMiTuoFoTimeStore.tuo = ucAMiTuoFoTime_bk.tuo;
    ucAMiTuoFoTimeStore.fo = ucAMiTuoFoTime_bk.fo;

    s_rgb.r = BuddistcolorCal[0].ulcolordata;
    s_rgb.g = BuddistcolorCal[0].ulcolordata;
    s_rgb.b = BuddistcolorCal[0].ulcolordata;
    rgb2hsv(&s_rgb, &s_hsv);
    // rgb2hsv(&s_rgb,&s_hsv_bak);
    s_hsv_bak = s_hsv;
    s_hsv_save = s_hsv;
    s_hsv_bak.v = Brightcal * s_hsv_save.v;
    ColorChange(COLOR_WHITE);
    bLightDecrease = true;
    count_breath = 0;
    ucStepRunTime = 1;
    Maxtime_bk = ucAMiTuoFoTimeStore.a;
}

/*****************************************************************************************************
 - 函数说明：
 - 隶属模块：外部
 - 参数说明：
 - 返回说明：
 - 注      
*****************************************************************************************************/
void ColorLightStart(void)
{
    bLightBreathStart = TRUE;
    LS_Init();
}

/*****************************************************************************************************
 - 函数说明：
 - 隶属模块：外部
 - 参数说明：
 - 返回说明：
 - 注      
*****************************************************************************************************/
void ColorLightoff(void)
{
    bLightBreathStart = FALSE;
}

/*****************************************************************************************************
 - 函数说明：调节亮度
 - 隶属模块：外部
 - 参数说明：
 - 返回说明：
 - 注      
*****************************************************************************************************/
u8 Change_Brightness(bool inc)
{
    for (u8 i = 0, j = 0; i < eBrightnessLevel_Max; i++)
    {

        if (Brightcal == BrightnessLevel[i])
        {
            if (inc)
            {
                j = i + 1;
                if (i == eBrightnessLevel_Max - 1)
                {
                    j = eBrightnessLevel_Max - 1;
                }
            }
            else
            {
                j = i - 1;
                if (i == 0)
                    j = 0;
            }

            Brightcal = BrightnessLevel[j];
            s_hsv_bak.v = Brightcal * s_hsv_save.v;
            return j;
        }
    }

    Brightcal = BrightnessLevel[eBrightnessLevel_Max];
    s_hsv_bak.v = Brightcal * s_hsv_save.v;
    return eBrightnessLevel_Max;
}
/*****************************************************************************************************
 - 函数说明：
 - 隶属模块：外部
 - 参数说明：
 - 返回说明：
 - 注      
*****************************************************************************************************/
void BuddistColor_Handler(void)
{

    if (bLightBreathStart)
    {
        ColorWave();
    }

    if (TimeBase_Get100msSystemTimeDelta())
    {

        if (TRUE == BuddistcolorCal[COLORNUM - 1].b4timeAMiTuoFoFinish)
        {
            LS_Init();
            if (bLightBreathStart)
                put_msg_lifo(MSG_MUSIC_SPECIAL_PLAY);
        }
        if (bLightBreathStart)
        {
            do_colorcycle(BuddistcolorCal);
        }
        else
        {
            LS_Init();
            LS_LedOff();
        }

        if (blightOn == FALSE)
        {
            LS_LedOff();
        }
    }
}

/*****************************************************************************************************
 - 函数说明：
 - 隶属模块：外部
 - 参数说明：
 - 返回说明：
 - 注      
*****************************************************************************************************/
void ColorWave(void)
{
    bool bUpdateColor = FALSE;

    if (TimeBase_Get10msSystemTimeDelta())
    {

        if (bLightIncrease == true)
        {
            count_breath++;
            //static float t;
            float t = (float)(count_breath / 10.0);

            // s_hsv.v = k * sqrt(t * INC_DEC_RATE);
            s_hsv.v = k * pow((t - (Maxtime_bk * INC_DEC_RATE)), 2) + s_hsv_bak.v; //  y = -1/(t*r)^2 (x- (t*r))^2 +v
            // Put_u8((u8)(s_hsv.v * 100));

            // max v limitation
            if (s_hsv.v > s_hsv_bak.v)
            {
                s_hsv.v = s_hsv_bak.v;
            }
            bLightDecrease = FALSE;
            // max time limitation
            if (t >= Maxtime_bk)
            {
                t = Maxtime_bk;

                // s_hsv.v = s_hsv_bak.v;

                bLightDecrease = true;
                bLightIncrease = false;
                DBG("\n Increas ok\n");
                count_breath = 0;
            }
        }
        if (bLightDecrease == true)
        {
            // count_breath--;
            float t = (float)(count_breath / 10.0);
            // TODO: add flags for scheme changes
            // For Scheme 1:
            // Put_u8((u8)count_breath);
            // DBG(" ");
            if (t < Maxtime_bk)
            {
                s_hsv.v = (float)(t / (Maxtime_bk * (1 - INC_DEC_RATE))) * s_hsv_bak.v;
            }
            else
            {
                s_hsv.v = 0;
                count_breath = 0;
                // DBG("\n Decreas ok\n");
                bLightDecrease = false;

                // bUpdateWordsCycle = true;
            }

            // if( s_hsv.v >= (float)0.2 )
            // {
            //     s_hsv.v -= (float)0.2;
            // }
            // else
            // {
            //     s_hsv.v = 0;
            //     count = 0;

            //     bLightDecrease = false;
            // bLightIncrease = true;

            // LS_Calculat_k(s_hsv_bak.v,eLSWC_Cycle_1);
            // }

            // For Scheme 2:
        }

        // v_bak -= (float)0.01;
        hsv2rgb(&s_hsv, &s_rgb);
        bUpdateColor = true;
    }

    if (bUpdateColor == TRUE)
    {
        u32 ulRGB = 0;
        // ulRGB = (((u32)(r)) << 16) | ((u32)(g << 8)) | ((u32)(b));

        ulRGB |= s_rgb.r;
        ulRGB = (ulRGB << 8) | s_rgb.g;
        ulRGB = (ulRGB << 8) | s_rgb.b;

        RGB_Refresh(ulRGB, LED_Num);
    }
}

bool LS_StartColorCycle(void)
{
    bool bReturn = false;

    // if (bStartColorCycleReady == true)
    // {
    //     bStartColorCycle = true;
    //     bReturn = true;
    // }

    return bReturn;
}
