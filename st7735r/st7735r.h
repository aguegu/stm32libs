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
#include "dma/dma.h"

class St7735r {
public:
	St7735r(Spi & spi, Dma & dma, Gpio &pin_ss, Gpio &pin_reset, Gpio &pin_rs, uint16_t offset_x = 2, uint16_t offset_y = 3);
	virtual ~St7735r();
	void init();
	void write8(bool is_data, const uint8_t * data, uint16_t length);
	void write16(bool is_data, const uint16_t * data, uint16_t length);
	void write16(bool is_data, const uint16_t data, uint16_t length = 1);
	void command(uint8_t cmd);
	void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

	typedef struct {
		uint8_t command;   // ST7735 command byte
		uint8_t delay;     // ms delay after
		uint8_t len;       // length of parameter data
		uint8_t data[16];  // parameter data
	} instruction;

private:
	Spi & _spi;
	Dma & _dma;
	Gpio & _pin_ss;
	Gpio & _pin_reset;
	Gpio & _pin_rs;

	const uint16_t _offset_x;
	const uint16_t _offset_y;
	static const instruction initializers[];
};

#endif /* ST7735R_H_ */
