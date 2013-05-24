/*
 * Gpio.h
 *
 *  Created on: May 24, 2013
 *      Author: agu
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

class Gpio {
public:
	Gpio(GPIO_TypeDef *port, uint16_t pin);
	virtual ~Gpio();
	void init();

private:
	GPIO_TypeDef * const _port;
	const uint16_t _pin;
};

#endif /* GPIO_H_ */
