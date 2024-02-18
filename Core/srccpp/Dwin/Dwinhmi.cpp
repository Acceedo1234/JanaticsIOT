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
constexpr uint8_t multipleWriteRequestH = 0x0f;
constexpr uint8_t multipleWriteRequestL = 0x82;
constexpr uint8_t multipleReadRequestH = 0x04;
constexpr uint8_t multipleReadRequestL = 0x83;

constexpr uint8_t multipleWriteRequestF2H = 0x10;
constexpr uint8_t multipleWriteRequestF2L = 0x82;

extern uint8_t wifi_command;
extern uint8_t hour_t,min_t,sec_t;
extern uint8_t Wdate_Rtc,date_Rtc,month_Rtc,year_Rtc;
extern uint16_t Production_Total,Rejection_Total,productionInc;
extern uint16_t productionTarget;

extern uint8_t DwinDatabuffer[255];
extern uint8_t Rx_Dwin_Complete,startStopStatus;
extern uint8_t wifiConnection;
extern uint16_t batchNumber;
extern uint8_t itemNumber[11];
extern uint8_t espIpAddress[16];
extern uint8_t espMacAdress[20];

uint16_t machineId,machineIdK1;
uint16_t portNumber,portNumberK1;
uint8_t serverAddress[20],serverAddressK1[20];
uint8_t userNameWifi[20],userNameWifiK1[20];
uint8_t passwordWifi[20],passwordWifiK1[20];
uint8_t shiftP,len_i;
uint16_t dwinRxFramValid1,dwinRxFramValid2;
uint8_t updateDwindata;
uint8_t Dwinseq;
uint8_t lengthOfServerAdd,lengthOfUserName,lengthOfPassword;
uint8_t CheckP;
uint8_t updateCloudConn;
uint16_t espMacAddressLoc;
uint16_t dwinRxFramValidKey;

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
			u8ModbusRegisterdwin[22] = highByte(0);
			u8ModbusRegisterdwin[23] = lowByte(wifiConnection);//2008
			u8ModbusRegisterdwin[24] = highByte(0);
			u8ModbusRegisterdwin[25] = lowByte(wifiConnection);//2009
			u8ModbusRegisterdwin[26] = highByte(0);
			u8ModbusRegisterdwin[27] = lowByte(12);//200A
			u8ModbusRegisterdwin[28] = highByte(productionTarget);
			u8ModbusRegisterdwin[29] = lowByte(productionTarget);//200B
			u8ModbusRegisterdwin[30] = highByte(productionInc);
			u8ModbusRegisterdwin[31] = lowByte(productionInc);//200C
			u8ModbusRegisterdwin[32] = highByte(startStopStatus);
			u8ModbusRegisterdwin[33] = lowByte(startStopStatus);//200C
			u8ModbusRegisterdwin[34] = highByte(batchNumber);
			u8ModbusRegisterdwin[35] = lowByte(batchNumber);//200C
			u8ModbusRegisterdwin[36] = highByte(0);
			u8ModbusRegisterdwin[37] = lowByte(0);//200D
			u8ModbusRegisterdwin[38] = highByte(0);
			u8ModbusRegisterdwin[39] = lowByte(0);//200e
			u8ModbusRegisterdwin[40] = highByte(0);
			u8ModbusRegisterdwin[41] = lowByte(0);//200f
			u8ModbusRegisterdwin[42] = highByte(0);
			u8ModbusRegisterdwin[43] = lowByte(0);//2010
			u8ModbusRegisterdwin[44] = 'I';//itemNumber[0];
			u8ModbusRegisterdwin[45] = 'T';//itemNumber[1];//2011
			u8ModbusRegisterdwin[46] = 'E';//itemNumber[2];
			u8ModbusRegisterdwin[47] = 'M';//itemNumber[3];//2012
			u8ModbusRegisterdwin[48] = '1';//itemNumber[4];
			u8ModbusRegisterdwin[49] = 'T';//itemNumber[5];//2013
			u8ModbusRegisterdwin[50] = 'E';//itemNumber[6];
			u8ModbusRegisterdwin[51] = 'S';//itemNumber[7];//2014
			u8ModbusRegisterdwin[52] = '1';//itemNumber[8];
			u8ModbusRegisterdwin[53] = '2';//itemNumber[9];//2015
			noOfDataDwin=54;

			noOfDataDwin=36;
   			Cntid_dwin=1;
		break;
		case 1:
			u8ModbusRegisterdwin[0] = START_BYTE_1;
			u8ModbusRegisterdwin[1] = START_BYTE_2;
			u8ModbusRegisterdwin[2] = multipleWriteRequestH;
			u8ModbusRegisterdwin[3] = multipleWriteRequestL;
			u8ModbusRegisterdwin[4] = 0x20;
			u8ModbusRegisterdwin[5] = 0x27;
			u8ModbusRegisterdwin[6] = highByte(espMacAdress[0]);
			u8ModbusRegisterdwin[7] = lowByte(espMacAdress[0]);
			u8ModbusRegisterdwin[8] = highByte(espMacAdress[1]);
			u8ModbusRegisterdwin[9] = lowByte(espMacAdress[1]);
			u8ModbusRegisterdwin[10] = highByte(espMacAdress[2]);
			u8ModbusRegisterdwin[11] = lowByte(espMacAdress[2]);
			u8ModbusRegisterdwin[12] = highByte(espMacAdress[3]);
			u8ModbusRegisterdwin[13] = lowByte(espMacAdress[3]);
			u8ModbusRegisterdwin[14] = highByte(espMacAdress[4]);
			u8ModbusRegisterdwin[15] = lowByte(espMacAdress[4]);
			u8ModbusRegisterdwin[16] = highByte(espMacAdress[5]);
			u8ModbusRegisterdwin[17] = lowByte(espMacAdress[5]);

			noOfDataDwin=18;
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

	dwinRxFramValidKey = ((DwinDatabuffer[1]<<8)|DwinDatabuffer[2]);
	dwinRxFramValid1 = ((DwinDatabuffer[23]<<8)|DwinDatabuffer[24]);
	dwinRxFramValid2 = ((DwinDatabuffer[51]<<8)|DwinDatabuffer[52]);
	if((dwinRxFramValid1 > 0)&&(dwinRxFramValid2 > 0)&&(dwinRxFramValidKey==55)){
		dwinRxFramValidKey=0;
		machineId  = ((DwinDatabuffer[23]<<8)|DwinDatabuffer[24]);
		portNumber = ((DwinDatabuffer[51]<<8)|DwinDatabuffer[52]);
		memset(serverAddress,0,20);
		memset(userNameWifi,0,20);
		memset(passwordWifi,0,20);

		for(shiftP=3,len_i=0;shiftP<=22;shiftP++,len_i++){
			if(DwinDatabuffer[shiftP] == 0xff){lengthOfServerAdd = len_i;break;}
			serverAddress[len_i] = DwinDatabuffer[shiftP];
		}
		for(shiftP=27,len_i=0;shiftP<=49;shiftP++,len_i++){
			if(DwinDatabuffer[shiftP] == 0xff){lengthOfUserName = len_i;break;}
			userNameWifi[len_i] = DwinDatabuffer[shiftP];
		}
		for(shiftP=53,len_i=0;shiftP<=70;shiftP++,len_i++){
			if(DwinDatabuffer[shiftP] == 0xff){lengthOfPassword = len_i;break;}
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

		for(CheckP=0;CheckP<lengthOfServerAdd;CheckP++){
				if(serverAddress[CheckP] != serverAddressK1[CheckP]){
				updateCloudConn = 1;
				break;
			}
		}
		for(CheckP=0;CheckP<lengthOfUserName;CheckP++){
						if(userNameWifi[CheckP] != userNameWifiK1[CheckP]){
						updateCloudConn = 1;
						break;
			}
		}
		for(CheckP=0;CheckP<lengthOfPassword;CheckP++){
						if(passwordWifi[CheckP] != passwordWifiK1[CheckP]){
						updateCloudConn = 1;
						break;
			}
		}
  }
}

