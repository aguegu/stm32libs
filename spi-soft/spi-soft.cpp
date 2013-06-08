/*
 * spi-soft.cpp
 *
 *  Created on: Jun 7, 2013
 *      Author: agu
 */

#include "spi-soft.h"
#include "stm32-template.h"

SpiSoft::SpiSoft(Gpio & mosi, Gpio & miso, Gpio & sck, Gpio & ss) :
		_mosi(mosi), _miso(miso), _sck(sck), _ss(ss) {

}

SpiSoft::~SpiSoft() {

}

void SpiSoft::init() {
	_mosi.init(GPIO_Mode_Out_PP);
	_miso.init(GPIO_Mode_IPD);
	_sck.init(GPIO_Mode_Out_PP);
	_ss.init(GPIO_Mode_Out_PP);

	_ss.set(Bit_SET);
	_sck.set(Bit_RESET);
}

void SpiSoft::transmit(uint8_t * out, uint8_t * in, uint8_t length) {
	_ss.set(Bit_RESET);

	while (length--) {
		*in++ = transmitByte(*out++);
	}

	_ss.set(Bit_SET);
}

uint8_t SpiSoft::transmitByte(uint8_t out) {

	uint8_t c = 0x00;
	for (uint8_t i = 0; i < 8; i++) {

		c <<= 1;
		c |= _miso.getInput() ? 0x01 : 0x00;
		_mosi.set(out & 0x80 ? Bit_SET : Bit_RESET);
		_sck.set(Bit_SET);
		_sck.set(Bit_RESET);
		out <<= 1;
	}

	return c;
}
