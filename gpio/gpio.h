/*
 * Gpio.h
 *
 *  Created on: May 24, 2013
 *      Author: agu
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "stm32f10x_gpio.h"

class Gpio {
public:
	/* PORTA: 0, PORTB: 1, PORTC: 2, ...  */
	Gpio(uint16_t port, uint16_t pin);
	~Gpio();
	void init(GPIOMode_TypeDef mode = GPIO_Mode_IN_FLOATING, GPIOSpeed_TypeDef speed = GPIO_Speed_2MHz);
	void set(BitAction bit);
	void set(bool bit);
	void toggle();
	BitAction getInput();
	BitAction getOutput();

private:
	GPIO_TypeDef * const _port;
	const uint16_t _pin;
};

#endif /* GPIO_H_ */
