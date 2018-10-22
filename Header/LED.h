#ifndef LED_H
#define LED_H


#include "Main.h"


#define RED_LED_UP  	GPIOA->ODR &= ~GPIO_ODR_11
#define RED_LED_OFF  	GPIOA->ODR |=  GPIO_ODR_11
#define BLUE_LED_UP  	GPIOA->ODR &= ~GPIO_ODR_8
#define BLUE_LED_OFF  	GPIOA->ODR |=  GPIO_ODR_8
#define GREEN_LED_UP  	GPIOF->ODR &= ~GPIO_ODR_6
#define GREEN_LED_OFF  	GPIOF->ODR |=  GPIO_ODR_6


void LED_Init(void);




#endif
