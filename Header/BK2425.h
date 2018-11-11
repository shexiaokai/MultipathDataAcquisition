#ifndef BK2425_H
#define BK2425_H


#include "Main.h"

#define BK2425_CE_SET 		GPIOB->ODR |=  GPIO_ODR_11
#define BK2425_CE_CLR 		GPIOB->ODR &= ~GPIO_ODR_11
#define BK2425_SCN_SET 		GPIOB->ODR |=  GPIO_ODR_10
#define BK2425_SCN_CLR 		GPIOB->ODR &= ~GPIO_ODR_10
#define BK2425_SCK_SET 		GPIOB->ODR |=  GPIO_ODR_13
#define BK2425_SCK_CLR 		GPIOB->ODR &= ~GPIO_ODR_13
#define BK2425_MOSI_SET 	GPIOB->ODR |=  GPIO_ODR_15
#define BK2425_MOSI_CLR 	GPIOB->ODR &= ~GPIO_ODR_15

#define BK2425_IQR_READ 	(GPIOB->IDR &  GPIO_ODR_12)
#define BK2425_MISO_READ 	(GPIOB->IDR &  GPIO_ODR_14)

#define BK2425_READ_REG        0x00  // Define read command to register
#define BK2425_WRITE_REG       0x20  // Define write command to register

#define BK2425_DELAY_1us	Delay_us(1)



//********************************************************************************************************************//
// STATUS Register 
#define RX_DR						0x40  /**/
#define TX_DS						0x20
#define MAX_RT						0x10

//BK2425¼Ä´æÆ÷µØÖ·
#define CONFIG          0x00  // 'Config' register address
#define EN_AA           0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR       0x02  // 'Enabled RX addresses' register address
#define SETUP_AW        0x03  // 'Setup address width' register address
#define SETUP_RETR      0x04  // 'Setup Auto. Retrans' register address
#define RF_CH           0x05  // 'RF channel' register address
#define RF_SETUP        0x06  // 'RF setup' register address
#define STATUS          0x07  // 'Status' register address
#define OBSERVE_TX      0x08  // 'Observe TX' register address
#define CD              0x09  // 'Carrier Detect' register address
#define RX_ADDR_P0      0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1      0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2      0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3      0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4      0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5      0x0F  // 'RX address pipe5' register address
#define TX_ADDR         0x10  // 'TX address' register address
#define RX_PW_P0        0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1        0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2        0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3        0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4        0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5        0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS     0x17  // 'FIFO Status Register' register address

//********************************************************************************************************************//
// SPI(BK2425) commands
#define READ_REG        0x00  // Define read command to register
#define WRITE_REG       0x20  // Define write command to register
#define RD_RX_PLOAD     0x61  // Define RX payload register address
#define WR_TX_PLOAD     0xA0  // Define TX payload register address
#define FLUSH_TX        0xE1  // Define flush TX register command
#define FLUSH_RX        0xE2  // Define flush RX register command
#define REUSE_TX_PL     0xE3  // Define reuse TX payload register command
#define NOP             0xFF  // Define No Operation, might be used to read status register


void BK2425_Init(void);
void BK2425_TX_Mode(void);
void BK2425_RX_Mode(void);
u8 BK2425_SPI_RW(u8);
u8 BK2425_Write_Reg(u8,u8);
u8 BK2425_Write_Buf(u8,const u8*,u8);
u8 BK2425_Read_Reg(u8);
u8 BK2425_TxPacket(u8*);
#endif
