/*
 * esp8266_rx.c
 *
 *  Created on: Jan 21, 2023
 *      Author: MKS
 */
#include "esp8266_rx.h"

extern uint8_t refinc;
extern uint8_t RefreshBlockInfo;//update at rx end
uint8_t alarmOnOff;
uint8_t SW_Hour,SW_Minute;
uint8_t SW_Date,SW_Month,SW_Year;
uint16_t temperatureSetOL,temperaturehighSetOL,temperatureLowSetOL;
uint16_t productiontimeSetOL=18;
uint8_t productionhysPosSetOL,productionhysNegSetOL,SWCurrentShift,productChangeOL;
uint16_t Production_Total,Rejection_Total;
uint16_t batchTargetquantity,temperateSet,temperateSetHigh,temperateSetLow;
uint16_t requirementId;
uint16_t batchNumber;
uint8_t resetStatus,startStopStatus,reasonEntryStatus;
uint8_t commFeedbackFlag;
uint8_t itemNumber[11];
uint8_t as;
uint8_t espIpAddress[16]={0};
uint8_t len_espIpAddress=0;
uint8_t esp8266IpObtained=0;
uint8_t tempbuff[100];
uint8_t ipConfimation=0;

extern void W25qxx_ReadSector(uint8_t *pBuffer, uint32_t Sector_Address, uint32_t OffsetInByte, uint32_t NumByteToRead_up_to_SectorSize);
extern void W25qxx_WriteSector(uint8_t *pBuffer, uint32_t Sector_Address, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_SectorSize);
extern void W25qxx_EraseSector(uint32_t SectorAddr);

uint8_t wifiusernamecheck[15] = {'N','A','V','E','E','N','P','H','O','N','E'};

void ReadOnlineData(void);

void ESPRXDataSeg(void)
{
	if(!RxCompleteU2C1WIFI){return;}
	RxCompleteU2C1WIFI=0;

	Production_Total  	= Uart_rx_buffer[19]+(Uart_rx_buffer[18]*10)+(Uart_rx_buffer[17]*100)+(Uart_rx_buffer[16]*1000)+(Uart_rx_buffer[15]*10000);//Production
	Rejection_Total 	= Uart_rx_buffer[24]+(Uart_rx_buffer[23]*10)+(Uart_rx_buffer[22]*100)+(Uart_rx_buffer[21]*1000)+(Uart_rx_buffer[20]*10000);//Rejection
	batchTargetquantity = Uart_rx_buffer[29]+(Uart_rx_buffer[28]*10)+(Uart_rx_buffer[27]*100)+(Uart_rx_buffer[26]*1000)+(Uart_rx_buffer[25]*10000); //
	temperateSet 		= Uart_rx_buffer[32]+(Uart_rx_buffer[31]*10)+(Uart_rx_buffer[30]*100);//temperateSet
	temperateSetHigh 	= Uart_rx_buffer[35]+(Uart_rx_buffer[34]*10)+(Uart_rx_buffer[33]*100);//temperateSetHigh
	temperateSetLow 	= Uart_rx_buffer[38]+(Uart_rx_buffer[37]*10)+(Uart_rx_buffer[36]*100);//temperateSetLow
	requirementId	 	= Uart_rx_buffer[44]+(Uart_rx_buffer[43]*10)+(Uart_rx_buffer[42]*100)+(Uart_rx_buffer[41]*1000)+(Uart_rx_buffer[40]*10000)+(Uart_rx_buffer[39]*100000);//requirementId

	batchNumber  	 = Uart_rx_buffer[57]+(Uart_rx_buffer[56]*10)+(Uart_rx_buffer[55]*100);
	resetStatus  	 = Uart_rx_buffer[59]+(Uart_rx_buffer[58]*10);
	startStopStatus  = Uart_rx_buffer[61]+(Uart_rx_buffer[60]*10);
	reasonEntryStatus  = Uart_rx_buffer[63]+(Uart_rx_buffer[62]*10);
	//Date,Month,Year
	SW_Date = Uart_rx_buffer[4]+(Uart_rx_buffer[3]*10);//SW_Date
	SW_Month = Uart_rx_buffer[6]+(Uart_rx_buffer[5]*10);//SW_Month
	SW_Year = Uart_rx_buffer[8]+(Uart_rx_buffer[7]*10);//SW_Year
	SW_Hour = Uart_rx_buffer[10]+(Uart_rx_buffer[9]*10);//SW_Hour
	SW_Minute = Uart_rx_buffer[12]+(Uart_rx_buffer[11]*10);//SW_Minute

	itemNumber[0] = Uart_rx_buffer[45];
	itemNumber[1] = Uart_rx_buffer[46];
	itemNumber[0] = Uart_rx_buffer[47];
	itemNumber[0] = Uart_rx_buffer[48];
	itemNumber[0] = Uart_rx_buffer[49];
	itemNumber[0] = Uart_rx_buffer[50];
	itemNumber[0] = Uart_rx_buffer[51];
	itemNumber[0] = Uart_rx_buffer[52];
	itemNumber[0] = Uart_rx_buffer[53];
	itemNumber[0] = Uart_rx_buffer[54];

	commFeedbackFlag = 1;
	if((esp8266IpObtained==0)&&(ipConfimation==1)){
		for(as = 25;as<= 41;as++){
			if(tempbuff[as] == 34)//"
			{esp8266IpObtained=1;break;}
			++len_espIpAddress;
			espIpAddress[as-25]= tempbuff[as];
		}
	}

}

void ReadOnlineData(void)
{
	uint8_t FlashReadOLdata[10];
	W25qxx_ReadSector(FlashReadOLdata,602,0,10);
	temperatureSetOL 		= (FlashReadOLdata[1]<<8|FlashReadOLdata[0]);
	temperaturehighSetOL 	= (FlashReadOLdata[3]<<8|FlashReadOLdata[2]);
	temperatureLowSetOL 	= (FlashReadOLdata[5]<<8|FlashReadOLdata[4]);
	productiontimeSetOL 	= (FlashReadOLdata[7]<<8|FlashReadOLdata[6]);
	productionhysPosSetOL 	= FlashReadOLdata[8];
	productionhysNegSetOL   = FlashReadOLdata[9];
}


void ESPRxDecoder(unsigned char Rxwifi_data,unsigned char Rxseqdecoder)
{

   static unsigned char Error_Retry;
	 switch(Rxseqdecoder)
	   {
	   	case 1:
			 if((Rxwifi_data=='O')&&(bufferptr==0))
			 {
			 	bufferptr=1;
			 }
			 else if((Rxwifi_data=='K')&&(bufferptr==1))
			 {
			 	bufferptr=0;
				Rxseqdecoder=0;
				wifi_command=30;
//				WifiStatusVar = 1;
				Check_CWMODE_For_Hang=0;
			 }
			 else{;}

			 if((Rxwifi_data=='n')&&(bufferptr==0))
			 {
			 	bufferptr=1;
			 }
			 else if((Rxwifi_data=='o')&&(bufferptr==1))
			 {
			 	bufferptr=2;
			 }
			 else if(bufferptr==2)
			 {
			 	bufferptr=3;
			 }
			 else if((Rxwifi_data=='c')&&(bufferptr==3))
			 {
			 	bufferptr=4;
			 }
			 else if((Rxwifi_data=='h')&&(bufferptr==4))
			 {
			 	bufferptr=5;
			 }
			 else if((Rxwifi_data=='a')&&(bufferptr==5))
			 {
			 	bufferptr=6;
			 }
			 else if((Rxwifi_data=='n')&&(bufferptr==6))
			 {
			 	bufferptr=7;
			 }
			 else if((Rxwifi_data=='g')&&(bufferptr==7))
			 {
			 	bufferptr=8;
			 }
			 else if((Rxwifi_data=='e')&&(bufferptr==8))
			 {
			 	bufferptr=0;
				Rxseqdecoder=0;
				wifi_command=30;
				Check_CWMODE_For_Hang=0;
			 }
			 else{;}
		break;
		case 2:
			if((Rxwifi_data=='O')&&(bufferptr==0))
			 {
			 	bufferptr=1;
			 }
			 else if((Rxwifi_data=='K')&&(bufferptr==1))
			 {
			 	bufferptr=0;
				Rxseqdecoder=0;
				if(esp8266IpObtained==0){
					wifi_command=121;}
				else{
					wifi_command=49;}
			 }
		break;
		case 3:
			//wifiusernamecheck[15] "define the username at top"
			//
			if((Rxwifi_data== wifiusernamecheck[0])&&(bufferptr==0))
			 {
				bufferptr=1;
			 }
			 else if((Rxwifi_data== wifiusernamecheck[1])&&(bufferptr==1))
			 {
				bufferptr=2;
			 }
			 else if((Rxwifi_data== wifiusernamecheck[2])&&(bufferptr==2))
			 {
				bufferptr=3;
			 }

			 else if((Rxwifi_data== wifiusernamecheck[3])&&(bufferptr==3))
			 {
				bufferptr=4;
			 }
			 else if((Rxwifi_data== wifiusernamecheck[4])&&(bufferptr==4))
			 {
				bufferptr=5;
			 }
			 else if((Rxwifi_data== wifiusernamecheck[5])&&(bufferptr==5))
			 {
				bufferptr=6;
			 }
			 else if((Rxwifi_data== wifiusernamecheck[6])&&(bufferptr==6))
			 {
				bufferptr=7;
			 }
			 else if((Rxwifi_data== wifiusernamecheck[7])&&(bufferptr==7))
			 {
				bufferptr=8;
			 }
			 else if((Rxwifi_data== wifiusernamecheck[8])&&(bufferptr==8))
			 {
				bufferptr=9;
			 }
			 else if((Rxwifi_data== wifiusernamecheck[9])&&(bufferptr==9))
			 {
				 bufferptr=0;
				Err_bufferptr=0;
				Rxseqdecoder=0;
				wifi_command=70;
				Error_Retry=0;
				WifiDisplay = 1;
			 }
			 else if((Rxwifi_data== wifiusernamecheck[10])&&(bufferptr==10))
			 {
				bufferptr=11;
			 }
			 else if((Rxwifi_data== wifiusernamecheck[11])&&(bufferptr==11))
			 {
				//bufferptr=12;
				bufferptr=0;
				Err_bufferptr=0;
				Rxseqdecoder=0;
				wifi_command=70;
				Error_Retry=0;
				WifiDisplay = 1;
			 }
			 else if((Rxwifi_data=='A')&&(bufferptr==12))
			 {
				bufferptr=13;
			 }
			 else if((Rxwifi_data=='X')&&(bufferptr==13))
				 {
				bufferptr=0;
				Err_bufferptr=0;
				Rxseqdecoder=0;
				wifi_command=70;
				Error_Retry=0;
				WifiDisplay = 1;
//				WifiStatusVar = 2;
			 }



			 if((Rxwifi_data=='N')&&(Err_bufferptr==0))
			 {
			 	Err_bufferptr=1;
			 }
			 else if((Rxwifi_data=='o')&&(Err_bufferptr==1))
			 {
			 	Err_bufferptr=2;
			 }
			 else if((Rxwifi_data=='A')&&(Err_bufferptr==2))
			 {
			 	Err_bufferptr=3;
			 }
			 else if((Rxwifi_data=='P')&&(Err_bufferptr==3))
			 {
			 	Err_bufferptr=0;
				bufferptr=0;
				Rxseqdecoder=0;
				WifiDisplay = 3;
				if(++Error_Retry<3)		//retry for error
				{
				   wifi_command=50;
				}
				else
				{
				   wifi_command=10;
				   Error_Retry=0;
				}
			 }
		break;
		case 4:	   //retry need to be added
			 if((Rxwifi_data=='O')&&(bufferptr==0))
			 {
			 	bufferptr=1;
			 }
			 else if((Rxwifi_data=='K')&&(bufferptr==1))
			 {
			 	bufferptr=0;
				Rxseqdecoder=0;
				wifi_command=80;
			 }
		break;
		case 5:

			if((Rxwifi_data=='C')&&(bufferptr==0))
			 {
			 	bufferptr=1;
			 }
			 else if((Rxwifi_data=='O')&&(bufferptr==1))
			 {
			 	bufferptr=2;
			 }
			 else if((Rxwifi_data=='N')&&(bufferptr==2))
			 {
			 	bufferptr=3;
			 }
			 else if((Rxwifi_data=='N')&&(bufferptr==3))
			 {
			 	bufferptr=4;
			 }
			 else if((Rxwifi_data=='E')&&(bufferptr==4))
			 {
			 	bufferptr=5;
			 }
			 else if((Rxwifi_data=='C')&&(bufferptr==5))
			 {
			 	bufferptr=6;
			 }
			 else if((Rxwifi_data=='T')&&(bufferptr==6))
			 {
			 	Rxseqdecoder=0;
				wifi_command=90;
				bufferptr=0;
				Error_Retry=0;
				Err_bufferptr=0; //wifi connected
			   WifiDisplay = 2;
			 }
			 	if((Rxwifi_data=='L')&&(bufferptr==0))	  //linked
			 {
			 	bufferptr=1;
			 }
			 else if((Rxwifi_data=='i')&&(bufferptr==1))
			 {
			 	bufferptr=2;
			 }
			 else if((Rxwifi_data=='n')&&(bufferptr==2))
			 {
			 	bufferptr=3;
			 }
			 else if((Rxwifi_data=='k')&&(bufferptr==3))
			 {
			 	bufferptr=4;
			 }
			 else if((Rxwifi_data=='e')&&(bufferptr==4))
			 {
			 	bufferptr=5;
			 }
			 else if((Rxwifi_data=='d')&&(bufferptr==5))
			 {
			 	Rxseqdecoder=0;
				wifi_command=90;
				bufferptr=0;
				Error_Retry=0;
				Err_bufferptr=0;
			 }
			 if((Rxwifi_data=='E')&&(Err_bufferptr==0))
			 {
			 	Err_bufferptr=1;
			 }
			 else if((Rxwifi_data=='R')&&(Err_bufferptr==1))
			 {
			 	Err_bufferptr=2;
			 }
			 else if((Rxwifi_data=='R')&&(Err_bufferptr==2))
			 {
			 	Err_bufferptr=3;
			 }
			 else if((Rxwifi_data=='O')&&(Err_bufferptr==3))
			 {
			 	Err_bufferptr=4;
			 }
			 else if((Rxwifi_data=='R')&&(Err_bufferptr==4))
			 {
			   	Err_bufferptr=0;
				bufferptr=0;
				Rxseqdecoder=0;
				WifiDisplay = 4;
				if(++Error_Retry<3)		//retry for error
				{
				   wifi_command=80;
				}
				else
				{
				   wifi_command=50;	 // modified by satheesh
				   Error_Retry=0;
				}
			 }
		break;
		case 6:
			 if((Rxwifi_data=='>')&&(bufferptr==0))
			 {
			 	bufferptr=0;
				Rxseqdecoder=0;
				wifi_command=100;
			 }
		break;
		case 7:
			if((Rxwifi_data=='$')&&(Data_bufferptr==0))
			{
				 Data_bufferptr=1;
				 Valid_DataWifi1= 1;
			}
			else if(Data_bufferptr==1)
			 {
				Uart_rx_buffer[0] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=2;
			}
			else if(Data_bufferptr==2)
			{
				 Uart_rx_buffer[1] = DecToASCIIFun(Rxwifi_data);
				 Data_bufferptr=3;
			}
			else if(Data_bufferptr==3)//,
			{
				 Data_bufferptr=4;
				 Uart_rx_buffer[2] = DecToASCIIFun(Rxwifi_data);//cyclic time
			}
			else if(Data_bufferptr==4)
			{
				 Data_bufferptr=5;
			}
			else if(Data_bufferptr==5)
			{
				Uart_rx_buffer[3] = DecToASCIIFun(Rxwifi_data);
				 Data_bufferptr=6;
			}
			else if(Data_bufferptr==6)
			{
				Uart_rx_buffer[4] = DecToASCIIFun(Rxwifi_data);//date
				Data_bufferptr=7;
			}
			else if(Data_bufferptr==7)// '/'
			{
				 Data_bufferptr=8;
			}
			else if(Data_bufferptr==8)
			{
				 Uart_rx_buffer[5] = DecToASCIIFun(Rxwifi_data);
				 Data_bufferptr=9;
			}
			else if(Data_bufferptr==9)
			{
				Uart_rx_buffer[6] = DecToASCIIFun(Rxwifi_data);
				 Data_bufferptr=10;//month
			}
			else if(Data_bufferptr==10)
			{
				 Data_bufferptr=11;//month
			}

			else if(Data_bufferptr==11)// '/'
			 {
				Uart_rx_buffer[7] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=12;
			}
			else if(Data_bufferptr==12)////year
			{
				Uart_rx_buffer[8] = DecToASCIIFun(Rxwifi_data);
				 Data_bufferptr=13;
			}
			else if(Data_bufferptr==13)//_
			{

				 Data_bufferptr=14;
			}
			else if(Data_bufferptr==14)//
			{
				Uart_rx_buffer[9] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=15;
			}
			else if(Data_bufferptr==15)
			{
				Uart_rx_buffer[10] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=16;
			}

			else if(Data_bufferptr==16)
			 {

				Data_bufferptr=17;//hour
			}
			else if(Data_bufferptr==17)//:
			{
				Uart_rx_buffer[11] = DecToASCIIFun(Rxwifi_data);
				 Data_bufferptr=18;//minute
			}
			else if(Data_bufferptr==18)
			{
				Uart_rx_buffer[12] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=19;
			}
			else if(Data_bufferptr==19)
			{

				 Data_bufferptr=20;
			}
			else if(Data_bufferptr==20)//:
			{
				Data_bufferptr=21;
				Uart_rx_buffer[13] = DecToASCIIFun(Rxwifi_data);
			}

			else if(Data_bufferptr==21)       	//seconds
			 {
				Uart_rx_buffer[14] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=22;
			}
			else if(Data_bufferptr==22)
			{

				 Data_bufferptr=23;
			}
			else if(Data_bufferptr==23)      //,(to seperate data)
			{
				 Data_bufferptr=24;
				 Uart_rx_buffer[15] = DecToASCIIFun(Rxwifi_data);
			}
			else if(Data_bufferptr==24)
			{
				Uart_rx_buffer[16] = DecToASCIIFun(Rxwifi_data);
				 Data_bufferptr=25;
			}
			else if(Data_bufferptr==25)
			{
				Uart_rx_buffer[17] = DecToASCIIFun(Rxwifi_data);
				 Data_bufferptr=26;
			}
			else if(Data_bufferptr==26)
			{
				 Data_bufferptr=27;
				 Uart_rx_buffer[18] = DecToASCIIFun(Rxwifi_data);
			}
			else if(Data_bufferptr==27)
			 {
				Uart_rx_buffer[19] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=28;//production
			}
			else if(Data_bufferptr==28)//,
			{

				 Data_bufferptr=29;
			}
			else if(Data_bufferptr==29)//rejection
			{
				 Data_bufferptr=30;
				 Uart_rx_buffer[20] = DecToASCIIFun(Rxwifi_data);
			}
			else if(Data_bufferptr==30)
			{
				Uart_rx_buffer[21] = DecToASCIIFun(Rxwifi_data);
				 Data_bufferptr=31;
			}
			else if(Data_bufferptr==31)
			{
				 Data_bufferptr=32;
				 Uart_rx_buffer[22] = DecToASCIIFun(Rxwifi_data);
			}
			else if(Data_bufferptr==32)
			{
				 Uart_rx_buffer[23] = DecToASCIIFun(Rxwifi_data);
				 Data_bufferptr=33;
			}
			else if(Data_bufferptr==33)//rejection calculated in software
			{
				 Uart_rx_buffer[24] = DecToASCIIFun(Rxwifi_data);
				 Data_bufferptr=34;
		/*		RefreshBlockInfo = 0;
				RxCompleteU2C1WIFI=1;
				Updatetimeinfo=1;*/
			}
			else if(Data_bufferptr==34)      //,
			{
				Data_bufferptr=35;
			}
			else if(Data_bufferptr==35)      //batchTargetquantity
			 {
				Data_bufferptr=36;
				Uart_rx_buffer[25] = DecToASCIIFun(Rxwifi_data);
			}
			else if(Data_bufferptr==36)
			 {
				Uart_rx_buffer[26] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=37;
			}
			else if(Data_bufferptr==37)
			{
				Uart_rx_buffer[27] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=38;
			}
			else if(Data_bufferptr==38)
			{
				Uart_rx_buffer[28] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=39;
			}
			else if(Data_bufferptr==39)
			{
				Uart_rx_buffer[29] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=40;
			}
			else if(Data_bufferptr==40)//,
			{
				Data_bufferptr=41;
			}
			else if(Data_bufferptr==41)//temperateSet
			{
				Uart_rx_buffer[30] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=42;
			}
			else if(Data_bufferptr==42)
			{
				Uart_rx_buffer[31] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=43;
			}
			else if(Data_bufferptr==43)
			{
				Uart_rx_buffer[32] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=44;
			}
			else if(Data_bufferptr==44)//,
			{
				Data_bufferptr=45;
			}
			else if(Data_bufferptr==45)//temperateSetHigh
			{
				Uart_rx_buffer[33] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=46;
			}
			else if(Data_bufferptr==46)
			{
				Uart_rx_buffer[34] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=47;
			}
			else if(Data_bufferptr==47)
			{
				Uart_rx_buffer[35] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=48;
			}
			else if(Data_bufferptr==48)//,
			{
				Data_bufferptr=49;
			}
			else if(Data_bufferptr==49)//temperateSetLow
			{
				Uart_rx_buffer[36] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=50;
			}
			else if(Data_bufferptr==50)
			{
				Uart_rx_buffer[37] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=51;
			}
			else if(Data_bufferptr==51)
			{
				Uart_rx_buffer[38] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=52;
			}
			else if(Data_bufferptr==52)//,
			{
				Data_bufferptr=53;
			}
			else if(Data_bufferptr==53)//requirementId
			{
				Uart_rx_buffer[39] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=54;
			}
			else if(Data_bufferptr==54)
			{
				Uart_rx_buffer[40] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=55;
			}
			else if(Data_bufferptr==55)
			{
				Uart_rx_buffer[41] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=56;
			}
			else if(Data_bufferptr==56)
			{
				Uart_rx_buffer[42] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=57;
			}
			else if(Data_bufferptr==57)
			{
				Uart_rx_buffer[43] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=58;
			}
			else if(Data_bufferptr==58)
			{
				Uart_rx_buffer[44] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=59;
			}
			else if(Data_bufferptr==59)//,
			{
				Data_bufferptr=60;
			}
			else if(Data_bufferptr==60)//itemNumber
			{
				Uart_rx_buffer[45] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=61;
			}
			else if(Data_bufferptr==61)
			{
				Uart_rx_buffer[46] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=62;
			}
			else if(Data_bufferptr==62)
			{
				Uart_rx_buffer[47] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=63;
			}
			else if(Data_bufferptr==63)
			{
				Uart_rx_buffer[48] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=64;
			}
			else if(Data_bufferptr==64)
			{
				Uart_rx_buffer[49] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=65;
			}
			else if(Data_bufferptr==65)
			{
				Uart_rx_buffer[50] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=66;
			}
			else if(Data_bufferptr==66)
			{
				Uart_rx_buffer[51] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=67;
			}
			else if(Data_bufferptr==67)
			{
				Uart_rx_buffer[52] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=68;
			}
			else if(Data_bufferptr==68)
			{
				Uart_rx_buffer[53] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=69;
			}
			else if(Data_bufferptr==69)
			{
				Uart_rx_buffer[54] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=70;
			}
			else if(Data_bufferptr==70)//,
			{
				Data_bufferptr=71;
			}
			else if(Data_bufferptr==71)//batchNumber
			{
				Uart_rx_buffer[55] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=72;
			}
			else if(Data_bufferptr==72)
			{
				Uart_rx_buffer[56] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=73;
			}
			else if(Data_bufferptr==73)
			{
				Uart_rx_buffer[57] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=74;
			}
			else if(Data_bufferptr==74)//,
			{
				Data_bufferptr=75;
			}
			else if(Data_bufferptr==75)//resetStatus
			{
				Uart_rx_buffer[58] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=76;
			}
			else if(Data_bufferptr==76)
			{
				Uart_rx_buffer[59] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=77;
			}
			else if(Data_bufferptr==77)//,
			{
				Data_bufferptr=78;
			}
			else if(Data_bufferptr==78)//Reset status
			{
				Uart_rx_buffer[60] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=79;
			}
			else if(Data_bufferptr==79)
			{
				Uart_rx_buffer[61] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=80;
			}
			else if(Data_bufferptr==80)//,
			{
				Data_bufferptr=81;
			}
			else if(Data_bufferptr==81)//Reason entry status
			{
				Uart_rx_buffer[62] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=82;
			}
			else if(Data_bufferptr==82)
			{
				Uart_rx_buffer[63] = DecToASCIIFun(Rxwifi_data);
				Data_bufferptr=0;
				RefreshBlockInfo = 0;
				RxCompleteU2C1WIFI=1;
				Updatetimeinfo=1;
			}


		   if(Valid_DataWifi1)
		   {
			 if((Rxwifi_data=='U')&&(bufferptr==0))
			 {
			 	bufferptr=1;
			 }
			 else if((Rxwifi_data=='n')&&(bufferptr==1))
			 {
			 	bufferptr=2;
			 }
			 else if((Rxwifi_data=='l')&&(bufferptr==2))
			 {
			    bufferptr=3;
			 }
			 else if((Rxwifi_data=='i')&&(bufferptr==3))
			 {
			    bufferptr=4;
			 }
			 else if((Rxwifi_data=='n')&&(bufferptr==4))
			 {
			    bufferptr=5;
			 }
			 else if((Rxwifi_data=='k')&&(bufferptr==5))
			 {
			    bufferptr=0;
				Rxseqdecoder=0;
				wifi_command=102;
				//RefreshBlockInfo=0;//update at rx end
				Error_Close=0;
				Valid_DataWifi1=0;
				//RxCompleteU2C1WIFI=1;			  /*receive complete*/
			 }
			 if((Rxwifi_data=='C')&&(bufferptr==0))
			 {
			 	bufferptr=1;
			 }
			 else if((Rxwifi_data=='L')&&(bufferptr==1))
			 {
			 	bufferptr=2;
			 }
			 else if((Rxwifi_data=='O')&&(bufferptr==2))
			 {
			    bufferptr=3;
			 }
			 else if((Rxwifi_data=='S')&&(bufferptr==3))
			 {
			    bufferptr=4;
			 }
			 else if((Rxwifi_data=='E')&&(bufferptr==4))
			 {
			    bufferptr=5;
			 }
			 else if((Rxwifi_data=='D')&&(bufferptr==5))
			 {
			    bufferptr=0;
				Rxseqdecoder=0;
				wifi_command=102;
				//RefreshBlockInfo=0;//update at rx end
				Error_Close=0;
				Valid_DataWifi1=0;
				//RxCompleteU2C1WIFI=1;				  /*receive complete*/
			 }
			}
			else
			{
			  if((Rxwifi_data=='U')&&(bufferptr==0))
			 {
			 	bufferptr=1;
			 }
			 else if((Rxwifi_data=='n')&&(bufferptr==1))
			 {
			 	bufferptr=2;
			 }
			 else if((Rxwifi_data=='l')&&(bufferptr==2))
			 {
			    bufferptr=3;
			 }
			 else if((Rxwifi_data=='i')&&(bufferptr==3))
			 {
			    bufferptr=4;
			 }
			 else if((Rxwifi_data=='n')&&(bufferptr==4))
			 {
			    bufferptr=5;
			 }
			 else if((Rxwifi_data=='k')&&(bufferptr==5))
			 {
			    bufferptr=0;
				Rxseqdecoder=0;
				wifi_command=102;
				//RefreshBlockInfo=0;//update at rx end
				Error_Close=0;
				Valid_DataWifi1=0;
				//RxCompleteU2C1WIFI=1;				    /*receive complete*/
			 }

			   if((Rxwifi_data=='C')&&(bufferptr==0))
			 {
			 	bufferptr=1;
			 }
			 else if((Rxwifi_data=='L')&&(bufferptr==1))
			 {
			 	bufferptr=2;
			 }
			 else if((Rxwifi_data=='O')&&(bufferptr==2))
			 {
			    bufferptr=3;
			 }
			 else if((Rxwifi_data=='S')&&(bufferptr==3))
			 {
			    bufferptr=4;
			 }
			 else if((Rxwifi_data=='E')&&(bufferptr==4))
			 {
			    bufferptr=5;
			 }
			 else if((Rxwifi_data=='D')&&(bufferptr==5))
			 {
			    bufferptr=0;
				Rxseqdecoder=0;
				if(++Error_Close>3){wifi_command=10;Error_Close=0;}
				else{
					//RefreshBlockInfo=0;//update at rx end
					wifi_command=102;}
				Valid_DataWifi1=0;
				//RxCompleteU2C1WIFI=0;   /*Wrong receive*/
			 }
			}
		break;
		case 8:
			 if((Rxwifi_data=='O')&&(bufferptr==0))
			 {
			 	bufferptr=1;
			 }
			 else if((Rxwifi_data=='K')&&(bufferptr==1))
			 {
			 	bufferptr=0;
				Rxseqdecoder=0;
				wifi_command=20;
			 }
		break;
		case 9:
			tempbuff[refinc] = Rxwifi_data;
			refinc= refinc+1;
			if((Rxwifi_data=='O')&&(bufferptr==0))
			 {
				bufferptr=1;
			 }
			 else if((Rxwifi_data=='K')&&(bufferptr==1))
			 {
				bufferptr=0;
				Rxseqdecoder=0;
				ipConfimation=1;
				wifi_command=49;
			 }
		break;
	   	default:
		break;
		}
}

unsigned char DecToASCIIFun(unsigned char TempVar)
{
	unsigned char  ASCIIHex;
	switch(TempVar)
	{
	case 48:
			ASCIIHex = 0x00;
			break;
	case 49:
			ASCIIHex = 0x01;
			break;
	case 50:
			ASCIIHex = 0x02;
			break;
	case 51:
			ASCIIHex = 0x03;
			break;
	case 52:
			ASCIIHex = 0x04;
			break;
	case 53:
			ASCIIHex = 0x05;
			break;
	case 54:
			ASCIIHex = 0x06;
			break;
	case 55:
			ASCIIHex = 0x07;
			break;
	case 56:
			ASCIIHex = 0x08;
			break;
	case 57:
			ASCIIHex = 0x09;
			break;
	case 65:
			ASCIIHex = 0x0A;
			break;
	case 66:
			ASCIIHex = 0x0B;
			break;
	case 67:
			ASCIIHex = 0x0C;
			break;
	case 68:
			ASCIIHex = 0x0D;
			break;
	case 69:
			ASCIIHex = 0x0E;
			break;
	case 70:
			ASCIIHex = 0x0F;
			break;
	}
	return (ASCIIHex);
}




