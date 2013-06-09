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
	enum EraseType {
		Chip = 0x60, Sector4K = 0x20, Block32K = 0x52, Block64K = 0xd8,
	};

	Sst25SpiSoft(Gpio & mosi, Gpio & miso, Gpio & sck, Gpio & ss);
	virtual ~Sst25SpiSoft();
	uint16_t getId();

	uint8_t getStatus();
	void setStatus(uint8_t status);

	void write(uint32_t address, uint8_t data);
	uint8_t read(uint32_t address);

	void write(uint32_t address, uint8_t *data, uint32_t length);
	void read(uint32_t address, uint8_t *data, uint32_t length);

	void erase(EraseType type, uint32_t address = 0);
	uint8_t isBusy();
};

#endif /* SST25_SPISOFT_H_ */
