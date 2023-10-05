/*
 * UARTTxRxHandle.c
 *
 *  Created on: Jan 21, 2023
 *      Author: MKS
 */
#include"main.h"

extern UART_HandleTypeDef hlpuart1;
extern UART_HandleTypeDef huart1;
extern uint8_t rxTempBuff[5];
extern uint8_t u8rxbuf[255];
uint8_t checkbuff[200];
uint8_t refinc;

uint16_t Dye_Temperature;
uint16_t Connector_Temperature;


extern uint8_t Rxseqdecoder;
extern void ESPRxDecoder(unsigned char Rxwifi_data,unsigned char Rxseqdecoder);

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &hlpuart1)
	{
		ESPRxDecoder(rxTempBuff[0],Rxseqdecoder);

		HAL_UART_Receive_IT(&hlpuart1,rxTempBuff,1);
	}

	if(huart == &huart1)
	{
		if((u8rxbuf[0]==1)&&(u8rxbuf[1]==3)&&(u8rxbuf[2]==8))
		{
			Dye_Temperature = (u8rxbuf[3]<<8|u8rxbuf[4]);
			Dye_Temperature = Dye_Temperature/10;
		}
		if((u8rxbuf[0]==2)&&(u8rxbuf[1]==3)&&(u8rxbuf[2]==8))
		{
			Connector_Temperature = (u8rxbuf[3]<<8|u8rxbuf[4]);
			Connector_Temperature = Connector_Temperature/10;
		}
	}
}



