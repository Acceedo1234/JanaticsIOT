/*
 * LpdcLogic.cpp
 *
 *  Created on: 04-Feb-2023
 *      Author: MKS
 */
extern "C"{
#include "Struct_Input_output.h"
}
#include "LpdcLogic.h"
#include "main.h"
#define INDICATIONOFFSET 20

GPIO_PinState GPIO_PinStateMac;
GPIO_PinState GPIO_PinStateEjection;
GPIO_PinState GPIO_PinStateRej;

GPIO_PinState Sim_Trigger;

uint8_t MAC_A_Prod_Input1_StartTimer;
uint16_t MAC_A_Prod_Input1_CountBase;
uint16_t MAC_Gen_Prod_Input1_Production,Production_Zeit;
uint16_t MAC_Gen_Rej_Input_Production,Rejection_Zeit;
uint16_t Manual_RejectionCount;
uint8_t CurrentShift_K1;
uint8_t IsCurrentShiftUpdated;
uint8_t UpdateStorage;
uint8_t processControl;
uint8_t updateMemProcCtrl;
uint16_t productionInc;
extern uint8_t CurrentShift;
extern uint8_t UpdateShiftInfo;
extern uint16_t productiontimeSetOL;
extern uint8_t productionhysPosSetOL,productionhysNegSetOL,productChangeOL;
extern uint8_t alarmOnOff;
extern uint8_t resetStatus,startStopStatus;
extern uint16_t batchTargetquantity,batchNumber,requirementId;
uint16_t requirementIdK1;
uint16_t productionTarget;
uint8_t triggerStartForReq;


LpdcLogic::LpdcLogic() {
	// TODO Auto-generated constructor stub

}

LpdcLogic::~LpdcLogic() {
	// TODO Auto-generated destructor stub
}

void LpdcLogic::run()
{
	shiftChange();
	machineControl();
	productChange();
	mAlarmControl();
}

void LpdcLogic::shiftChange()
{
	if((CurrentShift != CurrentShift_K1)&&(UpdateShiftInfo==1))
	{
		MAC_Gen_Prod_Input1_Production=0;
		MAC_Gen_Rej_Input_Production=0;
		Manual_RejectionCount=0;
		Production_Zeit=0;
		Rejection_Zeit=0;
		UpdateShiftInfo=0;
		CurrentShift_K1= CurrentShift;
		IsCurrentShiftUpdated=1;
	}
}

void LpdcLogic::productChange()
{
	if((productChangeOL ==1)&&(Debounce_productChangeOL ==1))	  /*reset due to product change*/
	  {
		MAC_Gen_Prod_Input1_Production=0;
		MAC_Gen_Rej_Input_Production=0;
		Manual_RejectionCount=0;
		Production_Zeit=0;
		Rejection_Zeit=0;
		Debounce_productChangeOL=0;
	  }
	  else if(productChangeOL==0)
	  {
	  	Debounce_productChangeOL=1;
	  }
}

void LpdcLogic::production()
{
GPIO_PinStateMac = HAL_GPIO_ReadPin(GPIOC,InputMachine1_Pin);
}

//}

void LpdcLogic::manualRejection()
{
	GPIO_PinStateRej = GPIO_PIN_SET;//HAL_GPIO_ReadPin(GPIOA,InputMachine3_Pin);
	if((GPIO_PinStateRej ==GPIO_PIN_RESET)&&(DebounceMACA_Rej))
	 {
		  DebounceMACA_Rej=0;
		  //Manual_RejectionCount	=  Manual_RejectionCount+1;
		  MAC_Gen_Prod_Input1_Production =0;
		  MAC_Gen_Rej_Input_Production = 1;
		  productionInc = productionInc+1;
		  UpdateStorage=1;
	 }
	 else if(GPIO_PinStateRej ==GPIO_PIN_SET)
	 {
		 DebounceMACA_Rej=1;
	 }
}

void LpdcLogic::mAlarmControl(void)
{
	if(alarmOnOff == 1)
	{
	//	HAL_GPIO_WritePin(GPIOC, RELAY4_Pin, GPIO_PIN_SET);
	}
	else
	{
	//	HAL_GPIO_WritePin(GPIOC, RELAY4_Pin, GPIO_PIN_RESET);
	}
}

void LpdcLogic::machineControl(void)
{
	if(requirementId != requirementIdK1){
		productionInc 		= 0;
		requirementIdK1 	= requirementId;
		updateMemProcCtrl	= 1;
		triggerStartForReq  = 1;
		productionTarget 	= batchTargetquantity;
	}

	if(triggerStartForReq==1){
		if(startStopStatus ==1){
			triggerStartForReq=2;
		}
	}
	else if(triggerStartForReq==2){
		if((productionInc <= productionTarget)&&(startStopStatus!=2))/*FInal Stop*/
		{
			HAL_GPIO_WritePin(GPIOC, RELAY4_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOC, RELAY4_Pin, GPIO_PIN_RESET);
		}
		if((productionInc <= (productionTarget-INDICATIONOFFSET))&&(startStopStatus!=2))/*Pre indication*/
		{
			HAL_GPIO_WritePin(GPIOC, RELAY3_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOC, RELAY3_Pin, GPIO_PIN_RESET);
		}
	}

}


