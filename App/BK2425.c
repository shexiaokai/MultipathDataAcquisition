#include "BK2425.h"

const u8  BK2425_ADDR_P0[5]={0xE7,0xE7,0xE7,0xE7,0xE7};
const u8  BK2425_ADDR_P1[5]={0xC2,0xC2,0xC2,0xC2,0xC2};
const u8  BK2425_ADDR_P2_5[4]={0xC3,0xC4,0xC5,0xC6};


u8 BK2425_tx_buf[32] = {0,1,2,3};

/*******************************************************
函数名：	BK2425_GPIO_Init
提  要：	BK2425无线模块引脚初始化  IO模拟
作  者：	佘晓凯
*******************************************************/
void BK2425_GPIO_Init(void)
{
	BK2425_CE_SET;
	BK2425_SCN_SET;
	BK2425_SCK_CLR;
	BK2425_MOSI_SET;
	
	// General purpose output mode
	GPIOB->MODER |= GPIO_MODER_MODER10_0|GPIO_MODER_MODER11_0|\
					GPIO_MODER_MODER13_0|GPIO_MODER_MODER15_0;
	// Output open-drain
	GPIOB->OTYPER|=	GPIO_OTYPER_OT_10|GPIO_OTYPER_OT_11|\
					GPIO_OTYPER_OT_12|GPIO_OTYPER_OT_13|\
					GPIO_OTYPER_OT_14|GPIO_OTYPER_OT_15;
	//Pull-up
	GPIOB->PUPDR |=	GPIO_PUPDR_PUPDR10_0|GPIO_PUPDR_PUPDR11_0|\
					GPIO_PUPDR_PUPDR12_0|GPIO_PUPDR_PUPDR13_0|\
					GPIO_PUPDR_PUPDR14_0|GPIO_PUPDR_PUPDR15_0;
	
	
}

/*******************************************************
函数名：	BK2425_Init
提  要：	BK2425无线模块初始化  IO模拟
		引脚在头文件中修改
作  者：	佘晓凯
*******************************************************/
void BK2425_Init(void)
{
	void BK2425_GPIO_Init(void);
	
	BK2425_GPIO_Init();
	BK2425_CE_CLR;
	BK2425_DELAY_1us;

	BK2425_RX_Mode();
	BK2425_CE_SET;  
}

/********************************************************
函数功能：BK2425接收模式初始化                      
入口参数：无
返回  值：无
*********************************************************/
void BK2425_RX_Mode(void)
{
	BK2425_CE_CLR;
	BK2425_Write_Buf(BK2425_WRITE_REG+RX_ADDR_P0, BK2425_ADDR_P0, 5);
	BK2425_Write_Reg(EN_AA, 0x01);       			// 使能接收通道0自动应答
	BK2425_Write_Reg(EN_RXADDR, 0x01);   			// 使能接收通道0
	BK2425_Write_Reg(RF_CH, 40);         			// 选择射频通道0x40
	BK2425_Write_Reg(RX_PW_P0, 32); 
	BK2425_Write_Reg(RF_SETUP, 0x0f);    			// 数据传输率2Mbps，发射功率7dBm
	
	BK2425_Write_Reg(CONFIG, 0x0f);         			// CRC使能，16位CRC校验，上电，接收模式
	BK2425_Write_Reg(STATUS, 0xff);  					// 清除所有的中断标志位
	BK2425_CE_SET;                                     	// 拉高CE启动接收设备
}

/********************************************************
函数功能：BK2425发送模式初始化                      
入口参数：无
返回  值：无
*********************************************************/
void BK2425_TX_Mode(void)
{
	BK2425_CE_CLR;
	BK2425_Write_Buf(BK2425_WRITE_REG+TX_ADDR, BK2425_ADDR_P0, 5);     // 写入发送地址
	BK2425_Write_Buf(BK2425_WRITE_REG+RX_ADDR_P0, BK2425_ADDR_P0, 5);  // 为了应答接收设备，接收通道0地址和发送地址相同
	BK2425_Write_Reg(EN_AA, 0x01);       			// 使能接收通道0自动应答
	BK2425_Write_Reg(EN_RXADDR, 0x01);   			// 使能接收通道0
	BK2425_Write_Reg(SETUP_RETR, 0x0a);  			// 自动重发延时等待250us+86us，自动重发10次
	BK2425_Write_Reg(RF_CH, 40);         			// 选择射频通道0x40
	BK2425_Write_Reg(RF_SETUP, 0x0f);    			// 数据传输率2Mbps，发射功率7dBm
				
	BK2425_Write_Reg(CONFIG, 0x0e);      			// CRC使能，16位CRC校验，上电
	
	//BK2425_Read_Reg(EN_AA);
	
	//BK2425_CE_SET;    							// 拉高CE启动接收设备
}


/*******************************************************
函数名：	BK2425_SPI_RW
提  要：	BK2425无线模块SPI写入或读出  IO模拟
参  数：	byte：需要写入的数据
返回值：	byte：MISO线返回的数据
作  者：	佘晓凯
*******************************************************/
u8 BK2425_SPI_RW(u8 byte)
{
	u8 i;
	BK2425_DELAY_1us;
	for(i=0; i<8; i++)
	{
		if(byte & 0x80)
			BK2425_MOSI_SET;
		else
			BK2425_MOSI_CLR;																 
		byte = (byte << 1);     
		BK2425_DELAY_1us;
		
		BK2425_SCK_SET; 			//拉高时钟线
		BK2425_DELAY_1us;
		
		if(BK2425_MISO_READ)		
			byte |= 0x01;  		
		
		BK2425_SCK_CLR;				//拉低时钟线
		BK2425_DELAY_1us;
	}
	BK2425_MOSI_CLR;
	BK2425_DELAY_1us;
	return(byte);                              
}

/********************************************************
函数功能：写寄存器的值（多字节）                  
入口参数：reg:寄存器映射地址（格式：WRITE_REG｜reg）
					pBuf:写数据首地址
					bytes:写数据字节数
返回  值：状态寄存器的值
*********************************************************/
u8 BK2425_Write_Buf(u8 reg, const u8 *pBuf, u8 bytes)
{
	u8 status,byte_ctr;

	BK2425_SCN_CLR;  
	BK2425_DELAY_1us;
	status = BK2425_SPI_RW(reg);    
	
	for(byte_ctr=0; byte_ctr<bytes; byte_ctr++)     
		BK2425_SPI_RW(*pBuf++);
	
	BK2425_DELAY_1us;
	BK2425_SCN_SET;                                    	
	BK2425_DELAY_1us;
	return(status);       
}			
/********************************************************
函数功能：写寄存器的值（单字节）                
入口参数：reg:寄存器映射地址（格式：WRITE_REG｜reg）
					value:寄存器的值
返回  值：状态寄存器的值
*********************************************************/
u8 BK2425_Write_Reg(u8 reg, u8 value)
{
	u8 status;

	BK2425_SCN_CLR;  
  	BK2425_DELAY_1us;
	status = BK2425_SPI_RW(BK2425_WRITE_REG|reg);	
	BK2425_DELAY_1us;
	BK2425_SPI_RW(value);
	BK2425_DELAY_1us;
	BK2425_SCN_SET; 
	
	return(status);
}

/********************************************************
函数功能：读取寄存器的值（单字节）                  
入口参数：reg:寄存器映射地址（格式：READ_REG｜reg）
返回  值：寄存器值
*********************************************************/
u8 BK2425_Read_Reg(u8 reg)
{
 	u8 value;

	BK2425_SCN_CLR;    
	BK2425_SPI_RW(BK2425_READ_REG|reg);			
	value = BK2425_SPI_RW(0);
	BK2425_SCN_SET;               

	return(value);
}


/********************************************************
函数功能：发送一个数据包                      
入口参数：txbuf:要发送的数据
返回  值：0x10:达到最大重发次数，发送失败 
          0x20:发送成功            
          0xff:发送失败                  
*********************************************************/
u8 BK2425_TxPacket(u8 *txbuf)
{
	u8 state;
	BK2425_CE_CLR;		
	BK2425_DELAY_1us;	
	BK2425_Write_Buf(WR_TX_PLOAD, txbuf, 32);	    //写数据到TX FIFO,32个字节
	BK2425_DELAY_1us;
 	BK2425_CE_SET;																										  //CE置高，使能发送	   
	
	while(BK2425_IQR_READ);			//等待发送完成
	
	
	state = BK2425_Read_Reg(STATUS);  								//读取状态寄存器的值	   
	BK2425_Write_Reg(STATUS, state); 								//清除TX_DS或MAX_RT中断标志
	if(state&MAX_RT)												//达到最大重发次数
	{
		BK2425_Write_Reg(FLUSH_TX,0xff);							//清除TX FIFO寄存器 
		return MAX_RT; 
	}
	if(state&TX_DS)													//发送完成
	{
		return TX_DS;
	}
	return 0XFF;													//发送失败
}
