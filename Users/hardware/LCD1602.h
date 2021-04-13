#ifndef __LCD1602_H__
#define __LCD1602_H__



void LCD1602_Init() ;
void LCD1602_DispDat( u8 x,  u8 y, u8 ch);
void LCD1602_DispStr( u8 x, u8 y, u8 *str);
void LCD1602_ClearLine(u8 x);
void LCD1602_Clear(void);
void ReturnLcdInit(void);
void Disp_PlayEQ(u8 EQ);
void Disp_PlayMode(u8 mode);
void Disp_PlayDevice(u8 device);
void Disp_OnlineDevice(u8 Online);
void GoInitDevice(u8 Online) ;
void GotoNextDevice(u8 device) ;
void ChangeDevice(u8 dev);
#endif
