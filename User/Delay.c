#include "Delay.h"

void Delay_Init(void)
{
	//SysTick->LOAD = 0x00FFFFFF;

	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
}

void Delay_us(u16 t)
{
	SysTick->LOAD = t*SYSCLK_CLOCK-1;
	SysTick->VAL = 0;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void Delay_ms(u16 t)
{
	int i;
	for(i=0;i<t;i++)
	{
		Delay_us(1000);
	}
}

void Delay1us(void)   //Îó²î 0us
{
    unsigned char a,b;
    for(b=1;b>0;b--)
        for(a=3;a>0;a--);
}
	
