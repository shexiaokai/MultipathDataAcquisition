#include "Main.h"


int main()
{
	
	void Init(void);
	
	Init();
	
	GPIOB->MODER|=GPIO_MODER_MODER0_0;
	GPIOB->ODR	|=GPIO_ODR_0;
	
	
	
	Delay_ms(5);
	TFT_Init();
	TFT_Clear(0x0000);
	//TFT_SetRegion(0,0,0,0);
	//TFT_WriteReg(0x07,0x0017);
	//TFT_Display_Char(1,0,'T',0xFFFF,0x0000);
	TFT_Display_Str(0,5,"temperature:  23.8",0xFFFF,0x0000);
	TFT_Display_Str(16,5,"humidity:     62%",0xFFFF,0x0000);
	TFT_Display_Str(32,5,"air pressure:102 kpa",0xFFFF,0x0000);
	
	//TFT_WriteReg(0x07,0x1017);
	//TFT_SetRegion(0,0,220-1,176-1);
	//TFT_WriteIndex(0x22);
	while(1)
	{
	//	TFT_WriteReg(0x21,55);
	//	Delay_ms(1);
	//	TFT_Writ6eReg(0x20,55);
	//	Delay_ms(1);
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
}

/*******************************************************
函数名：	Clock_Init
提  要：	时钟初始化函数,使能外设时钟
		设置HSI为系统时钟，主频1M，外设0.5M，外设定时器1M
作  者：	佘晓凯
*******************************************************/
void Clock_Init(void)
{
	void Peripheral_Clock_Enable(void);
	
    /* Enable Prefetch Buffer and set Flash Latency */
    FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;
 
    /* HCLK = SYSCLK/8 = 8M  */
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
      
    /* PCLK = HCLK/2 = 4M*/
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
	RCC->AHBENR|=RCC_AHBENR_GPIOAEN;//GPIOA clock enable
	RCC->AHBENR|=RCC_AHBENR_GPIOBEN;//GPIOB	clock enable
	RCC->AHBENR|=RCC_AHBENR_GPIOFEN;//GPIOF clock enable
}



