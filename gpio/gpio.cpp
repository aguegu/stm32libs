/*
 * Gpio.cpp
 *
 *  Created on: May 24, 2013
 *      Author: agu
 */

#include "gpio.h"

Gpio::Gpio(uint16_t port, uint16_t pin) :
		_port((GPIO_TypeDef *) (GPIOA_BASE + (GPIOB_BASE - GPIOA_BASE) * (port))),
		_pin(1 << pin) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA << (port), ENABLE);
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

void Gpio::set(bool bit) {
	GPIO_WriteBit(_port, _pin, bit ? Bit_SET : Bit_RESET);
}

BitAction Gpio::getInput() {
	return (BitAction) GPIO_ReadInputDataBit(_port, _pin);
}

BitAction Gpio::getOutput() {
	return (BitAction) GPIO_ReadOutputDataBit(_port, _pin);
}

void Gpio::toggle() {
	GPIO_WriteBit(_port, _pin,
			(BitAction) (!GPIO_ReadOutputDataBit(_port, _pin)));
}
