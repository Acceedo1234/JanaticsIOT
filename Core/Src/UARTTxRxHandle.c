/*
 * UARTTxRxHandle.c
 *
 *  Created on: Jan 21, 2023
 *      Author: MKS
 */
#include"main.h"

extern UART_HandleTypeDef hlpuart1;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern uint8_t rxTempBuff[5];
extern uint8_t rxDwinBuff[5];
extern uint8_t u8rxbuf[255];
extern uint8_t Dwinseq;
uint8_t checkbuff[200];
uint8_t refinc;

uint16_t Dye_Temperature;
uint16_t Connector_Temperature;
uint8_t check[210],trackpoint;

uint8_t NoOfDwinRxdata,Rx_Dwin_Data_Buff_Point,Rx_Dwin_Complete;
uint8_t DwinDatabuffer[255];

extern uint8_t Rxseqdecoder;
extern void ESPRxDecoder(unsigned char Rxwifi_data,unsigned char Rxseqdecoder);
void DwinFrameDecode(uint8_t Dwindatarx);


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

	if(huart == &huart2)
	{
		DwinFrameDecode(rxDwinBuff[0]);
		check[trackpoint]=rxDwinBuff[0];
		if(++trackpoint>200){trackpoint=0;}
		HAL_UART_Receive_IT(&huart2,rxDwinBuff,1);
	}
}

void DwinFrameDecode(uint8_t Dwindatarx){

	switch(Dwinseq)
	{
		case 0:
			if(Dwindatarx == 0x5A){
				Dwinseq=1;
			}
			else
			{
				Dwinseq=0;
			}
		break;
		case 1:
			if(Dwindatarx == 0xA5){
				Dwinseq=2;
			}
			else
			{
				Dwinseq=0;
			}
		break;
		case 2:
			NoOfDwinRxdata = Dwindatarx-3;
			Dwinseq=3;
		break;
		case 3:
			if(Dwindatarx == 0x83){
			Dwinseq=4;}
			else{
				Dwinseq=0;
			}
		break;
		case 4:
			if(Dwindatarx == 0x30){
				Dwinseq=5;
			}
			else
			{
				Dwinseq=0;
			}
		break;
		case 5:
			if(Dwindatarx == 0x00){
				Dwinseq=6;
				Rx_Dwin_Data_Buff_Point=0;
			}
			else
			{
				Dwinseq=0;
			}
		break;
		case 6:
				DwinDatabuffer[Rx_Dwin_Data_Buff_Point]=Dwindatarx;
				NoOfDwinRxdata= NoOfDwinRxdata-1;
				Rx_Dwin_Data_Buff_Point = Rx_Dwin_Data_Buff_Point+1;
				if(NoOfDwinRxdata == 0){
					Dwinseq=0;
					Rx_Dwin_Complete=1;
				}
		break;
		default:
		break;
	}
}

