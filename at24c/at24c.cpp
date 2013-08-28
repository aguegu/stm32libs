/*
 * at24c.cpp
 *
 *  Created on: Aug 28, 2013
 *      Author: agu
 */

#include "at24c/at24c.h"

At24c::At24c(I2c & i2c, uint8_t slave_address) :
		_i2c(i2c), _slave_address(slave_address) {

}

At24c::~At24c() {

}

uint8_t At24c::write(uint16_t reg_address, const void * buff,
		uint16_t length) {
	assert_param(length <= 16);

	uint8_t result = _i2c.setReg(_slave_address | (reg_address >> 8), reg_address, (uint8_t *)buff,
			length);
	delay(5);

	return result;
}

uint8_t At24c::write(uint16_t reg_address, uint8_t data) {
	return this->write(reg_address, &data, 1);
}

uint8_t At24c::read(uint16_t reg_address, void * buff, uint16_t length) {
	return _i2c.getReg(_slave_address | (reg_address >> 8), reg_address, (uint8_t *)buff,
			length);
	delay(5);
}
