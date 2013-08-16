/*
 DotChar.cpp
 DotChar Class for printing characters on DotMatrix
 Created on: 2012-01-22
 Updated on: 2012-05-14

 library for Arduino for Dot Matrix Display, support driver by 74HC595 and 74HC138, ST7920, HD47780
 Author: Weihong Guan
 Blog: http://aguegu.net
 E-mail: weihong.guan@gmail.com
 Code license: Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
 http://creativecommons.org/licenses/by-nc-sa/3.0/

 source host: https://github.com/aguegu/dot-matrix
 */

#include "dot-char.h"

DotChar::DotChar(DotMatrix & dm, const DotFont & df) :
		_dm(dm), _df(df) {
	_col = _row = _index = _indent = 0;
	_vertical = true;
}

DotChar::~DotChar() {

}

void DotChar::setChar(const char chr) {
	if (chr < _df.indent || chr >= _df.indent + _df.length)
		_index = _df.length;
	else
		_index = chr - _df.indent;

	_indent = _index * _df.width;
}

void DotChar::setVertical(const bool vertical) {
	_vertical = vertical;
}

void DotChar::postAt(const uint8_t col, const uint8_t row) {
	_col = col;
	_row = row;
	this->print();
}

void DotChar::clear(const bool on) const {
	if (_vertical)
		_dm.setRect(_col, _row, _col + _df.width, _row + _df.height, on);
	else
		_dm.setRect(_row, this->getMaxRow() - _col, _row + _df.height,
			this->getMaxCol() - _col - _df.width, on);
}

uint8_t DotChar::calcFontRealWidth() const {
	if (_index == _df.length)
		return _df.width;

	uint8_t i = _df.width - 1;

	while (*(_df.pattern + _indent + i) == 0 && i--)
		;

	return i + 1;
}

uint8_t DotChar::getWidth() const {
	return _df.width;
}

uint8_t DotChar::getHeight() const {
	return _df.height;
}

bool DotChar::getVertical() const {
	return _vertical;
}

uint8_t DotChar::getMaxCol() const {
	return _dm.countCol() - 1;
}

uint8_t DotChar::getMaxRow() const {
	return _dm.countRow() - 1;
}

void DotChar::print() const {
	if (_index == _df.length)
		this->clear(false);
	else
		_vertical ? printV() : printH();
}

void DotChar::printH() const {
	for (uint8_t c = 0; c < _df.width; c++) {
		for (uint8_t r = 0; r < _df.height; r++) {
			if (bool b = bitRead(*(_df.pattern + _indent + c), r))
				_dm.setDot(_row + r, _dm.countRow() - _col - 1 - c, b);
		}
	}
}

void DotChar::printV() const {
	for (uint8_t c = 0; c < _df.width && c + _col < _dm.countCol(); c++) {
		for (uint8_t r = 0; r < _df.height; r++) {
			if (bool b = bitRead(*(_df.pattern + _indent + c), r))
				_dm.setDot(_col + c, _row + r, b);
		}
	}
}
