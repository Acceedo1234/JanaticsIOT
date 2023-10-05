/*
 * w5500_spi.c
 *
 *  Created on: Jan 10, 2023
 *      Author: MKS
 */

#include "main.h"
#include "wizchip_conf.h"
#include "stdio.h"

extern SPI_HandleTypeDef hspi2;
uint16_t wiz5500_var;


uint8_t SPIReadwrite(uint8_t data)
{
	//wait untill fifo has free slot
	while((hspi2.Instance ->SR & SPI_FLAG_TXE) != SPI_FLAG_TXE);

	*(__IO uint8_t*)&hspi2.Instance -> DR = data;

	//Now wait untill data arrives
	while((hspi2.Instance ->SR & SPI_FLAG_RXNE) != SPI_FLAG_RXNE);

	return(*(__IO uint8_t*)&hspi2.Instance -> DR);
}


void wizchip_select(void)
{
	HAL_GPIO_WritePin(GPIOB, W5500_CS_Pin, GPIO_PIN_RESET);
}

void wizchip_deselect(void)
{
	HAL_GPIO_WritePin(GPIOB, W5500_CS_Pin, GPIO_PIN_SET);
}

uint8_t wizchip_read()
{
	uint8_t rb;
	rb = SPIReadwrite(0x00);
	return(rb);
}

void wizchip_write(uint8_t* wb)
{
	SPIReadwrite(wb);
	//HAL_SPI_TransmitReceive(hspi2,&wb,0x00,1,1000);
}

void wizchip_readburst(uint8_t* pbuf,uint8_t len)
{
	for(uint16_t i=0;i<= len;i++)
	{
		*pbuf = SPIReadwrite(0x00);
		 pbuf++;
	}
}

void wizchip_writeburst(uint8_t* pbuf,uint8_t len)
{
	for(uint16_t i=0;i<= len;i++)
	{
		SPIReadwrite(*pbuf);
		 pbuf++;
	}
}

void wiz5500Init(void)
{
	uint8_t memsize[2][8]={{2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}};
	//hardware reset the module
	HAL_GPIO_WritePin(W5500RST_GPIO_Port, W5500RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(15000);
	HAL_GPIO_WritePin(W5500RST_GPIO_Port, W5500RST_Pin, GPIO_PIN_SET);

	reg_wizchip_cs_cbfunc(wizchip_select,wizchip_deselect);
	reg_wizchip_spi_cbfunc(wizchip_read,wizchip_write);
//	reg_wizchip_spiburst_cbfunc(wizchip_readburst,wizchip_writeburst);
	wiz5500_var = 0;
	//Wizchip initialization
	if(ctlwizchip(CW_INIT_WIZCHIP,(void*)memsize) == -1){
		while(1);
//    		//failure in initialization
		wiz5500_var = 1;
	}

}



