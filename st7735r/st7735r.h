/*
 * st7735r.h
 *
 *  Created on: Jun 4, 2013
 *      Author: agu
 */

#ifndef ST7735R_H_
#define ST7735R_H_

#include "stm32-template.h"
#include "spi/spi.h"
#include "gpio/gpio.h"

class St7735r {
public:
	St7735r(Spi & spi, Gpio &pin_ss, Gpio &pin_reset, Gpio &pin_rs);
	virtual ~St7735r();
	void init();
	void write8(uint8_t is_data, const uint8_t * data, uint8_t length);
	void write16(uint8_t is_data, const uint16_t * data, uint8_t length);
	void write16(uint8_t is_data, const uint16_t data);
	void command(uint8_t cmd);
	void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
	void pushColor(uint16_t color, uint16_t length);
private:
	Spi & _spi;
	Gpio & _pin_ss;
	Gpio & _pin_reset;
	Gpio & _pin_rs;
};

#endif /* ST7735R_H_ */
