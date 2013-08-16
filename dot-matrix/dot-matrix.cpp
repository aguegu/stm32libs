/*
 DotMatrix.cpp
 DotMatrix Class for modeling on the Dot Matrix, providing methods like dot control, draw rectangle, line
 Created on: 2012-01-16
 Updated on: 2012-05-14

 library for Arduino for Dot Matrix Display, support driver by 74HC595 and 74HC138, ST7920, HD47780
 Author: Weihong Guan
 Blog: http://aguegu.net
 E-mail: weihong.guan@gmail.com
 Code license: Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
 http://creativecommons.org/licenses/by-nc-sa/3.0/

 source host: https://github.com/aguegu/dot-matrix
 */

#include "dot-matrix.h"

DotMatrix::DotMatrix(uint8_t colCount, uint8_t rowCount) {
	_col_count = colCount;
	_row_count = rowCount;

	_bytes_per_row = _col_count / 8;
	if (_col_count % 8)
		_bytes_per_row++;

	_bytes_length = _bytes_per_row * _row_count;

	_cache = (uint8_t *) malloc(sizeof(uint8_t) * (_bytes_length + 1));
	setMoveDirection(this->BYTE_IN_COL_POSI);

	this->clear(0x00);
}

DotMatrix::~DotMatrix() {
	free(_cache);
}

void DotMatrix::clear(uint8_t c) {
	memset(_cache, c, _bytes_length);
}

void DotMatrix::reverseDot(uint8_t col, uint8_t row) {
	this->setDot(col, row, !getDot(col, row));
}

void DotMatrix::setDot(uint8_t col, uint8_t row, bool b) {
	uint16_t i = getIndex(col, row);

	writeHighBitInByte(*(_cache+i), col & 0x07, b);
}

void DotMatrix::setLine(uint8_t cA, uint8_t rA, uint8_t cB, uint8_t rB, bool on) {
	uint8_t cMin, cMax, rMin, rMax;

	cMin = rawmin(cA, cB);
	cMin = rawmax(cMin, 0);

	cMax = rawmax(cA, cB);
	cMax = rawmin(cMax, _col_count - 1);

	rMin = rawmin(rA, rB);
	rMin = rawmax(rMin, 0);

	rMax = rawmax(rA, rB);
	rMax = rawmin(rMax, _row_count - 1);

	if (cMax - cMin >= rMax - rMin) {
		for (uint8_t c = cMin; c <= cMax; c++) {
			int d = 32767;
			for (uint8_t r = rMin; r <= rMax; r++) {
				int k = abs((c - cA) * (rA - rB) - (r - rA) * (cA - cB));
				if (d > k)
					d = k;
			}

			for (uint8_t r = rMin; r <= rMax; r++) {
				if (abs((c - cA) * (rA - rB) - (r - rA) * (cA - cB)) == d)
					this->setDot(c, r, on);
			}
		}
	} else {
		for (uint8_t r = rMin; r <= rMax; r++) {
			int d = 32767;
			for (uint8_t c = cMin; c <= cMax; c++) {
				int k = abs((c - cA) * (rA - rB) - (r - rA) * (cA - cB));
				if (d > k)
					d = k;
			}

			for (uint8_t c = cMin; c <= cMax; c++) {
				if (abs((c - cA) * (rA - rB) - (r - rA) * (cA - cB))
						== d)
					this->setDot(c, r, on);
			}
		}
	}
}

void DotMatrix::setRect(uint8_t cA, uint8_t rA, uint8_t cB, uint8_t rB, bool on) {
	uint8_t cMin, cMax, rMin, rMax;

	cMin = rawmin(cA, cB);
	cMin = rawmax(cMin, 0);

	cMax = rawmax(cA, cB);
	cMax = rawmin(cMax, this->_col_count - 1);

	rMin = rawmin(rA, rB);
	rMin = rawmax(rMin, 0);

	rMax = rawmax(rA, rB);
	rMax = rawmin(rMax, this->_row_count - 1);

	for (uint8_t c = cMin; c <= cMax; c++)
		for (uint8_t r = rMin; r <= rMax; r++)
			this->setDot(c, r, on);
}

uint16_t DotMatrix::getIndex(uint8_t col, uint8_t row) const {
	uint16_t index = row * _bytes_per_row + (col >> 3);
	return index < _bytes_length ? index : 0;
}

uint8_t DotMatrix::getDot(uint8_t col, uint8_t row) const {
	uint16_t i = getIndex(col, row);
	return readHighBitInByte(_cache[i], col & 0x07);
}

uint8_t * DotMatrix::output() const {
	return _cache;
}

uint8_t DotMatrix::countCol() const {
	return _col_count;
}

uint8_t DotMatrix::countRow() const {
	return _row_count;
}

uint8_t DotMatrix::countBytePerRow() const {
	return _bytes_per_row;
}

uint16_t DotMatrix::countBytes() const {
	return _bytes_length;
}

void DotMatrix::setByte(uint16_t index, uint8_t value) {
	if (index >= _bytes_length)
		return;
	_cache[index] = value;
}

void DotMatrix::putByte(uint8_t col, uint8_t row, uint8_t value) {
	uint16_t i = getIndex(col, row);
	uint8_t j = col & 0x07;
	_cache[i] |= value >> j;
	_cache[i + 1] |= value << (8 - j);
}

uint8_t DotMatrix::getByte(uint16_t index) {
	return index >= _bytes_length ? 0 : _cache[index];
}

void DotMatrix::moveBitInColNega(bool recycle) {
	uint8_t *p = _cache;
	for (uint8_t r = _row_count; r--;) {
		bool b0 = readHighBitInByte(*p, 0);
		for (uint8_t i = _bytes_per_row - 1; i--;)
		{
			bool b = readHighBitInByte(*(p+1), 0);
			*p <<= 1;
			writeLowBitInByte(*p, 0, b);
			p++;
		}

		*p <<= 1;
		if (recycle)
			writeLowBitInByte(*p, 0, b0);

		p++;
	}
}

void DotMatrix::moveBitInColPosi(bool recycle) {
	uint8_t *p = _cache + _bytes_per_row - 1;
	for (uint8_t r = _row_count; r--;) {
		bool b0 = readLowBitInByte(*p, 0);
		for (uint8_t i = _bytes_per_row - 1; i--;)
		{
			bool b = readLowBitInByte(*(p-1), 0);
			*p >>= 1;
			writeHighBitInByte(*p, 0, b);
			p--;
		}

		*p >>= 1;
		if (recycle)
			writeHighBitInByte(*p, 0, b0);

		p += _bytes_per_row + _bytes_per_row - 1;
	}
}

void DotMatrix::moveBitInRowNega(bool recycle) {
	uint8_t pTemp[_bytes_per_row];
	memcpy(pTemp, _cache, _bytes_per_row);

	memcpy(_cache, _cache + _bytes_per_row, _bytes_length - _bytes_per_row);

	recycle ?
			memcpy(_cache + _bytes_length - _bytes_per_row, pTemp,
					_bytes_per_row) :
			memset(_cache + _bytes_length - _bytes_per_row, 0x00,
					_bytes_per_row);
}

void DotMatrix::moveBitInRowPosi(bool recycle) {
	uint8_t pTemp[_bytes_per_row];
	memcpy(pTemp, _cache + _bytes_length - _bytes_per_row, _bytes_per_row);

	memmove(_cache + _bytes_per_row, _cache,
			_bytes_length - _bytes_per_row);

	recycle ?
			memcpy(_cache, pTemp, _bytes_per_row) :
			memset(_cache, 0x00, _bytes_per_row);
}

void DotMatrix::moveByteInColNega(bool recycle) {
	uint8_t * p = _cache;
	for (uint8_t r = _row_count; r--;) 	{
		uint8_t temp = recycle ? *p : 0x00;
		memcpy(p, p + 1, _bytes_per_row - 1);

		p += _bytes_per_row;
		*(p - 1) = temp;
	}
}

void DotMatrix::moveByteInColPosi(bool recycle) {
	uint8_t *p = _cache;
	for (uint8_t r = _row_count; r--;) {
		uint8_t temp = recycle ? *(p + _bytes_per_row - 1) : 0x00;
		memmove(p + 1, p, _bytes_per_row - 1);
		*p = temp;
		p += _bytes_per_row;
	}
}

void DotMatrix::moveBitInByteNega(bool recycle) {
	uint8_t *p = _cache;
	for (uint16_t index = _bytes_length; index--; p++) 	{
		bool temp = *p & 0x80;
		*p <<= 1;
		if (recycle && temp)
			*p |= 0x01;
	}
}

void DotMatrix::moveBitInBytePosi(bool recycle) {
	uint8_t *p = _cache;
	for (uint16_t index = _bytes_length; index--; p++) {
		bool temp = *p & 0x01;
		*p >>= 1;
		if (recycle && temp)
			*p |= 0x80;
	}
}

void DotMatrix::setMoveDirection(Direction d) {
	switch (d) {
	case BIT_IN_COL_NEGA:
		_funMoveDirection = &DotMatrix::moveBitInColNega;
		break;
	case BIT_IN_COL_POSI:
		_funMoveDirection = &DotMatrix::moveBitInColPosi;
		break;
	case BIT_IN_ROW_NEGA:
		_funMoveDirection = &DotMatrix::moveBitInRowNega;
		break;
	case BIT_IN_ROW_POSI:
		_funMoveDirection = &DotMatrix::moveBitInRowPosi;
		break;
	case BYTE_IN_COL_NEGA:
		_funMoveDirection = &DotMatrix::moveByteInColNega;
		break;
	case BYTE_IN_COL_POSI:
		_funMoveDirection = &DotMatrix::moveByteInColPosi;
		break;
	case BIT_IN_BYTE_NEGA:
		_funMoveDirection = &DotMatrix::moveBitInByteNega;
		break;
	case BIT_IN_BYTE_POSI:
		_funMoveDirection = &DotMatrix::moveBitInBytePosi;
		break;
	}
}

void DotMatrix::move(bool recycle) {
	(this->*_funMoveDirection)(recycle);
}

uint8_t DotMatrix::orValue() {
	uint8_t c = 0x00;
	for (uint16_t i = _bytes_length; i--;)
		c |= _cache[i];
	return c;
}

uint8_t DotMatrix::andValue() {
	uint8_t c = 0xff;
	for (uint16_t i = _bytes_length; i--;)
		c &= _cache[i];

	return c;
}
