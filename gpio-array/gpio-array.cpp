/*
 * gpio-array.cpp
 *
 *  Created on: Sep 12, 2013
 *      Author: agu
 */

#include "gpio-array.h"

GpioArray::GpioArray(uint8_t length) :
		_length(length) {
	assert_param(_length <= 32);
	_gpios = (Gpio **) malloc(sizeof(Gpio *) * _length);
	for (uint8_t i = 0; i < _length; i++)
		_gpios[i] = NULL;
}

GpioArray::~GpioArray() {
	free(_gpios);
}

void GpioArray::setGpio(uint8_t index, Gpio & gpio) {
	assert_param(index < _length);
	_gpios[index] = &gpio;
}

void GpioArray::init(GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed) {
	for (uint8_t i = 0; i < _length; i++) {
		assert_param(_gpios[i] != NULL);
		_gpios[i]->init(mode, speed);
	}
}

void GpioArray::set(uint32_t val) {
	for (uint8_t i = 0; i < _length; i++) {
		assert_param(_gpios[i] != NULL);
		_gpios[i]->set(bitRead(val, i));
	}
}

void GpioArray::toggle() {
	this->set(this->getOutput() ^ 0xffffffff);
}

uint32_t GpioArray::getOutput() {
	uint32_t val = 0;
	for (uint8_t i = 0; i < _length; i++) {
		assert_param(_gpios[i] != NULL);
		bitWrite(val, i, _gpios[i]->getOutput());
	}
	return val;
}

uint32_t GpioArray::getInput() {
	uint32_t val = 0;
	for (uint8_t i = 0; i < _length; i++) {
		assert_param(_gpios[i] != NULL);
		bitWrite(val, i, _gpios[i]->getInput());
	}
	return val;
}
