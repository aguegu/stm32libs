/*
 * hd44780-basic.cpp
 *
 *  Created on: Aug 11, 2013
 *      Author: agu
 */

#include "hd44780/hd44780-basic.h"

const uint8_t Hd44780Basic::BAR_PATTERN[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 00000000
	0x1f, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 11000000
	0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00, // 00011000
	0x1f, 0x1f, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00, // 11011000
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x1f, // 00000011
	0x1f, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x1f, // 11000011
	0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x1f, 0x1f, // 00011011
	0x1f, 0x1f, 0x00, 0x1f, 0x1f, 0x00, 0x1f, 0x1f, // 11011011
};

const uint8_t Hd44780Basic::ROW_INDEX_16[] = { 0x00, 0x40, 0x10, 0x50 };

const uint8_t Hd44780Basic::ROW_INDEX_20[] = { 0x00, 0x40, 0x14, 0x54 };

Hd44780Basic::Hd44780Basic(uint8_t row_count, uint8_t col_count) :
		_row_count(row_count), _col_count(col_count) {
	_cache_length = _row_count * _col_count + 1;
	_cache = (char *) malloc(sizeof(char) * _cache_length);
	this->setCache();
}

Hd44780Basic::~Hd44780Basic() {
	free(_cache);
}

void Hd44780Basic::setCache(uint8_t value) {
	memset(_cache, value, _cache_length);
}

void Hd44780Basic::setCache(uint8_t index, uint8_t value) {
	assert_param(index < _cache_length);
	_cache[index] = value;
}

void Hd44780Basic::putString(uint8_t address, char *p, uint8_t length) const {
	this->setCursor(address);
	while (length--)
		transmit(true, *p++);
}

void Hd44780Basic::putChar(uint8_t address, char c) const {
	this->putString(address, &c, 1);
}

void Hd44780Basic::init() {
	this->initHardware();

	this->setCGRam(BAR_PATTERN, 64);
	this->configureDisplay(true, false, false);
}

void Hd44780Basic::putCache() const {
	for (uint8_t r = 0; r < _row_count; r++)
		this->putString(*((_col_count <= 16 ? ROW_INDEX_16 : ROW_INDEX_20) + r),
				_cache + _col_count * r, _col_count);
}

void Hd44780Basic::printf(uint8_t index, const char *__fmt, ...) {
	assert_param(index < _cache_length);

	va_list ap;
	va_start(ap, __fmt);
	vsnprintf(_cache + index, _cache_length - index, __fmt, ap);
	va_end(ap);
}

void Hd44780Basic::printf(const char *__fmt, ...) {
	va_list ap;
	va_start(ap, __fmt);
	this->printf(0, __fmt, ap);
	va_end(ap);
}

// 0x01
void Hd44780Basic::clear() const {
	transmit(false, 0x01);
	delayMicroseconds(2000);
}

// 0x02
void Hd44780Basic::rst() const {
	transmit(false, 0x02);
	delayMicroseconds(2000);
}

// 0x04
void Hd44780Basic::configureInput(bool ac, bool screen_move) const {
	uint8_t cmd = 0x04;

	if (ac)
		cmd |= 0x02;
	if (screen_move)
		cmd |= 0x01;

	transmit(false, cmd);
}

// 0x08
void Hd44780Basic::configureDisplay(bool display_on, bool cursor,
		bool blink) const {
	uint8_t cmd = 0x08;
	if (display_on)
		cmd |= 0x04;
	if (cursor)
		cmd |= 0x02;
	if (blink)
		cmd |= 0x01;

	transmit(false, cmd);
}

// 0x10
void Hd44780Basic::moveCursor(bool right) const {
	uint8_t cmd = 0x10;
	if (right)
		cmd |= 0x04;
	transmit(false, cmd);
}

// 0x11
void Hd44780Basic::moveScreen(bool right) const {
	uint8_t cmd = 0x11;
	if (right)
		cmd |= 0x04;
	transmit(false, cmd);
}

// 0x20
void Hd44780Basic::configureFunction(bool interface8, bool doubleline,
		bool font5x10) const {
	uint8_t cmd = 0x20;
	if (interface8)
		cmd |= 0x10;
	if (doubleline)
		cmd |= 0x08;
	if (font5x10)
		cmd |= 0x04;
	transmit(false, cmd);
}

void Hd44780Basic::setCGRam(uint8_t const *p, uint8_t length) const {
	this->configureInput(true, false);
	transmit(false, 0x40);

	while (length--)
		transmit(true, *p++);
}

void Hd44780Basic::setCursor(uint8_t address) const {
	transmit(false, address | 0x80);
}
