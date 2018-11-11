#ifndef MAIN_H
#define MAIN_H

#define u8 uint8_t
#define u16 uint16_t

#include "stm32f0xx.h"
#include "system_stm32f0xx.h"
#include "core_cm0.h"
#include "stm32f0xx_conf.h"


#include "Delay.h"
#include "LED.h"
#include "BK2425.h"
#include "Touch.h"
#include "TFT.h"



#define SYSCLK_CLOCK 48UL

extern u8 led_rgb[];
extern u8 touch_d[];
extern u8 BK2425_tx_buf[];

#endif


