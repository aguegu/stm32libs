/*
 * ds3231.cpp
 *
 *  Created on: Aug 15, 2013
 *      Author: agu
 */

#include "ds3231/ds3231.h"

Ds3231::Ds3231(I2c & i2c) :
		_i2c(i2c) {
	_datetime = (uint8_t *) malloc(sizeof(uint8_t) * 7);
	memset(_datetime, 0, 7);
}

Ds3231::~Ds3231() {
	free(_datetime);
}

void Ds3231::init() {
	_i2c.setReg(_address, 0x0e, 0x9c);
	_i2c.setReg(_address, 0x0f, 0x00);
}

uint8_t Ds3231::getHexData(uint8_t index) {
	return _datetime[index] & 0x7f;
}

uint8_t Ds3231::getDecData(uint8_t index) {
	return Ds3231::hex2dec(_datetime[index] & 0x7f);
}

uint8_t Ds3231::hex2dec(uint8_t hex) {
	return ((hex >> 4) * 10 + (hex & 0x0f));
}

uint8_t Ds3231::dec2hex(uint8_t dec) {
	return (((dec / 10) << 4) + dec % 10);
}

void Ds3231::refresh(bool only_time) {
	uint8_t j = only_time ? 3 : 7;
	_i2c.getReg(_address, SECOND, _datetime, j);
}

void Ds3231::setTime(uint8_t hour, uint8_t minute, uint8_t second) {
	uint8_t tmp[3];
	tmp[0] = dec2hex(second);
	tmp[1] = dec2hex(minute);
	tmp[2] = dec2hex(hour);
	_i2c.setReg(_address, SECOND, tmp, 3);
}

void Ds3231::setDate(uint8_t year, uint8_t month,
	uint8_t date, uint8_t weekday) {
	uint8_t tmp[4];
	tmp[0] = weekday ? weekday : 7;
	tmp[1] = dec2hex(date);
	tmp[2] = dec2hex(month);
	tmp[3] = dec2hex(year);
	_i2c.setReg(_address, WEEKDAY, tmp, 4);
}

float Ds3231::getTemperature() {
	uint8_t temp[2];
	_i2c.getReg(_address, 0x11, temp, 2);
	int t = make16(temp[0], temp[1]);
	return t / 256.;
}

