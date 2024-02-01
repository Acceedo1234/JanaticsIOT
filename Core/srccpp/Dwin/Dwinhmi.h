/*
 * Dwinhmi.h
 *
 *  Created on: Feb 1, 2024
 *      Author: MKS
 */

#ifndef SRCCPP_DWIN_DWINHMI_H_
#define SRCCPP_DWIN_DWINHMI_H_
#include "main.h"

class Dwinhmi {
public:
	Dwinhmi();
	virtual ~Dwinhmi();
	void dwinFrame(void);
private:
	uint8_t u8ModbusRegisterdwin[120];
	uint8_t Cntid_dwin;
	uint8_t noOfDataDwin;
};

#endif /* SRCCPP_DWIN_DWINHMI_H_ */
