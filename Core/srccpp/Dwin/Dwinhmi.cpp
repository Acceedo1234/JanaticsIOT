/*
 * Dwinhmi.cpp
 *
 *  Created on: Feb 1, 2024
 *      Author: MKS
 */

#include <Dwin/Dwinhmi.h>
#include "baselib.h"

extern UART_HandleTypeDef huart2;
constexpr uint8_t START_BYTE_1=0x5A;
constexpr uint8_t START_BYTE_2=0xA5;
constexpr uint8_t multipleWriteRequestH = 0x17;
constexpr uint8_t multipleWriteRequestL = 0x82;
constexpr uint8_t multipleReadRequestH = 0x04;
constexpr uint8_t multipleReadRequestL = 0x83;

extern uint8_t wifi_command;
extern uint8_t hour_t,min_t,sec_t;
extern uint8_t Wdate_Rtc,date_Rtc,month_Rtc,year_Rtc;
extern uint16_t Production_Total,Rejection_Total;

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
			u8ModbusRegisterdwin[19] = lowByte(Production_Total);
			u8ModbusRegisterdwin[20] = highByte(Rejection_Total);
			u8ModbusRegisterdwin[21] = lowByte(Rejection_Total);
			u8ModbusRegisterdwin[23] = 0;
			u8ModbusRegisterdwin[24] = 1;//wifi
			u8ModbusRegisterdwin[25] = 0;
			u8ModbusRegisterdwin[26] = 1;
			u8ModbusRegisterdwin[27] = 0;
			u8ModbusRegisterdwin[28] = 1;

			noOfDataDwin=29;
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
			u8ModbusRegisterdwin[23] = 0;
			u8ModbusRegisterdwin[24] = 1;//wifi
			u8ModbusRegisterdwin[25] = 0;
			u8ModbusRegisterdwin[26] = 1;
			u8ModbusRegisterdwin[27] = 0;
			u8ModbusRegisterdwin[28] = 1;

			noOfDataDwin=29;
			Cntid_dwin=2;
		break;
		case 2:
			u8ModbusRegisterdwin[0] = START_BYTE_1;
			u8ModbusRegisterdwin[1] = START_BYTE_2;
			u8ModbusRegisterdwin[2] = multipleReadRequestH;
			u8ModbusRegisterdwin[3] = multipleReadRequestL;
			u8ModbusRegisterdwin[4] = 0x30;
			u8ModbusRegisterdwin[5] = 0x00;
			u8ModbusRegisterdwin[6] = 0x0a;
			Dwinseq=0;
			noOfDataDwin=7;
			Cntid_dwin=0;
		break;
		default:
		break;
	}
	HAL_UART_Transmit_IT(&huart2,u8ModbusRegisterdwin,noOfDataDwin);
}

