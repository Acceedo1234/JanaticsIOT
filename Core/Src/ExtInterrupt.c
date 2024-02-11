/*
 * ExtInterrupt.c
 *
 *  Created on: Feb 9, 2024
 *      Author: MKS
 */
#include "main.h"
uint8_t ProductionInput1,ProductionInput2;
uint8_t ProductionInputFallingEdge1,ProductionInputFallingEdge2;
uint16_t GPIO_Pin_Value1,GPIO_Pin_Value2;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

  if(GPIO_Pin == ProductionInput1_Pin)
  {
	  ++ProductionInput1;
	  ProductionInputFallingEdge1 = 1;
	  ProductionInputFallingEdge2 = 0;
	  GPIO_Pin_Value1 = GPIO_Pin;

  }
  else if(GPIO_Pin == ProductionInput2_Pin)
  {
	  ++ProductionInput2;
	  if(ProductionInputFallingEdge1==1){
		  ProductionInputFallingEdge1= 0;
		  ProductionInputFallingEdge2= 1;
		  GPIO_Pin_Value2 = GPIO_Pin;
	  }
  }
}




