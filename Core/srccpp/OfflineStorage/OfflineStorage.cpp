/*
 * OfflineStorage.cpp
 *
 *  Created on: 09-Feb-2023
 *      Author: MKS
 */
#include "main.h"
#include "OfflineStorage.h"
#include <stdio.h>
#include <string.h>
using namespace std;

#define MAXNOOFBLOCK 32

extern "C" {
void W25qxx_WriteSector(uint8_t *pBuffer, uint32_t Sector_Address, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_SectorSize);
void W25qxx_ReadSector(uint8_t *pBuffer, uint32_t Sector_Address, uint32_t OffsetInByte, uint32_t NumByteToRead_up_to_SectorSize);
void W25qxx_EraseSector(uint32_t SectorAddr);
bool W25qxx_IsEmptyBlock(uint32_t Block_Address, uint32_t OffsetInByte, uint32_t NumByteToCheck_up_to_BlockSize);
void W25qxx_EraseBlock(uint32_t BlockAddr);
void W25qxx_EraseChip(void);
}

extern uint16_t MAC_Gen_Prod_Input1_Production,Production_Zeit;
extern uint16_t MAC_Gen_Rej_Input_Production,Rejection_Zeit;
extern uint8_t hour_t,min_t,sec_t;
extern uint8_t Wdate_Rtc,date_Rtc,month_Rtc,year_Rtc;
extern uint8_t Manual_RejectionCount;
extern uint8_t CurrentShift_K1,CurrentShift;
extern uint16_t Dye_Temperature;
extern uint16_t Connector_Temperature;
uint8_t ProductionSet_uintFormat[100]={0};
char ProductionSet_charFormat[100]={'\0'};
uint16_t MAC_Gen_Prod_Input1_Production_K1,MAC_Gen_Rej_Input_Production_K1,Manual_RejectionCount_K1;
uint16_t SectorPos;
uint8_t BlockPos;
uint8_t BlockPos_K1;
uint8_t Blockdataread[]={0};
uint8_t checkvar;
uint8_t BlockStatusOffline[40];
extern uint8_t IsCurrentShiftUpdated;
extern uint8_t UpdateStorage,updateMemProcCtrl;
extern uint16_t productionInc;
extern uint16_t requirementIdK1;
extern uint16_t productionTarget,requirementId;
extern uint8_t triggerStartForReq,startStopStatus;
extern uint8_t updateDwindata;
extern uint16_t machineId,machineIdK1;
extern uint16_t portNumber,portNumberK1;
extern uint8_t updateCloudConn;
extern uint8_t lengthOfServerAdd,lengthOfUserName,lengthOfPassword;
extern uint8_t serverAddress[20],serverAddressK1[20];
extern uint8_t userNameWifi[20],userNameWifiK1[20];
extern uint8_t passwordWifi[20],passwordWifiK1[20];
uint16_t productionIncK1;
uint8_t triggerStartForReqK1;
uint8_t startStopStatusK1;
uint8_t CMDATCWJAPUsernamePswGlobal[60];
uint8_t noOfByteUsrPsw;

//uint8_t Checkbuf[100];
OfflineStorage::OfflineStorage() {
	// TODO Auto-generated constructor stub

}

OfflineStorage::~OfflineStorage() {
	// TODO Auto-generated destructor stub
}

void OfflineStorage::run()
{
	if(UpdateStorage==0){return;}
	UpdateStorage=0;

	ProductionSet_charFormat[100]={'\0'};
	/*high limit check for system data*/
	CurrentShift = (CurrentShift > 4)?4:CurrentShift;
	MAC_Gen_Prod_Input1_Production = (MAC_Gen_Prod_Input1_Production >2)?0:MAC_Gen_Prod_Input1_Production;
	MAC_Gen_Rej_Input_Production =(MAC_Gen_Rej_Input_Production >2)?0:MAC_Gen_Rej_Input_Production;
	Dye_Temperature =(Dye_Temperature >30)?30:Dye_Temperature;
	Connector_Temperature =(Connector_Temperature>30)?30:Connector_Temperature;
	hour_t =(hour_t >25)?70:hour_t;
	min_t=(min_t > 70)?70:min_t;
	sec_t=(sec_t >70)?70:sec_t;
	date_Rtc=(date_Rtc >32)?70:date_Rtc;
	month_Rtc =(month_Rtc>13)?70:month_Rtc;
	year_Rtc = (year_Rtc>90)?70:year_Rtc;
	Manual_RejectionCount = (Manual_RejectionCount >2)?2:Manual_RejectionCount;
	Production_Zeit =(Production_Zeit>10)?1:Production_Zeit;
	Rejection_Zeit = (Rejection_Zeit>10)?1:Rejection_Zeit;
	SectorPos = (SectorPos > 900)?1:SectorPos;
/*high limit check for system data*/
	sprintf(ProductionSet_charFormat,"%c%02d,%04d,%04d,%03d,%03d,%02d:%02d:%02d %02d/%02d/%02d,%04d,%03d,%03d%c",'"',CurrentShift,MAC_Gen_Prod_Input1_Production,
			MAC_Gen_Rej_Input_Production,Dye_Temperature,Connector_Temperature,hour_t,
			min_t,sec_t,date_Rtc,month_Rtc,year_Rtc,Manual_RejectionCount,123,SectorPos,'"');

	//Fix the size
	NoofData = strlen(ProductionSet_charFormat);
	memcpy(ProductionSet_uintFormat,ProductionSet_charFormat,NoofData);

	SectorPos = SectorPos+1;

	if((SectorPos % 16)==0)
	{
		tempBlockcalc = SectorPos/16;
		BlockStatusOffline[tempBlockcalc-1]	=1;
		updateInBlockFill=1;
		if(tempBlockcalc >= MAXNOOFBLOCK)
		{
			tempBlockcalc=0;
			BlockStatusOffline[tempBlockcalc]	=2;
		}
		else
		{
			BlockStatusOffline[tempBlockcalc]	=2;
		}
		if(SectorPos >= 512){SectorPos=0;}
		SectorPos = SectorPos+1;
	}


	W25qxx_EraseSector(SectorPos-1);
	W25qxx_WriteSector(ProductionSet_uintFormat,(SectorPos-1),0,NoofData);
	MAC_Gen_Prod_Input1_Production=0;
	MAC_Gen_Rej_Input_Production=0;

	MAC_Gen_Prod_Input1_Production_K1 = MAC_Gen_Prod_Input1_Production;
	MAC_Gen_Rej_Input_Production_K1   = MAC_Gen_Rej_Input_Production;
	Manual_RejectionCount_K1          = Manual_RejectionCount;
	IsCurrentShiftUpdated=0;

	FlashMemProductiondata[0] = (uint8_t)MAC_Gen_Prod_Input1_Production&0x00ff;
	FlashMemProductiondata[1] = (uint8_t)(MAC_Gen_Prod_Input1_Production>>8)&0x00ff;
	FlashMemProductiondata[2] = (uint8_t)MAC_Gen_Prod_Input1_Production_K1&0x00ff;
	FlashMemProductiondata[3] = (uint8_t)(MAC_Gen_Prod_Input1_Production_K1>>8)&0x00ff;
	FlashMemProductiondata[4] = (uint8_t)MAC_Gen_Rej_Input_Production&0x00ff;
	FlashMemProductiondata[5] = (uint8_t)(MAC_Gen_Rej_Input_Production>>8)&0x00ff;
	FlashMemProductiondata[6] = (uint8_t)MAC_Gen_Rej_Input_Production_K1&0x00ff;
	FlashMemProductiondata[7] = (uint8_t)(MAC_Gen_Rej_Input_Production_K1>>8)&0x00ff;
	FlashMemProductiondata[8] = (uint8_t)Production_Zeit&0x00ff;
	FlashMemProductiondata[9] = (uint8_t)(Production_Zeit>>8)&0x00ff;
	FlashMemProductiondata[10] = (uint8_t)Rejection_Zeit&0x00ff;
	FlashMemProductiondata[11] = (uint8_t)(Rejection_Zeit>>8)&0x00ff;
	FlashMemProductiondata[12] = (hour_t);
	FlashMemProductiondata[13] = (min_t);
	FlashMemProductiondata[14] = (sec_t);
	FlashMemProductiondata[15] = date_Rtc;
	FlashMemProductiondata[16] = month_Rtc;
	FlashMemProductiondata[17] = year_Rtc;
	FlashMemProductiondata[18] = (uint8_t)SectorPos&0x00ff;
	FlashMemProductiondata[19] = (uint8_t)(SectorPos>>8)&0x00ff;
	FlashMemProductiondata[20] = (uint8_t)Manual_RejectionCount&0x00ff;
	FlashMemProductiondata[21] = (uint8_t)(Manual_RejectionCount>>8)&0x00ff;
	FlashMemProductiondata[22] = (uint8_t)Manual_RejectionCount_K1&0x00ff;
	FlashMemProductiondata[23] = (uint8_t)(Manual_RejectionCount_K1>>8)&0x00ff;
	FlashMemProductiondata[24] = CurrentShift;
	FlashMemProductiondata[25] = CurrentShift_K1;
	W25qxx_EraseSector(600);
	W25qxx_WriteSector(FlashMemProductiondata,600,0,26);

	if(updateInBlockFill)
	{
		updateInBlockFill=0;
		W25qxx_EraseSector(601);
		W25qxx_WriteSector(BlockStatusOffline,601,0,40);
	}

}
void OfflineStorage::specialMacDataWrite()
{
	if(productionInc != productionIncK1){
	specialMacData[0] = (uint8_t)productionInc&0x00ff;
	specialMacData[1] = (uint8_t)(productionInc>>8)&0x00ff;

	productionIncK1 = productionInc;
	W25qxx_EraseSector(602);
	W25qxx_WriteSector(specialMacData,602,0,2);
	}
}
void OfflineStorage::dwinRxDataStore()
{
	if(updateDwindata==0){return;}
	updateDwindata=0;
	dwinData[0] = (uint8_t)machineId&0x00ff;
	dwinData[1] = (uint8_t)(machineId>>8)&0x00ff;
	dwinData[2] = (uint8_t)portNumber&0x00ff;
	dwinData[3] = (uint8_t)(portNumber>>8)&0x00ff;

	W25qxx_EraseSector(604);
	W25qxx_WriteSector(dwinData,604,0,4);
}
void OfflineStorage::dwinCloudDataStore()
{
	uint8_t i;
	if(!updateCloudConn){return;}
	updateCloudConn=0;
	lengthOfServerAdd =(lengthOfServerAdd >20)?20:lengthOfServerAdd;
	for(i=0;i<20;i++){
		couldData[i]=serverAddress[i];
	}
	lengthOfUserName =(lengthOfUserName >20)?20:lengthOfUserName;
	for(i=0;i<20;i++){
		couldData[i+20]=userNameWifi[i];
	}
	lengthOfPassword =(lengthOfPassword >20)?20:lengthOfPassword;
	for(i=0;i<20;i++){
		couldData[i+40]=passwordWifi[i];
	}
	couldData[60] = lengthOfServerAdd;
	couldData[61] = lengthOfUserName;
	couldData[62] = lengthOfPassword;

	memcpy(serverAddressK1,serverAddress,20);
	memcpy(userNameWifiK1,userNameWifi,20);
	memcpy(passwordWifiK1,passwordWifi,20);

	W25qxx_EraseSector(605);
	W25qxx_WriteSector(couldData,605,0,63);

}
void OfflineStorage::processDataWrite()
{
	if((updateMemProcCtrl==1)||(triggerStartForReq != triggerStartForReqK1)||
		(startStopStatus != startStopStatusK1)){
	updateMemProcCtrl=0;
	processData[0] = (uint8_t)requirementId&0x00ff;
	processData[1] = (uint8_t)(requirementId>>8)&0x00ff;
	processData[2] = (uint8_t)requirementIdK1&0x00ff;
	processData[3] = (uint8_t)(requirementIdK1>>8)&0x00ff;
	processData[4] = (uint8_t)productionTarget&0x00ff;
	processData[5] = (uint8_t)(productionTarget>>8)&0x00ff;
	processData[6] = triggerStartForReq;
	processData[7] = startStopStatus;
	triggerStartForReqK1= triggerStartForReq;
	startStopStatusK1 = startStopStatus;

	W25qxx_EraseSector(603);
	W25qxx_WriteSector(processData,603,0,8);
	}
}

void OfflineStorage::dwinCloudDataRead()
{
	uint8_t i;
	uint8_t CMDATCWJAPUsernamePswHead[10]={'A','T','+','C','W','J','A','P','=','"'};
	uint8_t CMDATCWJAPUsernamePswBody[3]={'"',',','"'};
	uint8_t CMDATCWJAPUsernamePswTail[3]={'"','\r','\n'};
	uint8_t lenp,lenk;
	W25qxx_ReadSector(couldData,605,0,63);
	for(i=0;i<20;i++){
	serverAddress[i] = couldData[i];
	}
	for(i=0;i<20;i++){
	userNameWifi[i] = couldData[i+20];
	}
	for(i=0;i<20;i++){
	passwordWifi[i] = couldData[i+40];
	}
	lengthOfServerAdd = couldData[60];
	lengthOfUserName  =	couldData[61];
	lengthOfPassword = couldData[62];
	memcpy(serverAddressK1,serverAddress,20);
	memcpy(userNameWifiK1,userNameWifi,20);
	memcpy(passwordWifiK1,passwordWifi,20);

	noOfByteUsrPsw =0;
	for(lenp=noOfByteUsrPsw;lenp<10;lenp++){
		CMDATCWJAPUsernamePswGlobal[lenp] = CMDATCWJAPUsernamePswHead[lenp];}
	noOfByteUsrPsw=0+10;
	for(lenp=noOfByteUsrPsw,lenk=0;lenk<lengthOfUserName;lenp++,lenk++){
		CMDATCWJAPUsernamePswGlobal[lenp] = userNameWifi[lenk];
	}
	noOfByteUsrPsw=0+10+lengthOfUserName;
	for(lenp=noOfByteUsrPsw,lenk=0;lenk<3;lenp++,lenk++){
			CMDATCWJAPUsernamePswGlobal[lenp] = CMDATCWJAPUsernamePswBody[lenk];
	}
	noOfByteUsrPsw=0+10+lengthOfUserName+3;
	for(lenp=noOfByteUsrPsw,lenk=0;lenk<lengthOfPassword;lenp++,lenk++){
			CMDATCWJAPUsernamePswGlobal[lenp] = passwordWifi[lenk];
	}
	noOfByteUsrPsw=0+10+lengthOfUserName+3+lengthOfPassword;
	for(lenp=noOfByteUsrPsw,lenk=0;lenk<3;lenp++,lenk++){
				CMDATCWJAPUsernamePswGlobal[lenp] = CMDATCWJAPUsernamePswTail[lenk];
	}
	noOfByteUsrPsw=0+10+lengthOfUserName+3+lengthOfPassword+3;
}

void OfflineStorage::specialMacDataRead()
{
	W25qxx_ReadSector(specialMacData,602,0,2);
	productionInc = (specialMacData[1]<<8|specialMacData[0]);
	productionIncK1 = productionInc;
}

void OfflineStorage::dwinRxDataRead()
{
	W25qxx_ReadSector(specialMacData,604,0,4);
	machineId = (specialMacData[1]<<8|specialMacData[0]);
	portNumber = (specialMacData[3]<<8|specialMacData[2]);
	machineIdK1 = machineId;
	portNumberK1 = portNumber;
}

void OfflineStorage::processDataRead()
{
	W25qxx_ReadSector(processData,603,0,8);
	requirementId = (processData[1]<<8|processData[0]);
	requirementIdK1 = (processData[3]<<8|processData[2]);
	productionTarget = (processData[5]<<8|processData[4]);
	triggerStartForReq = processData[6];
	startStopStatus = processData[7];
	triggerStartForReqK1= triggerStartForReq;
	startStopStatusK1 = startStopStatus;
}

void OfflineStorage::ReadOfflinedataInit()
{
	W25qxx_ReadSector(FlashMemProductiondata,600,0,26);
	MAC_Gen_Prod_Input1_Production 		= 0;//(FlashMemProductiondata[1]<<8|FlashMemProductiondata[0]);
	MAC_Gen_Prod_Input1_Production_K1 	= (FlashMemProductiondata[3]<<8|FlashMemProductiondata[2]);
	MAC_Gen_Rej_Input_Production 		= 0;//(FlashMemProductiondata[5]<<8|FlashMemProductiondata[4]);
	MAC_Gen_Rej_Input_Production_K1 	= (FlashMemProductiondata[7]<<8|FlashMemProductiondata[6]);
	Production_Zeit 					= (FlashMemProductiondata[9]<<8|FlashMemProductiondata[8]);
	Rejection_Zeit 						= (FlashMemProductiondata[11]<<8|FlashMemProductiondata[10]);
	hour_t								= FlashMemProductiondata[12];
	min_t								= FlashMemProductiondata[13];
	sec_t								= FlashMemProductiondata[14];
	date_Rtc							= FlashMemProductiondata[15];
	month_Rtc							= FlashMemProductiondata[16];
	year_Rtc							= FlashMemProductiondata[17];
	SectorPos 							= (FlashMemProductiondata[19]<<8|FlashMemProductiondata[18]);
	Manual_RejectionCount				= 0;(FlashMemProductiondata[21]<<8|FlashMemProductiondata[20]);
	Manual_RejectionCount_K1	        = 0;(FlashMemProductiondata[23]<<8|FlashMemProductiondata[22]);
	CurrentShift						= FlashMemProductiondata[24];
	CurrentShift_K1						= FlashMemProductiondata[25];

	W25qxx_ReadSector(BlockStatusOffline,601,0,40);


	//if(SectorPos)

//	W25qxx_EraseBlock(23);
}

void OfflineStorage::ECUProductionInit(void)
{
	W25qxx_EraseChip();
	W25qxx_EraseSector(601);
	W25qxx_WriteSector(0x00,601,0,40);
	W25qxx_EraseSector(600);
	W25qxx_WriteSector(0x00,600,0,26);
	SectorPos=0;
}

