/*
 * hd44780-gpio.cpp
 *
 *  Created on: Aug 11, 2013
 *      Author: agu
 */

#include "hd44780/hd44780-gpio.h"

Hd44780Gpio::Hd44780Gpio(Gpio & pin_rs, Gpio & pin_en, Gpio & pin_d4,
	Gpio & pin_d5, Gpio & pin_d6, Gpio & pin_d7, uint8_t row_count,
	uint8_t col_count) :
		Hd44780Basic(row_count, col_count), _pin_rs(pin_rs), _pin_en(pin_en), _pin_d4(
			pin_d4), _pin_d5(pin_d5), _pin_d6(pin_d6), _pin_d7(pin_d7) {

}

Hd44780Gpio::~Hd44780Gpio() {

}

void Hd44780Gpio::setDataPins(uint8_t c, bool high) const {
	if (high)
		c >>= 4;

	_pin_d4.set(c & 0x01);
	_pin_d5.set(c & 0x02);
	_pin_d6.set(c & 0x04);
	_pin_d7.set(c & 0x08);

	_pin_en.set(Bit_SET);
	_pin_en.set(Bit_RESET);

#ifdef  USE_FULL_ASSERT
	delayMicroseconds(0x20);
#else
	delayMicroseconds(0x24);
#endif
}

void Hd44780Gpio::setDataPins(uint8_t c) const {
	this->setDataPins(c, true);
	this->setDataPins(c, false);
}

void Hd44780Gpio::transmit(bool isData, uint8_t val) const {
	_pin_rs.set(isData);
	this->setDataPins(val);
}

void Hd44780Gpio::initHardware() const {

	_pin_rs.init(GPIO_Mode_Out_PP);
	_pin_en.init(GPIO_Mode_Out_PP);
	_pin_d4.init(GPIO_Mode_Out_PP);
	_pin_d5.init(GPIO_Mode_Out_PP);
	_pin_d6.init(GPIO_Mode_Out_PP);
	_pin_d7.init(GPIO_Mode_Out_PP);

	_pin_rs.set(Bit_RESET);
	_pin_en.set(Bit_RESET);

	delayMicroseconds(40000);

	this->setDataPins(0x30, true);
	delayMicroseconds(4500);

	this->setDataPins(0x30, true);
	delayMicroseconds(150);

	this->setDataPins(0x30, true);
	this->setDataPins(0x20, true);

	this->configureFunction();
	this->configureDisplay(false, false, false);

	this->clear();
	this->configureInput();
	this->rst();
}

