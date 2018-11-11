#include "LED.h"

u8 led_rgb[3]={0,0,0};

void LED_Init(void)
{
	
	/*set gpio to general purpose pull-up output mode */
	RED_LED_OFF;
	BLUE_LED_OFF;
	GREEN_LED_OFF;
	GPIOA->MODER|=GPIO_MODER_MODER8_0;	
	GPIOA->MODER|=GPIO_MODER_MODER11_0;
	GPIOF->MODER|=GPIO_MODER_MODER6_0;
	
	
	//TIM6_INIT
	NVIC->IP[TIM6_IRQn/4]|= 0x03 << 6 <<(TIM6_IRQn&0x03)*8;	//设置为最低优先级
	NVIC->ISER[0] |= 1UL<<TIM6_IRQn; 						//enable Interrupt
	
	//TIM6->CR1|=TIM_CR1_OPM;			//在发生下一次更新事件(清除CEN位)时，计数器停止
	TIM6->CR1 |= TIM_CR1_ARPE;			//自动重装载预装载使能
	TIM6->CR1 |= TIM_CR1_URS;			//只有计数器溢出/下溢才产生一个更新中断或DMA请求
	TIM6->DIER |= TIM_DIER_UIE;		//允许更新中断
	
	TIM6->PSC = 47;					//48分屏
	
	TIM6->ARR = 49;					//计100个数
	
	LEDEN;
	
}


void TIM6_IRQHandler(void)
{
	static u8 i=255;
	
	TIM6->SR &= ~TIM_SR_UIF;
	
	i++;

	if(i==0)
	{
		if(led_rgb[0])
			RED_LED_UP;
		if(led_rgb[1])
			GREEN_LED_UP;
		if(led_rgb[2])
			BLUE_LED_UP;
	}
	if(i==led_rgb[0])
		RED_LED_OFF;
	if(i==led_rgb[1])
		GREEN_LED_OFF;
	if(i==led_rgb[2])
		BLUE_LED_OFF;
	
}


