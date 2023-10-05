/*
 * UARTTxRxHandle.c
 *
 *  Created on: Jan 21, 2023
 *      Author: MKS
 */
 #include"main.h"
 
 extern TIM_HandleTypeDef htim7;
 extern TIM_HandleTypeDef htim6;
 uint8_t Flag1MS;
 uint8_t Flag1Second;
 uint8_t Flag100milliSeconds;
 uint8_t SimCount;
 
 uint8_t LocCount1S;
 extern uint8_t MAC_A_Prod_Input1_StartTimer;
 extern uint16_t MAC_A_Prod_Input1_CountBase;
 
 void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
 {
 	if(htim == &htim7)
 	{
 		Flag1MS=1;
 	}
 	if(htim == &htim6)
	{
 		Flag100milliSeconds=1;
 		if(++LocCount1S >= 10)
		{
			LocCount1S=0;
			if(++SimCount >= 20)
			{
				SimCount=0;
				HAL_GPIO_TogglePin(GPIOC, RELAY4_Pin);
			}
			Flag1Second =1;
		}
		if(MAC_A_Prod_Input1_StartTimer)
		{
		   ++MAC_A_Prod_Input1_CountBase;
		}

	}
 
 }
