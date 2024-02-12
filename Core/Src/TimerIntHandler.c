/*
 * UARTTxRxHandle.c
 *
 *  Created on: Jan 21, 2023
 *      Author: MKS
 */
 #include"main.h"
#define SIMON 1
#define SIMOFF 0
#define SimulationMode SIMOFF
 
 extern TIM_HandleTypeDef htim7;
 extern TIM_HandleTypeDef htim6;
 uint8_t Flag1MS;
 uint8_t Flag1Second;
 uint8_t Flag100milliSeconds;
 uint8_t SimCount;
 
 uint8_t LocCount1S;
 uint8_t wifiConnection,commFeedbackcount=0;
 extern uint8_t commFeedbackFlag;
 extern uint8_t MAC_A_Prod_Input1_StartTimer;
 extern uint16_t MAC_A_Prod_Input1_CountBase;
 extern GPIO_PinState Sim_Trigger;
 
 void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
 {
 	if(htim == &htim7)
 	{
 		Flag1MS=1;
 	}
 	if(htim == &htim6)
	{
 		Flag100milliSeconds=1;
#if SimulationMode== SIMON
 		SimCount= SimCount+1;
 		if(SimCount == 2)
		{
			//HAL_GPIO_TogglePin(GPIOC, RELAY3_Pin);
			//Sim_Trigger = GPIO_PIN_RESET;
 			HAL_GPIO_WritePin(GPIOC,RELAY3_Pin,GPIO_PIN_SET);
		}
 		else if(SimCount == 3){
 			HAL_GPIO_WritePin(GPIOC,RELAY3_Pin,GPIO_PIN_RESET);
 		}
 		else if(SimCount==7){
 			HAL_GPIO_WritePin(GPIOC,RELAY4_Pin,GPIO_PIN_SET);
 		}
 		else if(SimCount==8){
 		 	HAL_GPIO_WritePin(GPIOC,RELAY4_Pin,GPIO_PIN_RESET);
 		}
 		else if(SimCount==20)
 		{
 			SimCount=0;
 		}
#endif

 		if(++LocCount1S >= 10)
		{
			LocCount1S=0;

			Flag1Second =1;
			if(commFeedbackFlag==1){
				commFeedbackFlag=0;
				wifiConnection = 1;
				commFeedbackcount=0;
			}
			else
			{
				if(++commFeedbackcount > 50){
					wifiConnection=0;
					commFeedbackcount=81;
				}
			}
		}
		if(MAC_A_Prod_Input1_StartTimer)
		{
		   ++MAC_A_Prod_Input1_CountBase;
		}
	}
 }
