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
GPIO_PinState GPIO_PinStateMac;
GPIO_PinState GPIO_PinStateEjection;
GPIO_PinState GPIO_PinStateRej;

uint8_t MAC_A_Prod_Input1_StartTimer;
uint16_t MAC_A_Prod_Input1_CountBase;
uint16_t MAC_Gen_Prod_Input1_Production,Production_Zeit;
uint16_t MAC_Gen_Rej_Input_Production,Rejection_Zeit;
uint16_t Manual_RejectionCount;
uint8_t CurrentShift_K1;
uint8_t IsCurrentShiftUpdated;
uint8_t UpdateStorage;
uint8_t processControl;
uint16_t productionInc;
extern uint8_t CurrentShift;
extern uint8_t UpdateShiftInfo;
extern uint16_t productiontimeSetOL;
extern uint8_t productionhysPosSetOL,productionhysNegSetOL,productChangeOL;
extern uint8_t alarmOnOff;
extern uint8_t resetStatus,startStopStatus;
extern uint16_t batchTargetquantity,batchNumber,requirementId;


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
	production();
	manualRejection();
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
GPIO_PinStateEjection = HAL_GPIO_ReadPin(GPIOC,InputMachine2_Pin);
if(GPIO_PinStateMac == GPIO_PIN_RESET){
	if((GPIO_PinStateEjection ==GPIO_PIN_RESET)&&(MAC_A_Prod_Input1_DeBounce))
	{
		  MAC_A_Prod_Input1_DeBounce	= 0;
		  MAC_Gen_Prod_Input1_Production =1;
		  productionInc = productionInc+1;
		  MAC_Gen_Rej_Input_Production = 0;
		  UpdateStorage=1;

	}
	else if((GPIO_PinStateEjection==GPIO_PIN_SET)&&(!MAC_A_Prod_Input1_DeBounce))
	{
		   MAC_A_Prod_Input1_DeBounce	 =1;

	}
	else{}
}

}

void LpdcLogic::manualRejection()
{
	GPIO_PinStateRej = HAL_GPIO_ReadPin(GPIOA,InputMachine3_Pin);
	if((GPIO_PinStateRej ==GPIO_PIN_RESET)&&(DebounceMACA_Rej))
	 {
		  DebounceMACA_Rej=0;
		  //Manual_RejectionCount	=  Manual_RejectionCount+1;
		  MAC_Gen_Prod_Input1_Production =0;
		  MAC_Gen_Rej_Input_Production = 1;
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
#if 0
	switch(processControl)
	{
		case 0:
			processControl=1;
		break;
		case 1:
			productionInc = 0;
			HAL_GPIO_WritePin(GPIOC, RELAY4_Pin, GPIO_PIN_SET);
			processControl=2;
		break;
		case 2:

		break;
		default:
		break;
	}
#endif

}


