/*
 * LpdcLogic.h
 *
 *  Created on: 04-Feb-2023
 *      Author: MKS
 */

#ifndef SRCCPP_BUSSINESSLOGIC_LPDCLOGIC_H_
#define SRCCPP_BUSSINESSLOGIC_LPDCLOGIC_H_


#include "main.h"

class LpdcLogic {
public:
	LpdcLogic();
	virtual ~LpdcLogic();
	void run();
	void manualRejection();
	void production();
	void shiftChange();
	void productChange();


private:
	uint8_t MAC_A_Prod_Input1_DeBounce,DebounceMACA_Rej;
	uint8_t MAC_A_Prod_Input1_RisingEdge;
	uint8_t MAC_A_Prod_Input1_FallingEdge;
	uint8_t Debounce_productChangeOL=1;
	void mAlarmControl(void);
};

#endif /* SRCCPP_BUSSINESSLOGIC_LPDCLOGIC_H_ */
