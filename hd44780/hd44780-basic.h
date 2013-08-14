/*
 * hd44780-basic.h
 *
 *  Created on: Aug 11, 2013
 *      Author: agu
 */

#ifndef HD44780_BASIC_H_
#define HD44780_BASIC_H_

#include "stm32-template.h"
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <cstdio>

class Hd44780Basic {
public:
	Hd44780Basic(uint8_t row_count, uint8_t col_count);
	virtual ~Hd44780Basic();

	void init();
	void clear() const;
	void rst() const;

	void setCursor(uint8_t address) const;
	void moveCursor(bool right) const;
	void moveScreen(bool right) const;

	void configureDisplay(bool display_on = true, bool cursor = true,
			bool blink = true) const;
	void setCGRam(uint8_t  const *pFont, uint8_t length) const;

	void putString(uint8_t address, char *p, uint8_t length) const;
	void putChar(uint8_t address, char c) const;
	void putCache() const;

	void setCache(uint8_t value = 0x00);
	void setCache(uint8_t index, uint8_t value);
	void printf(const char *__fmt, ...);
	void printf(uint8_t index, const char *__fmt, ...);

protected:

	const uint8_t _row_count;
	const uint8_t _col_count;
	uint8_t _cache_length;
	char * _cache;

	static const uint8_t BAR_PATTERN[];
	static const uint8_t ROW_INDEX_16[];
	static const uint8_t ROW_INDEX_20[];

	virtual void initHardware() const = 0;
	virtual void transmit(bool isData, uint8_t val) const = 0;

	void configureInput(bool ac = true, bool screen_move = false) const;
	void configureFunction(bool interface8 = false, bool doubleline = true,
			bool font5x10 = false) const;
};

#endif /* HD44780_BASIC_H_ */
