#include "TFT.h"
#include "TFT_Library.h"

void TFT_WriteByte(u8 _data);

void TFT_Init(void)
{
	void TFT_GPIO_Init(void);
	
	Delay_ms(20);
	
	TFT_GPIO_Init();
	
	TFT_Reset();
	
	//Line Inversion – 1 field interlace
	TFT_WriteReg(0x02, 0x0100); 
	
	//R01H:SM=0,GS=1,SS=1 528*220
	TFT_WriteReg(0x01, 0x031C); // set the display line number and display direction
	//R03H:BGR=1,ID0=1,ID1=1,AM=1 (for details,See the datasheet of ILI9225)
	TFT_WriteReg(0x03, 0x1038); // set GRAM write direction .
	
	TFT_WriteIndex(0x08); TFT_WriteData_16Bit(0x0808); // set BP and FP
	TFT_WriteIndex(0x0B); TFT_WriteData_16Bit(0x1100); //frame cycle
	TFT_WriteIndex(0x0C); TFT_WriteData_16Bit(0x0000); // RGB interface setting R0Ch=0x0110 for RGB 18Bit and R0Ch=0111for RGB16Bit
	
	//TFT内部时钟频率计及使能
	TFT_WriteIndex(0x0F); TFT_WriteData_16Bit(0x0001); // Set frame rate----0801
	TFT_WriteIndex(0x15); TFT_WriteData_16Bit(0x0000); //set system interface
	TFT_WriteIndex(0x20); TFT_WriteData_16Bit(0x0000); // Set GRAM Address
	TFT_WriteIndex(0x21); TFT_WriteData_16Bit(0x0000); // Set GRAM Address
  //*************Power On sequence ****************//
    Delay_ms(50);                         // Delay 50ms
	TFT_WriteIndex(0x10); TFT_WriteData_16Bit(0x0800); // Set SAP,DSTB,STB----0A00
	TFT_WriteIndex(0x11); TFT_WriteData_16Bit(0x1F3F); // Set APON,PON,AON,VCI1EN,VC----1038
    Delay_ms(50);                         // Delay 50ms
	TFT_WriteIndex(0x12); TFT_WriteData_16Bit(0x0121); // Internal reference voltage= Vci;----1121
	TFT_WriteIndex(0x13); TFT_WriteData_16Bit(0x006F); // Set GVDD----0066
	TFT_WriteIndex(0x14); TFT_WriteData_16Bit(0x4349); // Set VCOMH/VCOML voltage----5F60
	//-------------- Set GRAM area -----------------//
	TFT_WriteIndex(0x30); TFT_WriteData_16Bit(0x0000);
	TFT_WriteIndex(0x31); TFT_WriteData_16Bit(0x00DB);
	TFT_WriteIndex(0x32); TFT_WriteData_16Bit(0x0000);
	TFT_WriteIndex(0x33); TFT_WriteData_16Bit(0x0000);
	TFT_WriteIndex(0x34); TFT_WriteData_16Bit(0x00DB);
	TFT_WriteIndex(0x35); TFT_WriteData_16Bit(0x0000);
	TFT_WriteIndex(0x36); TFT_WriteData_16Bit(0x00AF);
	TFT_WriteIndex(0x37); TFT_WriteData_16Bit(0x0000);
	TFT_WriteIndex(0x38); TFT_WriteData_16Bit(0x00DB);
	TFT_WriteIndex(0x39); TFT_WriteData_16Bit(0x0000);
	// ----------- Adjust the Gamma Curve ----------//
	TFT_WriteIndex(0x50); TFT_WriteData_16Bit(0x0001);  //0400
	TFT_WriteIndex(0x51); TFT_WriteData_16Bit(0x200B);  //060B
	TFT_WriteIndex(0x52); TFT_WriteData_16Bit(0x0000);  //0C0A
	TFT_WriteIndex(0x53); TFT_WriteData_16Bit(0x0404);  //0105
	TFT_WriteIndex(0x54); TFT_WriteData_16Bit(0x0C0C);  //0A0C
	TFT_WriteIndex(0x55); TFT_WriteData_16Bit(0x000C);  //0B06
	TFT_WriteIndex(0x56); TFT_WriteData_16Bit(0x0101);  //0004
	TFT_WriteIndex(0x57); TFT_WriteData_16Bit(0x0400);  //0501
	TFT_WriteIndex(0x58); TFT_WriteData_16Bit(0x1108);  //0E00
	TFT_WriteIndex(0x59); TFT_WriteData_16Bit(0x050C);  //000E
	Delay_ms(50);                                     // Delay 50ms
	TFT_WriteIndex(0x07); TFT_WriteData_16Bit(0x1004);
	TFT_WriteIndex(0x22);	
	
	
	TFT_Clear(0xFFFF);			//清屏
	TFT_Display_Str(20,35,"Welcome to use TFT",0x0000,0xFFFF); 	//显示文字
	TFT_WriteReg(0x07, 0x1017);				//开启显示
	
}

void TFT_GPIO_Init(void)
{
	
	// General purpose output mode
	GPIOB->MODER |= GPIO_MODER_MODER3_0|GPIO_MODER_MODER4_0|GPIO_MODER_MODER5_0|GPIO_MODER_MODER6_0|GPIO_MODER_MODER7_0;
	// Output open-drain
	GPIOB->OTYPER|=GPIO_OTYPER_OT_3|GPIO_OTYPER_OT_4|GPIO_OTYPER_OT_5|GPIO_OTYPER_OT_6|GPIO_OTYPER_OT_7;
	//Pull-up
	GPIOB->PUPDR|=GPIO_PUPDR_PUPDR3_0|GPIO_PUPDR_PUPDR4_0|GPIO_PUPDR_PUPDR5_0|GPIO_PUPDR_PUPDR6_0|GPIO_PUPDR_PUPDR7_0;
}	

void TFT_WriteIndex(u8 Index)
{
	TFT_CS_CLR;
	TFT_DELAY_1US;
	TFT_RS_CLR;
	TFT_DELAY_1US;
	TFT_WriteByte(Index);
	TFT_CS_SET;
	TFT_DELAY_1US;
}

void TFT_WriteData(u8 _data)
{
	TFT_CS_CLR;
	TFT_DELAY_1US;
	TFT_RS_SET;
	TFT_DELAY_1US;
	TFT_WriteByte(_data);
	TFT_CS_SET;
	TFT_DELAY_1US;
}

void TFT_WriteByte(u8 _data)
{
	u8 i=0;
	for(i=0;i<8;i++)
	{
		TFT_CLK_CLR;
		if(_data&0x80)	
			TFT_SDA_SET; //输出数据
		else 
			TFT_SDA_CLR;
		TFT_DELAY_1US;
		TFT_CLK_SET;
		TFT_DELAY_1US;
		_data<<=1; 
	}
	TFT_SDA_SET;	//拉高数据线
	TFT_CLK_CLR;	//拉低时钟线

}

/*
u16 TFT_ReadData_16Bit(void)
{
	u8 i=0;
	u16 _data=0;
	
	
	TFT_CS_CLR;		
	TFT_DELAY_1US;
	TFT_RS_SET;
	TFT_DELAY_1US;
	TFT_SDA_SET;
	TFT_CLK_CLR;
	TFT_DELAY_1US;
	for(i=0;i<16;i++)
	{
		_data<<=1;
		
		TFT_CLK_SET;
		TFT_DELAY_1US;
		
		if(TFT_SDA_READ)	
			_data|=0x0001;
		
		TFT_CLK_CLR;
		TFT_DELAY_1US;
		
	}
	TFT_CLK_CLR;
	TFT_CS_SET;		//拉高片选
	TFT_DELAY_1US;
	return _data;
}
*/

/*******************************************************
函数名：	TFT_Reset
提  要：	TFT复位
作  者：	佘晓凯
*******************************************************/
void TFT_Reset(void)
{
	TFT_RST_CLR;
	Delay_ms(50);
	TFT_RST_SET;
	Delay_ms(100);
}


void TFT_WriteData_16Bit(u16 _data)
{	
	TFT_WriteData(_data>>8);
	TFT_WriteData(_data);	
}

/*************************************************
函数名：TFT_WriteReg
功能：往寄存器中写入数据
提要：在48M主屏下执行时间约为0.15ms
入口参数：Index 寄存器地址，Data 数据
*************************************************/
void TFT_WriteReg(u8 Index,u16 Data)
{
	TFT_WriteIndex(Index);
  	TFT_WriteData_16Bit(Data);
}

void TFT_SetXY(u16 Xpos, u16 Ypos)
{
	TFT_WriteReg(0x20,Xpos);
	TFT_WriteReg(0x21,Ypos);
	TFT_WriteIndex(0x22);
}


/*************************************************
函数名：TFT_SetRegion
功能：设置lcd显示区域，在此区域写点数据自动换行
入口参数：xy起点和终点
返回值：无
*************************************************/
void TFT_SetRegion(u8 xStar, u8 yStar,u8 xEnd,u8 yEnd)
{
	TFT_WriteReg(0x38,xEnd);
	TFT_WriteReg(0x39,xStar);
	TFT_WriteReg(0x36,yEnd);
	TFT_WriteReg(0x37,yStar);
	TFT_WriteReg(0x21,xStar);
	TFT_WriteReg(0x20,yStar);
	TFT_WriteIndex(0x22);	
}

/*************************************************
函数名：Lcd_Clear
功能：全屏清屏函数
入口参数：填充颜色COLOR
返回值：无
*************************************************/
void TFT_Clear(u16 Color)               
{	
   unsigned int i,m;
   TFT_SetRegion(0,0,220-1,176-1);
   for(i=0;i<X_MAX_PIXEL;i++)
    for(m=0;m<Y_MAX_PIXEL;m++)
    {	
	  	TFT_WriteData_16Bit(Color);
    }   
}


void TFT_Display_Char(u16 x,u16 y,char c,u16 Color,u16 BackColor)
{
	u8 i,j;
	c=c-' ';	
	for(i=0;i<8;i++)
	{
		TFT_SetXY(x+i,y);
		for(j=0;j<8;j++)
		{
			
			if(ASCII_8x16[c][j]&(0x01<<i))
				TFT_WriteData_16Bit(Color);
			else
				TFT_WriteData_16Bit(BackColor);
		}
	}
	for(;i<16;i++)
	{
		TFT_SetXY(x+i,y);
		for(j=0;j<8;j++)
		{
			if(ASCII_8x16[c][j+8]&(0x01<<(i-8)))
				TFT_WriteData_16Bit(Color);
			else
				TFT_WriteData_16Bit(BackColor);
		}
	}
	
}

void TFT_Display_Str(u16 x,u16 y,char *str,u16 Color,u16 BackColor)
{	
	
	for(;*str != 0;str++)
	{
		TFT_Display_Char(x,y,*str,Color,BackColor);
		y+=8;
	}
}

