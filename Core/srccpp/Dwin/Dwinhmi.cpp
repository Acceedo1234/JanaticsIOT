/*
 * Dwinhmi.cpp
 *
 *  Created on: Feb 1, 2024
 *      Author: MKS
 */

#include <Dwin/Dwinhmi.h>
#include "baselib.h"
#include <stdio.h>

extern UART_HandleTypeDef huart2;
constexpr uint8_t START_BYTE_1=0x5A;
constexpr uint8_t START_BYTE_2=0xA5;
constexpr uint8_t multipleWriteRequestH = 0x1C;
constexpr uint8_t multipleWriteRequestL = 0x82;
constexpr uint8_t multipleReadRequestH = 0x04;
constexpr uint8_t multipleReadRequestL = 0x83;

extern uint8_t wifi_command;
extern uint8_t hour_t,min_t,sec_t;
extern uint8_t Wdate_Rtc,date_Rtc,month_Rtc,year_Rtc;
extern uint16_t Production_Total,Rejection_Total,productionInc;
extern uint16_t productionTarget;

extern uint8_t DwinDatabuffer[255];
extern uint8_t Rx_Dwin_Complete,startStopStatus;
extern uint8_t wifiConnection;
extern uint16_t batchNumber;

uint16_t machineId,machineIdK1;
uint16_t portNumber,portNumberK1;
uint8_t serverAddress[20];
uint8_t userNameWifi[20];
uint8_t passwordWifi[20];
uint8_t shiftP,len_i;
uint8_t dwinRxFramValid;
uint8_t updateDwindata;
uint8_t Dwinseq;

Dwinhmi::Dwinhmi() {
	// TODO Auto-generated constructor stub

}

Dwinhmi::~Dwinhmi() {
	// TODO Auto-generated destructor stub
}

void Dwinhmi::dwinFrame()
{
	switch(Cntid_dwin)
	{
		case 0:
			u8ModbusRegisterdwin[0] = START_BYTE_1;
			u8ModbusRegisterdwin[1] = START_BYTE_2;
			u8ModbusRegisterdwin[2] = multipleWriteRequestH;
			u8ModbusRegisterdwin[3] = multipleWriteRequestL;
			u8ModbusRegisterdwin[4] = 0x20;
			u8ModbusRegisterdwin[5] = 0x00;
			u8ModbusRegisterdwin[6] = 0x00;
			u8ModbusRegisterdwin[7] = date_Rtc;
			u8ModbusRegisterdwin[8] = 0x00;
			u8ModbusRegisterdwin[9] = month_Rtc;
			u8ModbusRegisterdwin[10] = 0x00;
			u8ModbusRegisterdwin[11] = year_Rtc;
			u8ModbusRegisterdwin[12] = 0x00;
			u8ModbusRegisterdwin[13] = hour_t;
			u8ModbusRegisterdwin[14] = 0x00;
			u8ModbusRegisterdwin[15] = min_t;
			u8ModbusRegisterdwin[16] = 0x00;
			u8ModbusRegisterdwin[17] = sec_t;
			u8ModbusRegisterdwin[18] = highByte(Production_Total);
			u8ModbusRegisterdwin[19] = lowByte(Production_Total);//2006
			u8ModbusRegisterdwin[20] = highByte(Rejection_Total);
			u8ModbusRegisterdwin[21] = lowByte(Rejection_Total);//2007
			u8ModbusRegisterdwin[22] = highByte(0);
			u8ModbusRegisterdwin[23] = lowByte(wifiConnection);//2008
			u8ModbusRegisterdwin[24] = highByte(0);
			u8ModbusRegisterdwin[25] =lowByte(wifiConnection);//2009
			u8ModbusRegisterdwin[26] = highByte(0);
			u8ModbusRegisterdwin[27] = lowByte(12);//200A
			u8ModbusRegisterdwin[28] = highByte(productionInc);
			u8ModbusRegisterdwin[29] = lowByte(productionInc);//200B
			u8ModbusRegisterdwin[30] = highByte(productionTarget);
			u8ModbusRegisterdwin[31] = lowByte(productionTarget);//200C
			u8ModbusRegisterdwin[32] = highByte(startStopStatus);
			u8ModbusRegisterdwin[33] = lowByte(startStopStatus);//200C
			u8ModbusRegisterdwin[34] = highByte(batchNumber);
			u8ModbusRegisterdwin[35] = lowByte(batchNumber);//200C

			noOfDataDwin=36;
   			Cntid_dwin=1;
		break;
		case 1:
			u8ModbusRegisterdwin[0] = START_BYTE_1;
			u8ModbusRegisterdwin[1] = START_BYTE_2;
			u8ModbusRegisterdwin[2] = multipleWriteRequestH;
			u8ModbusRegisterdwin[3] = multipleWriteRequestL;
			u8ModbusRegisterdwin[4] = 0x20;
			u8ModbusRegisterdwin[5] = 0x00;
			u8ModbusRegisterdwin[6] = 0x00;
			u8ModbusRegisterdwin[7] = date_Rtc;
			u8ModbusRegisterdwin[8] = 0x00;
			u8ModbusRegisterdwin[9] = month_Rtc;
			u8ModbusRegisterdwin[10] = 0x00;
			u8ModbusRegisterdwin[11] = year_Rtc;
			u8ModbusRegisterdwin[12] = 0x00;
			u8ModbusRegisterdwin[13] = hour_t;
			u8ModbusRegisterdwin[14] = 0x00;
			u8ModbusRegisterdwin[15] = min_t;
			u8ModbusRegisterdwin[16] = 0x00;
			u8ModbusRegisterdwin[17] = sec_t;
			u8ModbusRegisterdwin[18] = highByte(Production_Total);
			u8ModbusRegisterdwin[19] = lowByte(Production_Total);
			u8ModbusRegisterdwin[20] = highByte(Rejection_Total);
			u8ModbusRegisterdwin[21] = lowByte(Rejection_Total);
			u8ModbusRegisterdwin[22] = highByte(0);
			u8ModbusRegisterdwin[23] = lowByte(wifiConnection);//2008
			u8ModbusRegisterdwin[24] = highByte(0);
			u8ModbusRegisterdwin[25] = lowByte(wifiConnection);//2009
			u8ModbusRegisterdwin[26] = highByte(0);
			u8ModbusRegisterdwin[27] = lowByte(12);//200A
			u8ModbusRegisterdwin[28] = highByte(productionInc);
			u8ModbusRegisterdwin[29] = lowByte(productionInc);//200B
			u8ModbusRegisterdwin[30] = highByte(productionTarget);
			u8ModbusRegisterdwin[31] = lowByte(productionTarget);//200C
			u8ModbusRegisterdwin[32] = highByte(startStopStatus);
			u8ModbusRegisterdwin[33] = lowByte(startStopStatus);//200C
			u8ModbusRegisterdwin[34] = highByte(batchNumber);
			u8ModbusRegisterdwin[35] = lowByte(batchNumber);//200C
			noOfDataDwin=36;
			Cntid_dwin=2;
		break;
		case 2:
			u8ModbusRegisterdwin[0] = START_BYTE_1;
			u8ModbusRegisterdwin[1] = START_BYTE_2;
			u8ModbusRegisterdwin[2] = multipleReadRequestH;
			u8ModbusRegisterdwin[3] = multipleReadRequestL;
			u8ModbusRegisterdwin[4] = 0x30;
			u8ModbusRegisterdwin[5] = 0x00;
			u8ModbusRegisterdwin[6] = 0x22;
			memset(DwinDatabuffer,0,255);
			Dwinseq=0;
			noOfDataDwin=7;
			Cntid_dwin=0;
		break;
		default:
		break;
	}
	HAL_UART_Transmit_IT(&huart2,u8ModbusRegisterdwin,noOfDataDwin);
}


void Dwinhmi::dwinDecoder()
{
	if(!Rx_Dwin_Complete){return;}
	Rx_Dwin_Complete=0;

	dwinRxFramValid = ((DwinDatabuffer[23]<<8)|DwinDatabuffer[24]);
	if(dwinRxFramValid > 0){
		machineId  = ((DwinDatabuffer[23]<<8)|DwinDatabuffer[24]);
		portNumber = ((DwinDatabuffer[51]<<8)|DwinDatabuffer[52]);
		for(shiftP=1,len_i=0;shiftP<=22;shiftP++,len_i++){
			if(DwinDatabuffer[shiftP] == 0xff){break;}
			serverAddress[len_i] = DwinDatabuffer[shiftP];
		}
		for(shiftP=27,len_i=0;shiftP<=49;shiftP++,len_i++){
			if(DwinDatabuffer[shiftP] == 0xff){break;}
			userNameWifi[len_i] = DwinDatabuffer[shiftP];
		}
		for(shiftP=53,len_i=0;shiftP<=70;shiftP++,len_i++){
			if(DwinDatabuffer[shiftP] == 0xff){break;}
			passwordWifi[len_i] = DwinDatabuffer[shiftP];
		}

		if(machineId != machineIdK1){
			updateDwindata = 1;
			machineIdK1 = machineId;
		}
		if(portNumber != portNumberK1){
			updateDwindata = 1;
			portNumberK1 = portNumber;
		}

	}
}

