/*
 * isl1208-i2csoft.cpp
 *
 *  Created on: Jun 14, 2013
 *      Author: agu
 */

#include "isl1208-i2csoft.h"

Isl1208I2cSoft::Isl1208I2cSoft(Gpio &sda, Gpio &scl) :
		I2cSoft(sda, scl, 0x100) {

}

Isl1208I2cSoft::~Isl1208I2cSoft() {

}

void Isl1208I2cSoft::init() {
	this->I2cSoft::init();
	uint8_t cmd[2] = { 0x07, 0x90 };
	this->write(_ADDRESS, cmd, 2);
}

void Isl1208I2cSoft::refresh() {
	uint8_t cmd = 0;
	this->write(_ADDRESS, &cmd, 1);
	this->read(_ADDRESS, _data, 7);
}

uint8_t Isl1208I2cSoft::getVal(uint8_t index) {
	return _data[index];
}
