/*
 * st7920.h
 *
 *  Created on: Aug 15, 2013
 *      Author: agu
 */

#ifndef ST7920_H_
#define ST7920_H_

#include "stm32-template.h"
//#include "gpio/gpio.h"

class St7920 {

public:
	St7920(Gpio & pin_rs, Gpio & pin_en, Gpio & pin_d4, Gpio & pin_d5,
			Gpio & pin_d6, Gpio & pin_d7);
	virtual ~St7920();

	void init();

	void printWord(char *);
	void clear();
	void home();
	void configureEntry(bool right = true);
	void configureDisplay(bool display = true, bool cursor = false, bool blink =
			false);
	void moveCursor(bool right = true);
	void moveDisplay(bool right = true);
	void configureFunction(bool interface8 = false, bool re = false,
			bool graphic = false);

	void setDdRam(uint8_t address); // Set address in Text Mode

	void transmit(bool isData, uint8_t c);
	void transmit(bool isData, uint8_t *p, uint8_t length);

private:

	Gpio & _pin_rs;
	Gpio & _pin_en;
	Gpio & _pin_d4;
	Gpio & _pin_d5;
	Gpio & _pin_d6;
	Gpio & _pin_d7;

	void setDataPins(uint8_t c, bool high);
	void setCgRam(uint8_t value);
};

#endif /* ST7920_H_ */
