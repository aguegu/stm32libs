/*
 DotString.cpp
 DotString Class for string printing with DotFont
 Created on: 2012-01-25
 Updated on: 2012-05-14

 library for Arduino for Dot Matrix Display, support driver by 74HC595 and 74HC138, ST7920, HD47780
 Author: Weihong Guan
 Blog: http://aguegu.net
 E-mail: weihong.guan@gmail.com
 Code license: Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
 http://creativecommons.org/licenses/by-nc-sa/3.0/

 source host: https://github.com/aguegu/dot-matrix
 */
#include "dot-string.h"

DotString::DotString(DotChar & dc, uint8_t length, bool smart_width) :
		_dc(dc) {
	_length = length;
	_smart_width = smart_width;
	_str = (char *) malloc(sizeof(char) * _length);
}

DotString::~DotString() {
	free(_str);
}

void DotString::printf(const char *__fmt, ...) {
	va_list ap;
	va_start(ap, __fmt);
	vsnprintf(_str, _length, __fmt, ap);
	va_end(ap);
}

void DotString::postAt(uint8_t col, uint8_t row) {
	uint8_t cursor;

	if (row > (_dc.getVertical() ? _dc.getMaxRow() : _dc.getMaxCol()))
		return;

	char *p = _str;

	cursor = col;
	while (*p) {
		_dc.setChar(*p++);
		_dc.postAt(cursor, row);
		cursor +=
				_smart_width ? _dc.calcFontRealWidth() + 1 : _dc.getWidth() + 1;
		if (cursor > (_dc.getVertical() ? _dc.getMaxCol() : _dc.getMaxRow()))
			break;
	}
}

void DotString::setChar(uint8_t index, char chr) {
	assert_param(index < _length);
	*(_str + index) = chr;
}

char * DotString::getString() {
	return _str;
}
