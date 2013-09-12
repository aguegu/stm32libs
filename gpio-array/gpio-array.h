/*
 * gpio-array.h
 *
 *  Created on: Sep 12, 2013
 *      Author: agu
 */

#ifndef GPIO_ARRAY_H_
#define GPIO_ARRAY_H_

#include "gpio/gpio.h"
#include "common/bit_op.h"
#include <cstdlib>

class GpioArray {
public:
	GpioArray(uint8_t length);
	~GpioArray();
	void setGpio(uint8_t index, Gpio & gpio);
	void init(GPIOMode_TypeDef mode = GPIO_Mode_IN_FLOATING, GPIOSpeed_TypeDef speed = GPIO_Speed_2MHz);
	void set(uint32_t val);
private:
	Gpio ** _gpios;
	const uint8_t _length;
};

#endif /* GPIO_ARRAY_H_ */
