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
	~SpiSoft();
	void transmit(uint8_t * out, uint8_t * in, uint8_t length);
	void init();
private:
	Gpio & _mosi;
	Gpio & _miso;
	Gpio & _sck;
	Gpio & _ss;

	uint8_t transmitByte(uint8_t in);
};

#endif /* SPI_SOFT_H_ */
