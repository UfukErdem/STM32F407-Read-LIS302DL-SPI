#include "stm32f4xx.h"               

void GPIOD_Config(void);
uint8_t spi_oku(uint8_t adress);
void spi_yaz(uint8_t adress,uint8_t data);
void spi_init(void);
uint32_t i;
uint8_t x,y,z;
int main(void)
{
	GPIOD_Config();
	spi_init();
	spi_yaz(0x20,0x67);
	
	while(1)
	{
		x = spi_oku(0x29);
		y = spi_oku(0x2B);
		z = spi_oku(0x2D);
		i = 6000000;
		while(i--);
		GPIO_ToggleBits(GPIOD , GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
		
	}

}

void GPIOD_Config(void)
{
	
	GPIO_InitTypeDef GPIO_Register;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	GPIO_Register.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Register.GPIO_OType = GPIO_OType_PP;
	GPIO_Register.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Register.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Register.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOD, &GPIO_Register);

}

uint8_t spi_oku(uint8_t adress)
{
	GPIO_ResetBits(GPIOE,GPIO_Pin_3);
	adress = 0x80 | adress;
	while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI1,adress);
	while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE));
	SPI_I2S_ReceiveData(SPI1);
	
	while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI1,0x00);
	while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE));
	
	GPIO_SetBits(GPIOE,GPIO_Pin_3);
	return SPI_I2S_ReceiveData(SPI1);
}

void spi_yaz(uint8_t adress,uint8_t data)
{
	
	GPIO_ResetBits(GPIOE,GPIO_Pin_3);
	adress = 0x00 | adress;
	while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI1,adress);
	while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE));
	SPI_I2S_ReceiveData(SPI1);
	
	while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI1,data);
	while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE));
	SPI_I2S_ReceiveData(SPI1);
	
	GPIO_SetBits(GPIOE,GPIO_Pin_3);
}

void spi_init(void)
{
	SPI_InitTypeDef SPI_Structer;
	GPIO_InitTypeDef GPIO_Register;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	GPIO_Register.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Register.GPIO_OType = GPIO_OType_PP;
	GPIO_Register.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Register.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Register.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_Register);
	
	GPIO_Register.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Register.GPIO_OType = GPIO_OType_PP;
	GPIO_Register.GPIO_Pin = GPIO_Pin_3;
	GPIO_Register.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Register.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_Register);
	
	SPI_Structer.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_Structer.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_Structer.SPI_Mode = SPI_Mode_Master;
	SPI_Structer.SPI_DataSize = SPI_DataSize_8b;
	SPI_Structer.SPI_NSS = SPI_NSS_Soft;
	SPI_Structer.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_Structer.SPI_CPOL = SPI_CPOL_High;
	SPI_Structer.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_Init(SPI1,&SPI_Structer);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1); 
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1); 
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1); 
	GPIO_SetBits(GPIOE,GPIO_Pin_3);
	SPI_Cmd(SPI1,ENABLE);
	
}
