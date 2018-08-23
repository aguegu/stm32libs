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

u8 At24c::write_raw(u16 reg_address, const void * buff,
		u16 length) {
	assert_param(length <= _pagesize);
	u8 t = _i2c.setReg(_slave_address | (reg_address >> 8), reg_address, (u8 *)buff,
			length);
	delay(6);
	return t;
}

u8 At24c::read(u16 reg_address, void * buff, u16 length) {
	return _i2c.getReg(_slave_address | (reg_address >> 8), reg_address, (u8 *)buff,
			length);
}

u8 At24c::write(u16 reg_address, const void * buff,
		u16 length) {
	uint8_t t = 0;
	for (u16 i = reg_address; i < reg_address + length;) {
		u16 end = (i + 8) / 8 * 8;
		if (end > reg_address + length) {
			end = reg_address + length;
		}
		t = this->write_raw(i, (u8*)buff + i - reg_address, end - i);
		i = end;
	}
	return t;
}


//uint8_t At24c::write(uint16_t reg_address, const void * buff,
//		uint16_t length) {
//	assert_param(length <= _pagesize);
//	return _i2c.setReg(_slave_address | (reg_address >> 8), reg_address, (uint8_t *)buff,
//			length);
//}
//
//uint8_t At24c::write(uint16_t reg_address, uint8_t data) {
//	return this->write(reg_address, &data, 1);
//}
//
//uint8_t At24c::read(uint16_t reg_address, void * buff, uint16_t length) {
//	assert_param(length <= _pagesize);
//	return _i2c.getReg(_slave_address | (reg_address >> 8), reg_address, (uint8_t *)buff,
//			length);
//}
