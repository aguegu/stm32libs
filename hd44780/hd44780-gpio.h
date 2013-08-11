/*
 * hd44780-gpio.h
 *
 *  Created on: Aug 11, 2013
 *      Author: agu
 */

#ifndef HD44780_GPIO_H_
#define HD44780_GPIO_H_

#include "hd44780-basic.h"
#include "gpio/gpio.h"

class Hd44780Gpio: public Hd44780Basic {
public:
	Hd44780Gpio(Gpio & pin_rs, Gpio & pin_en, Gpio & pin_d4, Gpio & pin_d5,
			Gpio & pin_d6, Gpio & pin_d7, uint8_t row_count = 4,
			uint8_t col_count = 16);
	virtual ~Hd44780Gpio();

private:
	Gpio  & _pin_rs;
	Gpio  & _pin_en;
	Gpio  & _pin_d4;
	Gpio  & _pin_d5;
	Gpio  & _pin_d6;
	Gpio  & _pin_d7;

	void setDT(uint8_t c, bool b) const;
	void setData(uint8_t c) const;

	void transmit(bool isData, uint8_t val) const;

	void initHardware() const;
};

#endif /* HD44780_GPIO_H_ */
