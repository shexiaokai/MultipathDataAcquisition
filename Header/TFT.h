#ifndef TFT_H
#define TFT_H


#include "Main.h"

#define TFT_CLK_SET 	GPIOB->ODR |=  GPIO_ODR_7
#define TFT_CLK_CLR 	GPIOB->ODR &= ~GPIO_ODR_7
#define TFT_SDA_SET 	GPIOB->ODR |=  GPIO_ODR_6
#define TFT_SDA_CLR 	GPIOB->ODR &= ~GPIO_ODR_6
#define TFT_RS_SET 		GPIOB->ODR |=  GPIO_ODR_5
#define TFT_RS_CLR 		GPIOB->ODR &= ~GPIO_ODR_5
#define TFT_RST_SET 	GPIOB->ODR |=  GPIO_ODR_4
#define TFT_RST_CLR 	GPIOB->ODR &= ~GPIO_ODR_4
#define TFT_CS_SET 		GPIOB->ODR |=  GPIO_ODR_3
#define TFT_CS_CLR 		GPIOB->ODR &= ~GPIO_ODR_3

#define LCD_X_SIZE	        176
#define LCD_Y_SIZE	        220

#define X_MAX_PIXEL	        LCD_Y_SIZE
#define Y_MAX_PIXEL	        LCD_X_SIZE


void TFT_Init(void);
void TFT_Reset(void);
void TFT_WriteIndex(u8 Index);
void TFT_WriteData(u8 _data);
void TFT_WriteData_16Bit(u16 _data);
void TFT_WriteReg(u8 Index,u16 Data);

void TFT_Clear(u16 Color);
void TFT_SetXY(u16 Xpos, u16 Ypos);
void TFT_SetRegion(u8 xStar, u8 yStar,u8 xEnd,u8 yEnd);
void TFT_Display_Char(u16,u16,char,u16,u16);
void TFT_Display_Str(u16 x,u16 y,char *str,u16 Color,u16 BackColor);



#endif
