/*
 * ExtInterrupt.c
 *
 *  Created on: Feb 9, 2024
 *      Author: MKS
 */
#include "main.h"
#define PRESSMAC 1
#define INJECTIONMAC 2

/*Application tuning*/
#define MACTYPE PRESSMAC
/********************/

uint8_t ProductionInput1,ProductionInput2;
uint8_t ProductionInputFallingEdge1,ProductionInputFallingEdge2;
uint8_t MachineState_Auto;
uint16_t GPIO_Pin_Value1,GPIO_Pin_Value2;
uint16_t ProductionTest;

extern uint16_t MAC_Gen_Prod_Input1_Production,productionInc;
extern uint8_t UpdateStorage;


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  MachineState_Auto = HAL_GPIO_ReadPin(GPIOC,InputMachine1_Pin);
  if(MachineState_Auto==0){
#if MACTYPE == PRESSMAC
		if(GPIO_Pin == ProductionInput1_Pin)
		{
		 // ++ProductionInput1;
		  ProductionInputFallingEdge1 = 1;
		  ProductionInputFallingEdge2 = 0;
		  GPIO_Pin_Value1 = GPIO_Pin;

		}
		else if(GPIO_Pin == ProductionInput2_Pin)
		{
		 // ++ProductionInput2;
		  if(ProductionInputFallingEdge1==1){
			  ProductionInputFallingEdge1= 0;
			  ProductionInputFallingEdge2= 1;
			  GPIO_Pin_Value2 = GPIO_Pin;
			//  ProductionTest= ProductionTest+1;
			  MAC_Gen_Prod_Input1_Production =1;
			  productionInc = productionInc+1;
			  UpdateStorage=1;
		  }
		}
#endif
#if MACTYPE == INJECTIONMAC
		if(GPIO_Pin == ProductionInput1_Pin)
		{
		  ++ProductionInput1;
		  ProductionInputFallingEdge1 = 1;
		  ProductionInputFallingEdge2 = 0;
		  GPIO_Pin_Value1 = GPIO_Pin;
		  MAC_Gen_Prod_Input1_Production =1;
		  productionInc = productionInc+1;
		  UpdateStorage=1;
		}
#endif
  }
}




