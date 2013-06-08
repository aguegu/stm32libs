/*
 * sst25-spisoft.h
 *
 *  Created on: Jun 8, 2013
 *      Author: agu
 */

#ifndef SST25_SPISOFT_H_
#define SST25_SPISOFT_H_

#include "spi-soft/spi-soft.h"

class Sst25SpiSoft: public SpiSoft {
public:
	Sst25SpiSoft(Gpio & mosi, Gpio & miso, Gpio & sck, Gpio & ss);
	virtual ~Sst25SpiSoft();
	uint16_t getId();
	uint8_t getStatus();
	void write(uint32_t address, uint8_t data);
	uint8_t read(uint32_t address);
	void setStatus(uint8_t status);
};

#endif /* SST25_SPISOFT_H_ */
