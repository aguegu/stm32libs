/*
 * i2c-soft.cpp
 *
 *  Created on: Jun 9, 2013
 *      Author: agu
 */

#include "i2c-soft.h"

I2cSoft::I2cSoft(Gpio &sda, Gpio &scl) :
		_sda(sda), _scl(scl) {

}

I2cSoft::~I2cSoft() {

}

void I2cSoft::init() {
	_sda.init(GPIO_Mode_Out_OD);
	_scl.init(GPIO_Mode_Out_OD);
}

void I2cSoft::start() {

	_sda.set(Bit_SET);
	delayus(0x8000);
	_scl.set(Bit_SET);
	delayus(0x8000);
	_sda.set(Bit_RESET);
	delayus(0x8000);
	_scl.set(Bit_RESET);
}

void I2cSoft::stop() {
	_sda.set(Bit_RESET);
	_scl.set(Bit_SET);
	delayus(0x8000);
	_sda.set(Bit_SET);
	delayus(0x8000);
}

void I2cSoft::send(uint8_t data) {

	_sda.init(GPIO_Mode_Out_OD);

	for (uint8_t i = 0; i < 8; i++) {
		_sda.set(data & 0x80 ? Bit_SET : Bit_RESET);
		delayus(0x8000);
		_scl.set(Bit_SET);
		delayus(0x8000);
		_scl.set(Bit_RESET);
		delayus(0x8000);
		data <<= 1;
	}
}

vu32 I2cSoft::waitAck() {

	_sda.init(GPIO_Mode_IN_FLOATING);

	vu32 t = 0;

	_scl.set(Bit_SET);

	while (_sda.getInput() == Bit_SET)
		t++;

	_scl.set(Bit_RESET);
	_sda.init(GPIO_Mode_Out_OD);

	return t;
}

void I2cSoft::delayus(vu16 t) {
	while (t--)
		;
}
//uint8_t I2cSoft::readFrom(uint8_t address, uint8_t *data, uint8_t length,
//		uint8_t send_stop) {
//
//	this->start();
//	this->send((address << 1) | 0x01);
//	this->waitAck();
//
//	this->stop();
//}
//
