#include "Touch.h"

u16 touch_a[4]={0};
u16 touch_t[4]={0};
u8 	touch_d[4]={0};



void Touch_Init(void)
{
	void Tim3_Init(void);
	
	Tim3_Init();
	
	Delay_ms(150);
	
	touch_t[0]=touch_a[0]+TOUCH_THRESHOLD_VALUE;
	touch_t[1]=touch_a[1]+TOUCH_THRESHOLD_VALUE;
	touch_t[2]=touch_a[2]+TOUCH_THRESHOLD_VALUE;
	touch_t[3]=touch_a[3]+TOUCH_THRESHOLD_VALUE;
}

void Tim3_Init(void)
{
	
	//NVIC_InitTypeDef NVIC_InitStruct;
	//NVIC_InitStruct.NVIC_IRQChannel=TIM3_IRQn;
	//NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	//NVIC_InitStruct.NVIC_IRQChannelPriority=0x03;
	//NVIC_Init(&NVIC_InitStruct);
	
	TOUCH_GPIO_INIT;		//将按键引脚输出置低
	TOUCH_DELAY_1us;
	TOUCH_RELEASE_CHARGE;	//放电
	TOUCH_DELAY_1us;
	
	NVIC->IP[TIM3_IRQn/4]|= 0x03 << 6 <<(TIM3_IRQn&0x03)*8;	//设置为最低优先级
	NVIC->ISER[0] |= 1UL<<TIM3_IRQn; 						//enable Interrupt
	TIM3->CR1|=TIM_CR1_OPM;			//在发生下一次更新事件(清除CEN位)时，计数器停止
	TIM3->CR1|=TIM_CR1_URS;			//只有计数器溢出/下溢才产生一个更新中断或DMA请求
	TIM3->DIER|=TIM_DIER_UIE;		//允许更新中断
	TIM3->PSC = 2;
	
	//CCX channel is configured as input, ICX is mapped on TIX
	TIM3->CCMR1	|= TIM_CCMR1_CC1S_0;
	TIM3->CCMR1	|= TIM_CCMR1_CC2S_0; 
	TIM3->CCMR2	|= TIM_CCMR2_CC3S_0;
	TIM3->CCMR2	|= TIM_CCMR2_CC4S_0; 
	
	// Capture enabled
	TIM3->CCER |= TIM_CCER_CC1E|TIM_CCER_CC2E|TIM_CCER_CC3E|TIM_CCER_CC4E;
	
	TOUCH_RECHARGE_CHARGE;		//引脚配为输入模式
	TOUCH_DELAY_1us;			
	TOUCH_COM_SET;				//开始充电
	
	TIM3->CR1|= TIM_CR1_CEN;	//定时器启动
	
	
	
	
}


void TIM3_IRQHandler(void)
{
	
	TIM3->SR &= ~TIM_SR_UIF;
	
	
	
	//滑动滤波
	touch_a[0] = (touch_a[0]*9.0+(TIM3->CCR1))/10;
	touch_a[1] = (touch_a[1]*9.0+(TIM3->CCR2))/10;
	touch_a[2] = (touch_a[2]*9.0+(TIM3->CCR3))/10;
	touch_a[3] = (touch_a[3]*9.0+(TIM3->CCR4))/10;
	
//	touch_a[0] = (TIM3->CCR1);
//	touch_a[1] = (TIM3->CCR2);
//	touch_a[2] = (TIM3->CCR3);
//	touch_a[3] = (TIM3->CCR4);
	
	touch_d[0] = touch_a[0]>touch_t[0]? 1:0;
	touch_d[1] = touch_a[1]>touch_t[1]? 1:0;
	touch_d[2] = touch_a[2]>touch_t[2]? 1:0;
	touch_d[3] = touch_a[3]>touch_t[3]? 1:0;
	
	
	TOUCH_RELEASE_CHARGE;		//放电
	TOUCH_DELAY_1us;
	TOUCH_RECHARGE_CHARGE;		//引脚配为输入模式
	TOUCH_DELAY_1us;			
	TOUCH_COM_SET;				//开始充电
	
	
	
	TIM3->CR1|= TIM_CR1_CEN;
	
	
	//TIM3->CNT = 0x00;
}

