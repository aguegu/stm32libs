/*
 * spi-soft.h
 *
 *  Created on: Jun 7, 2013
 *      Author: agu
 */

#ifndef SPI_SOFT_H_
#define SPI_SOFT_H_

#include "gpio/gpio.h"

// function as a spi master

class SpiSoft {
public:
	SpiSoft(Gpio & mosi, Gpio & miso, Gpio & sck, Gpio & ss);
	virtual ~SpiSoft();
	void transmit(const uint8_t * out, uint8_t * in, uint32_t length);
	void transmit(const uint8_t *out, uint32_t out_length, uint8_t * in,
			uint32_t in_length, uint8_t dummy_data = 0x00);
	void transmit(const uint8_t out, uint8_t * in, uint32_t in_length);
	void init();
protected:
	uint8_t transmitByte(uint8_t in);
private:
	Gpio & _mosi;
	Gpio & _miso;
	Gpio & _sck;
	Gpio & _ss;
};

#endif /* SPI_SOFT_H_ */
