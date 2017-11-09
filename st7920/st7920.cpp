/*
 * st7920.cpp
 *
 *  Created on: Aug 15, 2013
 *      Author: agu
 */

#include "st7920.h"

St7920::St7920(Gpio & pin_rs, Gpio & pin_en, Gpio & pin_d4, Gpio & pin_d5,
	Gpio & pin_d6, Gpio & pin_d7) :
		_pin_rs(pin_rs), _pin_en(pin_en), _pin_d4(pin_d4), _pin_d5(pin_d5), _pin_d6(
			pin_d6), _pin_d7(pin_d7) {
}

St7920::~St7920() {

}

void St7920::init() {

	_pin_rs.init(GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
	_pin_en.init(GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
	_pin_d4.init(GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
	_pin_d5.init(GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
	_pin_d6.init(GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
	_pin_d7.init(GPIO_Mode_Out_PP, GPIO_Speed_50MHz);

	_pin_en.set(Bit_RESET);

	delayMicroseconds(50000);

	this->setDataPins(0x20, true);
	this->configureFunction(false, false);
	this->configureDisplay(true, false, false);
	this->clear();
	this->configureEntry(true);

	this->configureFunction(false, true, true);
}

void St7920::clear() {
	this->transmit(false, 0x01);
	delayMicroseconds(2000);
}

void St7920::home() {
	this->transmit(false, 0x02);
}

void St7920::configureEntry(bool right) {
	uint8_t cmd = 0x04;
	if (right)
		cmd |= 0x02;
	this->transmit(false, cmd);
}

void St7920::configureDisplay(bool display, bool cursor, bool blink) {
	uint8_t cmd = 0x08;
	if (display)
		cmd |= 0x04;
	if (cursor)
		cmd |= 0x02;
	if (blink)
		cmd |= 0x01;
	this->transmit(false, cmd);
}

void St7920::moveCursor(bool right) {
	uint8_t cmd = 0x10;
	if (right)
		cmd |= 0x04;
	this->transmit(false, cmd);
}

void St7920::moveDisplay(bool right) {
	uint8_t cmd = 0x18;
	if (right)
		cmd |= 0x04;
	this->transmit(false, cmd);
}

void St7920::configureFunction(bool interface8, bool re, bool graphic) {
	uint8_t cmd = 0x20;
	if (interface8)
		cmd |= 0x10;
	if (re)
		cmd |= 0x04;
	if (re && graphic)
		cmd |= 0x02;
	this->transmit(false, cmd);
}

void St7920::setCgRam(uint8_t value) {
	this->transmit(true, 0x40 | value);
}

void St7920::setDdRam(uint8_t address) {
	this->transmit(false, 0x80 | address);
}

/////////////////////////////////////

void St7920::transmit(bool isData, uint8_t c) {
	_pin_rs.set(isData);
	setDataPins(c, true);
	setDataPins(c, false);
}

void St7920::transmit(bool isData, uint8_t *p, uint8_t length) {
	_pin_rs.set(isData);
	while (length--) {
		setDataPins(*p, true);
		setDataPins(*p++, false);
	}
}

void St7920::setDataPins(uint8_t c, bool high) {
	if (high)
		c >>= 4;

	_pin_d4.set(c & 0x01);
	_pin_d5.set(c & 0x02);
	_pin_d6.set(c & 0x04);
	_pin_d7.set(c & 0x08);

	_pin_en.set(Bit_SET);
	_pin_en.set(Bit_RESET);

#ifndef USE_FULL_ASSERT
	delayMicroseconds(0x0c);
#endif
}

void St7920::printWord(char *p) {
	while (*p) {
		this->transmit(true, *p++);
	}
}
