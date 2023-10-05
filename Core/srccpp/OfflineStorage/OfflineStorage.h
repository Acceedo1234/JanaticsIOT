/*
 * OfflineStorage.h
 *
 *  Created on: 09-Feb-2023
 *      Author: MKS
 */

#ifndef SRCCPP_OFFLINESTORAGE_OFFLINESTORAGE_H_
#define SRCCPP_OFFLINESTORAGE_OFFLINESTORAGE_H_

class OfflineStorage {
public:
	OfflineStorage();
	virtual ~OfflineStorage();
	void run();
	void ReadOfflinedataInit();
	void ECUProductionInit(void);
private:
	uint8_t FlashMemProductiondata[22];
	uint16_t NoofData;
	uint8_t tempBlockcalc;
	uint8_t updateInBlockFill;

};

#endif /* SRCCPP_OFFLINESTORAGE_OFFLINESTORAGE_H_ */
