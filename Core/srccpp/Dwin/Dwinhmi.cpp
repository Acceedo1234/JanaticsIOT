/*
 * Dwinhmi.cpp
 *
 *  Created on: Feb 1, 2024
 *      Author: MKS
 */

#include <Dwin/Dwinhmi.h>
#include "baselib.h"

constexpr uint8_t START_BYTE_1=0x5A;
constexpr uint8_t START_BYTE_2=0xA5;
constexpr uint8_t multipleWriteRequestH = 0x04;
constexpr uint8_t multipleWriteRequestL = 0x82;
constexpr uint8_t multipleReadRequestH = 0x04;
constexpr uint8_t multipleReadRequestL = 0x83;

extern UART_HandleTypeDef huart2;

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
			u8ModbusRegisterdwin[6] = highByte(0);
			u8ModbusRegisterdwin[7] = lowByte(0);
			noOfDataDwin=8;
			Cntid_dwin=1;
		break;
		case 1:
			u8ModbusRegisterdwin[0] = START_BYTE_1;
			u8ModbusRegisterdwin[1] = START_BYTE_2;
			u8ModbusRegisterdwin[2] = multipleWriteRequestH;
			u8ModbusRegisterdwin[3] = multipleWriteRequestL;
			u8ModbusRegisterdwin[4] = 0x20;
			u8ModbusRegisterdwin[5] = 0x00;
			u8ModbusRegisterdwin[6] = highByte(0);
			u8ModbusRegisterdwin[7] = lowByte(0);
			noOfDataDwin=8;
			Cntid_dwin=0;
		break;
		default:
		break;
	}
	HAL_UART_Transmit_IT(&huart2,u8ModbusRegisterdwin,noOfDataDwin);
}

