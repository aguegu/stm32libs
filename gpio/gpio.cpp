/*
 * Gpio.cpp
 *
 *  Created on: May 24, 2013
 *      Author: agu
 */

#include "gpio.h"

Gpio::Gpio(GPIO_TypeDef *port, uint16_t pin, uint32_t rcc_apb2_periph) :
		_port(port), _pin(pin) {
	RCC_APB2PeriphClockCmd(rcc_apb2_periph, ENABLE);
}

Gpio::~Gpio() {

}

void Gpio::init(GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed) {
	GPIO_InitTypeDef gpio_init_type;

	gpio_init_type.GPIO_Pin = _pin;
	gpio_init_type.GPIO_Mode = mode;
	gpio_init_type.GPIO_Speed = speed;

	GPIO_Init(_port, &gpio_init_type);
}

void Gpio::set(BitAction bit) {
	GPIO_WriteBit(_port, _pin, bit);
}

BitAction Gpio::get() {
	return (BitAction)GPIO_ReadInputDataBit(_port, _pin);
}
