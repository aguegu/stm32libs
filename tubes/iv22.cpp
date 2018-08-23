/*
 * iv22.cpp
 *
 *  Created on: Oct 17, 2013
 *      Author: agu
 */

#include "iv22.h"

const uint8_t Iv22::_font[] = {
		// 0-9
		0xb7, 0x22, 0x9b, 0xab, 0x2e, 0xad, 0xbd, 0x23, 0xbf, 0xaf,

		// a-z
		0xbb, 0xbc, 0x98, 0xba, 0x9f, 0x1d, 0xb5, 0x3c, 0x14, 0xa2, 0x9c, 0x94,
		0x37, 0x38, 0xb8, 0x1f, 0x2f, 0x18, 0x2c, 0x1c, 0xb0, 0x90, 0x0e, 0x0f,
		0x2a, 0x1a, };

Iv22::Iv22(Gpio & pin_din, Gpio & pin_oe, Gpio & pin_st, Gpio & pin_sh,
		uint8_t section_count) :
		_pin_din(pin_din), _pin_oe(pin_oe), _pin_st(pin_st), _pin_sh(pin_sh), _section_count(
				section_count) {

	_buff = (uint8_t *) malloc(
			sizeof(uint8_t) * (section_count) * _BYTE_PER_SECTION);
}

Iv22::~Iv22() {
	free(_buff);
}

void Iv22::init() {
	_pin_din.init(GPIO_Mode_Out_PP);
	_pin_oe.init(GPIO_Mode_Out_PP);
	_pin_st.init(GPIO_Mode_Out_PP);
	_pin_sh.init(GPIO_Mode_Out_PP);

	_pin_oe.set(Bit_RESET);
}

void Iv22::send(uint8_t data) const {
	for (uint8_t i = 8; i--; data <<= 1) {
		_pin_din.set(data & 0x80);
		_pin_sh.set(Bit_SET);
		_pin_sh.set(Bit_RESET);
	}
}

void Iv22::display() {
	for (uint8_t i = 0; i < _section_count; i++) {
		this->send(_buff[i * _BYTE_PER_SECTION]);
		this->send(_buff[i * _BYTE_PER_SECTION + 1]);
	}

	_pin_st.set(Bit_SET);
	_pin_st.set(Bit_RESET);
}

void Iv22::setSection(uint8_t index, uint16_t value) {
	index %= _section_count;
	_buff[index + index] = highByte(value);
	_buff[index + index + 1] = lowByte(value);
}

void Iv22::clear(uint16_t value) {
	for (uint8_t i = 0; i < _section_count; i++)
		this->setSection(i, value);
}

void Iv22::setBackgroundColor(Color color) {
	for (uint8_t i = 0; i < _section_count; i++) {
		this->setBackgroundColor(i, color);
	}
}

void Iv22::setBackgroundColor(uint8_t index, Color color) {
	index %= _section_count;
	_buff[index * _BYTE_PER_SECTION] = color;
}

void Iv22::setPoint(uint8_t index) {
	index %= _section_count;
	_buff[index * _BYTE_PER_SECTION + 1] |= 0x40;
}

void Iv22::setPattern(uint8_t index, uint8_t pattern) {
	_buff[index * _BYTE_PER_SECTION + 1] &= 0x40;
	_buff[index * _BYTE_PER_SECTION + 1] |= pattern;
}

uint8_t Iv22::getPattern(uint8_t index) {
	return _buff[index * _BYTE_PER_SECTION + 1] & 0xbf;
}

bool Iv22::setChar(uint8_t index, char c) {
	index %= _section_count;

	bool val = displayable(c);

	if (val) {
		if (c >= '0' && c <= '9')
			this->setPattern(index, _font[c - '0']);
		else if (c >= 'A' && c <= 'Z')
			this->setPattern(index, _font[c - 'A' + 10]);
		else if (c >= 'a' && c <= 'z')
			this->setPattern(index, _font[c - 'a' + 10]);
	}

	return val;
}

void Iv22::setChar(char c) {
	for (uint8_t i = 0; i < _section_count; i++)
		this->setChar(i, c);
}


void Iv22::printf(const char *__fmt, ...) {
        uint16_t cache_length = _section_count * 2 + 1;
        char * cache = (char *) malloc(sizeof(char) * cache_length);

        va_list ap;
        va_start(ap, __fmt);
        vsnprintf(cache, cache_length, __fmt, ap);
        va_end(ap);

        uint8_t index = 0;
        uint8_t ptr = 0;

        for (uint8_t  i = 0; i < _section_count; i++)
                _buff[i * _BYTE_PER_SECTION + 1] = 0x00;

        while (cache[index] && ptr < _section_count) {
                if (this->setChar(ptr, cache[index]))
                        ptr++;
                else if (cache[index] == '.')
                        this->setPoint(ptr ? ptr - 1 : 0);
                else {
                        _buff[ptr * _BYTE_PER_SECTION + 1] = 0x00;
                        ptr++;
                }

                index++;
        }

        if (cache[index] == '.')
                this->setPoint(ptr - 1);

        free(cache);
}

bool Iv22::displayable(char c) {
        return ((c >= '0' && c <= '9') or (c >= 'A' && c <= 'Z')
                        or (c >= 'a' && c <= 'z'));
}
