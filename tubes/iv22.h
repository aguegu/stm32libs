/*
 * iv22.h
 *
 *  Created on: Oct 17, 2013
 *      Author: agu
 */

#ifndef IV22_H_
#define IV22_H_

#include "gpio/gpio.h"
#include "stdlib.h"
#include "common/bit_op.h"
#include <cstdarg>
#include <cstdio>

enum Color {
	White, Yellow, Magenta, Red, Cyan, Green, Blue, Black,
};

class Iv22 {
public:
	Iv22(Gpio & pin_din, Gpio & pin_oe, Gpio & pin_st, Gpio & pin_sh,
			uint8_t section_count);
	virtual ~Iv22();

	void init();
	void display();

	void setSection(uint8_t index, uint16_t value = 0xff00);
	void clear(uint16_t value = 0xff00);

	void setBackgroundColor(Color color);
	void setBackgroundColor(uint8_t index, Color color);

	void setBrightness(uint8_t brightness = 0xff);

	void setPoint(uint8_t index);
	void setPattern(uint8_t index, uint8_t pattern);
	uint8_t getPattern(uint8_t index);

	bool setChar(uint8_t index, char c);
	void setChar(char c);

	void printf(const char *__fmt, ...);

private:

	uint8_t *_buff;

	Gpio & _pin_din; // DIN
	Gpio & _pin_oe; // OE, pin with PWM output is recommended to enable brightness adjustment
	Gpio & _pin_st; // STCP
	Gpio & _pin_sh; // SHCP

	const uint8_t _section_count;
	static const uint8_t _BYTE_PER_SECTION = 2;

	void send(uint8_t data) const;
	bool displayable(char c);

	static const uint8_t _font[];


};

#endif /* IV22_H_ */
