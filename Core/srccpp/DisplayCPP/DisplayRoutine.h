/*
 * DisplayRoutine.h
 *
 *  Created on: 01-Feb-2023
 *      Author: MKS
 */

#ifndef SRCCPP_DISPLAYCPP_DISPLAYROUTINE_H_
#define SRCCPP_DISPLAYCPP_DISPLAYROUTINE_H_
#include "GLCD.h"


class DisplayRoutine {
public:
	DisplayRoutine();
	virtual ~DisplayRoutine();
	void run();
	void Init();

private:
	GLCD m_gLCDDrive;
  //unsigned char Lpdc[15]="      LPDC   ";
	char Lpdc[15]="     SKYFAST ";
	char colon[2]=":";
	char space[4]="  ";
	char backslash[2]="/";
	char production[13]="Production:";
	char rejection[13] ="Rejection :";
	char Mrejection[13]="Cyclictime:";
	void value2Digit(uint8_t value,uint8_t posLine,uint8_t posCol);
	void value4Digit(uint16_t value4dig,uint8_t posLine,uint8_t posCol);


};

#endif /* SRCCPP_DISPLAYCPP_DISPLAYROUTINE_H_ */
