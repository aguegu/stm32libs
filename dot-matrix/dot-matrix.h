/*
 DotMatrix.h
 DotMatrix Class for modeling on the Dot Matrix, providing methods like dot control, draw rectangle, line
 Created on: 2012-01-16

 library for Arduino for Dot Matrix Display, support driver by 74HC595 and 74HC138, ST7920, HD47780
 Author: Weihong Guan
 Blog: http://aguegu.net
 E-mail: weihong.guan@gmail.com
 Code license: Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
 http://creativecommons.org/licenses/by-nc-sa/3.0/

 source host: https://github.com/aguegu/dot-matrix
 */

#ifndef DOTMATRIX_H_
#define DOTMATRIX_H_

#include "stm32f10x.h"
#include "common/bit_op.h"
#include <cstdlib>
#include <cstring>

class DotMatrix {
public:
	DotMatrix(uint8_t col_count, uint8_t row_count);
	virtual ~DotMatrix();

	void clear(uint8_t c = 0x00);
	void setByte(uint16_t index, uint8_t value);
	void putByte(uint8_t col, uint8_t row, uint8_t value);
	uint8_t getByte(uint16_t index);

	void reverseDot(uint8_t col, uint8_t row);
	void setDot(uint8_t col, uint8_t row, bool b = true);
	uint8_t getDot(uint8_t col, uint8_t row) const;

	void setLine(uint8_t cA, uint8_t rA, uint8_t cB, uint8_t rB, bool on = true);
	void setRect(uint8_t cA, uint8_t rA, uint8_t cB, uint8_t rB, bool on = true);

	enum Direction {
		BIT_IN_COL_NEGA,
		BIT_IN_COL_POSI,
		BIT_IN_ROW_NEGA,
		BIT_IN_ROW_POSI,
		BYTE_IN_COL_NEGA,
		BYTE_IN_COL_POSI,
		BIT_IN_BYTE_NEGA,
		BIT_IN_BYTE_POSI
	};

	void move(bool recycle = false);
	void setMoveDirection(Direction d);

	uint8_t * output() const;
	uint8_t countCol() const;
	uint8_t countRow() const;
	uint16_t countBytes() const;
	uint8_t countBytePerRow() const;

	uint8_t orValue();
	uint8_t andValue();

protected:

	void moveBitInColNega(bool recycle);
	void moveBitInColPosi(bool recycle);
	void moveBitInRowNega(bool recycle);
	void moveBitInRowPosi(bool recycle);

	void moveByteInColNega(bool recycle);
	void moveByteInColPosi(bool recycle);

	void moveBitInByteNega(bool recycle);
	void moveBitInBytePosi(bool recycle);

	void (DotMatrix::*_funMoveDirection)(bool);

private:

	uint8_t _col_count;
	uint8_t _row_count;
	uint8_t _bytes_per_row;
	uint16_t _bytes_length;

	uint8_t * _cache;

	uint16_t getIndex(uint8_t col, uint8_t row) const;

};

#endif /* DOTMATRIX_H_ */
