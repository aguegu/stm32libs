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
	_i2c.init();
	this->setReg(0x0e, 0x9c);
	this->setReg(0x0f, 0x00);
}

uint8_t Ds3231::setReg(uint8_t reg, uint8_t data) {
	uint8_t p[2];
	p[0] = reg;
	p[1] = data;
	return _i2c.write(_address, p, 2);
}

uint8_t Ds3231::getReg(uint8_t reg) {
	uint8_t c;
	_i2c.write(_address, &reg, 1);
	_i2c.read(_address, &c, 1);
	return c;
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
	for (uint8_t i = 0; i < j; i++)
		_datetime[i] = this->getReg(i);
}

void Ds3231::setTime(uint8_t hour, uint8_t minute, uint8_t second) {
	this->setReg(SECOND, dec2hex(second));
	this->setReg(MINUTE, dec2hex(minute));
	this->setReg(HOUR, dec2hex(hour));
}

void Ds3231::setDate(uint8_t year,
	uint8_t month,
	uint8_t date,
	uint8_t weekday) {
	this->setReg(WEEKDAY, weekday ? weekday : 7);
	this->setReg(DATE, dec2hex(date));
	this->setReg(MONTH, dec2hex(month));
	this->setReg(YEAR, dec2hex(year));
}

float Ds3231::getTemperature() {
	uint8_t h = this->getReg(0x11);
	uint8_t l = this->getReg(0x12);

	int t = make16(h, l);
	return t / 256.;
}

