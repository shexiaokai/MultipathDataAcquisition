#include "BK2425.h"

const u8  BK2425_ADDR_P0[5]={0xE7,0xE7,0xE7,0xE7,0xE7};
const u8  BK2425_ADDR_P1[5]={0xC2,0xC2,0xC2,0xC2,0xC2};
const u8  BK2425_ADDR_P2_5[4]={0xC3,0xC4,0xC5,0xC6};


u8 BK2425_tx_buf[32] = {0,1,2,3};

/*******************************************************
��������	BK2425_GPIO_Init
��  Ҫ��	BK2425����ģ�����ų�ʼ��  IOģ��
��  �ߣ�	������
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
��������	BK2425_Init
��  Ҫ��	BK2425����ģ���ʼ��  IOģ��
		������ͷ�ļ����޸�
��  �ߣ�	������
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
�������ܣ�BK2425����ģʽ��ʼ��                      
��ڲ�������
����  ֵ����
*********************************************************/
void BK2425_RX_Mode(void)
{
	BK2425_CE_CLR;
	BK2425_Write_Buf(BK2425_WRITE_REG+RX_ADDR_P0, BK2425_ADDR_P0, 5);
	BK2425_Write_Reg(EN_AA, 0x01);       			// ʹ�ܽ���ͨ��0�Զ�Ӧ��
	BK2425_Write_Reg(EN_RXADDR, 0x01);   			// ʹ�ܽ���ͨ��0
	BK2425_Write_Reg(RF_CH, 40);         			// ѡ����Ƶͨ��0x40
	BK2425_Write_Reg(RX_PW_P0, 32); 
	BK2425_Write_Reg(RF_SETUP, 0x0f);    			// ���ݴ�����2Mbps�����书��7dBm
	
	BK2425_Write_Reg(CONFIG, 0x0f);         			// CRCʹ�ܣ�16λCRCУ�飬�ϵ磬����ģʽ
	BK2425_Write_Reg(STATUS, 0xff);  					// ������е��жϱ�־λ
	BK2425_CE_SET;                                     	// ����CE���������豸
}

/********************************************************
�������ܣ�BK2425����ģʽ��ʼ��                      
��ڲ�������
����  ֵ����
*********************************************************/
void BK2425_TX_Mode(void)
{
	BK2425_CE_CLR;
	BK2425_Write_Buf(BK2425_WRITE_REG+TX_ADDR, BK2425_ADDR_P0, 5);     // д�뷢�͵�ַ
	BK2425_Write_Buf(BK2425_WRITE_REG+RX_ADDR_P0, BK2425_ADDR_P0, 5);  // Ϊ��Ӧ������豸������ͨ��0��ַ�ͷ��͵�ַ��ͬ
	BK2425_Write_Reg(EN_AA, 0x01);       			// ʹ�ܽ���ͨ��0�Զ�Ӧ��
	BK2425_Write_Reg(EN_RXADDR, 0x01);   			// ʹ�ܽ���ͨ��0
	BK2425_Write_Reg(SETUP_RETR, 0x0a);  			// �Զ��ط���ʱ�ȴ�250us+86us���Զ��ط�10��
	BK2425_Write_Reg(RF_CH, 40);         			// ѡ����Ƶͨ��0x40
	BK2425_Write_Reg(RF_SETUP, 0x0f);    			// ���ݴ�����2Mbps�����书��7dBm
				
	BK2425_Write_Reg(CONFIG, 0x0e);      			// CRCʹ�ܣ�16λCRCУ�飬�ϵ�
	
	//BK2425_Read_Reg(EN_AA);
	
	//BK2425_CE_SET;    							// ����CE���������豸
}


/*******************************************************
��������	BK2425_SPI_RW
��  Ҫ��	BK2425����ģ��SPIд������  IOģ��
��  ����	byte����Ҫд�������
����ֵ��	byte��MISO�߷��ص�����
��  �ߣ�	������
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
		
		BK2425_SCK_SET; 			//����ʱ����
		BK2425_DELAY_1us;
		
		if(BK2425_MISO_READ)		
			byte |= 0x01;  		
		
		BK2425_SCK_CLR;				//����ʱ����
		BK2425_DELAY_1us;
	}
	BK2425_MOSI_CLR;
	BK2425_DELAY_1us;
	return(byte);                              
}

/********************************************************
�������ܣ�д�Ĵ�����ֵ�����ֽڣ�                  
��ڲ�����reg:�Ĵ���ӳ���ַ����ʽ��WRITE_REG��reg��
					pBuf:д�����׵�ַ
					bytes:д�����ֽ���
����  ֵ��״̬�Ĵ�����ֵ
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
�������ܣ�д�Ĵ�����ֵ�����ֽڣ�                
��ڲ�����reg:�Ĵ���ӳ���ַ����ʽ��WRITE_REG��reg��
					value:�Ĵ�����ֵ
����  ֵ��״̬�Ĵ�����ֵ
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
�������ܣ���ȡ�Ĵ�����ֵ�����ֽڣ�                  
��ڲ�����reg:�Ĵ���ӳ���ַ����ʽ��READ_REG��reg��
����  ֵ���Ĵ���ֵ
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
�������ܣ�����һ�����ݰ�                      
��ڲ�����txbuf:Ҫ���͵�����
����  ֵ��0x10:�ﵽ����ط�����������ʧ�� 
          0x20:���ͳɹ�            
          0xff:����ʧ��                  
*********************************************************/
u8 BK2425_TxPacket(u8 *txbuf)
{
	u8 state;
	BK2425_CE_CLR;		
	BK2425_DELAY_1us;	
	BK2425_Write_Buf(WR_TX_PLOAD, txbuf, 32);	    //д���ݵ�TX FIFO,32���ֽ�
	BK2425_DELAY_1us;
 	BK2425_CE_SET;																										  //CE�øߣ�ʹ�ܷ���	   
	
	while(BK2425_IQR_READ);			//�ȴ��������
	
	
	state = BK2425_Read_Reg(STATUS);  								//��ȡ״̬�Ĵ�����ֵ	   
	BK2425_Write_Reg(STATUS, state); 								//���TX_DS��MAX_RT�жϱ�־
	if(state&MAX_RT)												//�ﵽ����ط�����
	{
		BK2425_Write_Reg(FLUSH_TX,0xff);							//���TX FIFO�Ĵ��� 
		return MAX_RT; 
	}
	if(state&TX_DS)													//�������
	{
		return TX_DS;
	}
	return 0XFF;													//����ʧ��
}
