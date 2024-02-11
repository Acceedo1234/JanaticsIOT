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
	void dwinDecoder(void);
private:
	uint8_t u8ModbusRegisterdwin[60];
	uint8_t Cntid_dwin{0};
	uint8_t noOfDataDwin;
};

#endif /* SRCCPP_DWIN_DWINHMI_H_ */
