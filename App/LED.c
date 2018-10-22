#include "LED.h"

void LED_Init(void)
{
	
	/*set gpio to general purpose pull-up output mode */
	
	//RED_LED_OFF;
	//BLUE_LED_OFF;
	GREEN_LED_OFF;
	
	GPIOA->MODER|=GPIO_MODER_MODER8_0;
	
	GPIOA->MODER|=GPIO_MODER_MODER11_0;
	
	GPIOF->MODER|=GPIO_MODER_MODER6_0;
	

}



