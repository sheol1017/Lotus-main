/**
  ******************************************************************************
  * @file    Mp3ModuleControl.h
  * @author  Albert
  * @version V0.0.0
  * @date    2021-4
  * @brief   
  ******************************************************************************
**/

#ifndef __MP3_MODULE_CONTROL__
#define __MP3_MODULE_CONTROL__

#include "stm8s.h"


// cmd list
#define CMD_NEXT            0x01    // next song
#define CMD_PREVIOUS        0x02    // previous sone
#define CMD_SELECT_SONG     0X12    // select certain song, Parameter: 1-2999
#define CMD_VOLUMN_UP       0x04    // volumn up
#define CMD_VOLUMN_DOWN     0x05    // volumn down
#define CMD_SET_VOLUMN      0x06    // set volumn, Parameter: 0-30
#define CMD_SINGLE_CYCLING  0x19    // Cycling play





void MMC_Init(void);
void MMC_SendCMD(u8 CMD ,u8 feedback , u16 dat);


void MMC_Play(void);
void MMC_Pause(void);
void MMC_SET_VOLUMN_Mid(void);
void MMC_SELECT_SONG(u16 playlist_num);
void MMC_SET_SINGLE_CYCLING(void);
void MMC_Close_SINGLE_CYCLING(void);


#endif // ifndef __MP3_MODULE_CONTROL__