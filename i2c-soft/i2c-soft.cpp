/*
 * i2c-soft.cpp
 *
 *  Created on: Jun 9, 2013
 *      Author: agu
 */

#include "i2c-soft.h"

#define within(time, fun) for (t = (time); (fun) && --t;)

I2cSoft::I2cSoft(Gpio &sda, Gpio &scl, uint16_t flat_timeout) :
		_sda(sda), _scl(scl), _FLAG_TIMEOUT(flat_timeout) {

}

I2cSoft::~I2cSoft() {

}

void I2cSoft::init() {
	_scl.init(GPIO_Mode_Out_OD);
	_scl.set(Bit_SET);
}

void I2cSoft::start() {
	_sda.set(Bit_SET);
	_sda.init(GPIO_Mode_Out_OD);

	_sda.set(Bit_RESET);
	_scl.set(Bit_RESET);
}

void I2cSoft::stop() {
	_sda.set(Bit_RESET);
	_sda.init(GPIO_Mode_Out_OD);

	_scl.set(Bit_SET);
	_sda.set(Bit_SET);
}

void I2cSoft::transmit(uint8_t data) {

	_sda.init(GPIO_Mode_Out_OD);

	for (uint8_t i = 0; i < 8; i++) {
		_sda.set(data & 0x80 ? Bit_SET : Bit_RESET);
		_scl.set(Bit_SET);
		_scl.set(Bit_RESET);
		data <<= 1;
	}
}

uint8_t I2cSoft::receive() {
	_sda.init(GPIO_Mode_IN_FLOATING);
	uint8_t data = 0;
	for (u8 i = 0; i < 8; i++) {
		_scl.set(Bit_SET);
		data <<= 1;
		data |= _sda.getInput() ? 0x01 : 0x00;
		_scl.set(Bit_RESET);
	}
	return data;
}

u16 I2cSoft::waitAck() {
	_sda.init(GPIO_Mode_IN_FLOATING);
	_scl.set(Bit_SET);

	vu16 t;
	within(_FLAG_TIMEOUT, _sda.getInput() == Bit_SET);

	_scl.set(Bit_RESET);
	return t;
}

uint8_t I2cSoft::write(uint8_t address, uint8_t *data, uint8_t length,
		uint8_t send_stop) {
	vu16 t;
	within(_FLAG_TIMEOUT, _sda.getInput() == Bit_RESET);
	if (!t) return 1;

	this->start();
	this->transmit(address << 1);

	if (this->waitAck() == 0) return 2;

	while (length--) {
		this->transmit(*data++);
		if (this->waitAck() == 0) return 3;
	}

	if (send_stop) this->stop();

	return t;
}

void I2cSoft::read(uint8_t address, uint8_t *data, uint8_t length,
		uint8_t send_stop) {
	this->start();
	this->transmit((address << 1) | 0x01);
	this->waitAck();

	while (length--) {
		*data++ = this->receive();
		this->sendAck(length);
	}

	if (send_stop) this->stop();
}

void I2cSoft::sendAck(uint8_t ack) {
	_scl.set(Bit_RESET);

	_sda.set(ack ? Bit_RESET : Bit_SET);
	_sda.init(GPIO_Mode_Out_OD);

	_scl.set(Bit_SET);
	_scl.set(Bit_RESET);
}
