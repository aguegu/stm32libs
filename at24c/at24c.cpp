/*
 * at24c.cpp
 *
 *  Created on: Aug 28, 2013
 *      Author: agu
 */

#include "at24c/at24c.h"

At24c::At24c(I2c & i2c, uint8_t pagesize, uint8_t slave_address) :
		_i2c(i2c), _pagesize(pagesize), _slave_address(slave_address) {

}

At24c::~At24c() {

}

uint8_t At24c::write(uint16_t reg_address, const uint8_t * buff,
		uint16_t length) {
	if (_pagesize == 16) {
		uint8_t *p = (uint8_t *) malloc(sizeof(uint8_t) * (length + 2));
		p[0] = reg_address >> 8;
		p[1] = reg_address & 0xff;
		memcpy(p + 2, buff, length);
		uint8_t result = _i2c.write(_slave_address, p, length + 2);
		free(p);
		return result;
	} else
		return _i2c.setReg(_slave_address, reg_address, buff, length);
}

uint8_t At24c::write(uint16_t reg_address, uint8_t data) {
	return this->write(reg_address, &data, 1);
}

uint8_t At24c::read(uint16_t reg_address, uint8_t * buff, uint16_t length) {
	uint8_t p[2];
	p[0] = reg_address >> 8;
	p[1] = reg_address & 0xff;
	_i2c.write(0x50, p, 2, DISABLE);
	return _i2c.read(0x50, buff, length, DISABLE);
}
