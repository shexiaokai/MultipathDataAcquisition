#include "Main.h"


int main()
{
	
	void Init(void);
	u8 a=0;
	Init();	
	BK2425_TX_Mode(); 
	GPIOB->MODER |= GPIO_MODER_MODER3_0|GPIO_MODER_MODER4_0|GPIO_MODER_MODER5_0|GPIO_MODER_MODER6_0|GPIO_MODER_MODER7_0;
	// Output open-drain
	GPIOB->OTYPER|=GPIO_OTYPER_OT_3|GPIO_OTYPER_OT_4|GPIO_OTYPER_OT_5|GPIO_OTYPER_OT_6|GPIO_OTYPER_OT_7;
	//Pull-up
	GPIOB->PUPDR|=GPIO_PUPDR_PUPDR3_0|GPIO_PUPDR_PUPDR4_0|GPIO_PUPDR_PUPDR5_0|GPIO_PUPDR_PUPDR6_0|GPIO_PUPDR_PUPDR7_0;
	
	GPIOB->MODER |= GPIO_MODER_MODER0_0;
	
	GPIOB->ODR   |= GPIO_ODR_0;
	
	while(1)
	{
		
		a=BK2425_TxPacket(BK2425_tx_buf);
		if(a==0xFF)
			led_rgb[0]=255;
		
		else if(a==0x10)
		{
			led_rgb[1]=255;
			led_rgb[2]=0;
			
		}
		else
			led_rgb[2]=255;
		Delay_ms(1000);
		
		
	}
		;
}

/*******************************************************
函数名：	Clock_Init
作  者：	佘晓凯
*******************************************************/
void Init(void)
{
	void Clock_Init(void);	
	
	Clock_Init();
	Delay_Init();
	LED_Init();			
	Touch_Init();		//触摸按键初始化
	BK2425_Init();
	
	//TFT_Init();
	led_rgb[1]=255;
	Delay_ms(500);
	led_rgb[1]=0;
	
	
}

/*******************************************************
函数名：	Clock_Init
提  要：	时钟初始化函数,使能外设时钟
		设置HSI为系统时钟，主频48M，外设24M，外设定时器48M
作  者：	佘晓凯
*******************************************************/
void Clock_Init(void)
{
	void Peripheral_Clock_Enable(void);
	
    /* Enable Prefetch Buffer and set Flash Latency */
    FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;
 
    /* HCLK = SYSCLK = 48M  */
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
      
    /* PCLK = HCLK/2 = 24M*/
    RCC->CFGR |= RCC_CFGR_PPRE_DIV2;
	
	/* PLL configuration = HSE/6*12 = 48 MHz */
	RCC->CFGR |= RCC_CFGR_PLLSRC_HSI_DIV2;
	
	RCC->CFGR |= RCC_CFGR_PLLMULL12;
	
	 /* Enable PLL */
	RCC->CR |= RCC_CR_PLLON;

    /* Wait till PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0);
	
	 /* Select PLL as system clock source */
	RCC->CFGR |= RCC_CFGR_SW_PLL; 
	
	Peripheral_Clock_Enable();
}

void Peripheral_Clock_Enable(void)
{
	RCC->AHBENR|=RCC_AHBENR_GPIOAEN;	//GPIOA clock enable
	RCC->AHBENR|=RCC_AHBENR_GPIOBEN;	//GPIOB	clock enable
	RCC->AHBENR|=RCC_AHBENR_GPIOCEN;	//GPIOA clock enable
	RCC->AHBENR|=RCC_AHBENR_GPIOFEN;	//GPIOF clock enable
	RCC->APB1ENR|=RCC_APB1ENR_TIM3EN;	//TIM3 	clock enable
	RCC->APB1ENR|=RCC_APB1ENR_TIM6EN;
	
}



