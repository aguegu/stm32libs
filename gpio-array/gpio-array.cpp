/*
 * gpio-array.cpp
 *
 *  Created on: Sep 12, 2013
 *      Author: agu
 */

#include "gpio-array.h"

GpioArray::GpioArray(uint8_t length) :
		_length(length) {
	_gpios = (Gpio **) malloc(sizeof(Gpio *) * _length);
	for (uint8_t i = 0; i < _length; i++)
		_gpios[i] = NULL;
}

GpioArray::~GpioArray() {
	free(_gpios);
}

void GpioArray::setGpio(uint8_t index, Gpio & gpio) {
	_gpios[index] = &gpio;
}

void GpioArray::init(GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed) {
	for (uint8_t i = 0; i < _length; i++) {
		_gpios[i]->init(mode, speed);
	}
}

void GpioArray::set(uint32_t val) {
	for (uint8_t i = 0; i < _length; i++) {
		_gpios[i]->set(bitRead(val, i));
	}
}
