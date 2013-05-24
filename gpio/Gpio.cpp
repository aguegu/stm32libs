/*
 * Gpio.cpp
 *
 *  Created on: May 24, 2013
 *      Author: agu
 */

#include "Gpio.h"

Gpio::Gpio(GPIO_TypeDef *port, uint16_t pin) :
		_port(port), _pin(pin) {
	this->init();
}

Gpio::~Gpio() {

}

void Gpio::init() {
	GPIO_InitTypeDef gpio_init_type;
	GPIO_StructInit(&gpio_init_type);

	gpio_init_type.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio_init_type.GPIO_Pin = _pin;
	GPIO_Init(_port, &gpio_init_type);
}

void Gpio::set(BitAction bit) {
	GPIO_WriteBit(_port, _pin, bit);
}
