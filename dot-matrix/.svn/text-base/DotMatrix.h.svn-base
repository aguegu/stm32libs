/*
	DotMatrix.cpp
	DotMatrix Class for modeling on the Dot Matrix, providing methods like dot control, and serial output
	library for Arduino to drive LED Dot Matrix Display dirven by 74HC595(column) and 74HC138(row)

	Created on: 2012-1-16
	Updated on: 2012-2-26
	Author: Weihong Guan
	Blog: http://aguegu.net
	E-mail: weihong.guan@gmail.com
	Code license: Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
	http://creativecommons.org/licenses/by-nc-sa/3.0/

	source hosting: http://code.google.com/p/aguegu-arduino-library/
 */

#ifndef DOTMATRIX_H_
#define DOTMATRIX_H_

#include "Arduino.h"

class DotMatrix
{
public:
	DotMatrix(byte colCount, byte rowCount);
	virtual ~DotMatrix();

	void clear(boolean b=false);
	void reverseDot(byte col, byte row);
	void setDot(byte col, byte row, boolean b);

	void setByte(byte index,byte value);
	boolean getDot(byte col, byte row) const;

	enum Direction
	{
		Left, Right, Up, Down
	};

	void move(Direction d, boolean recycle = false);


	byte * output() const;
	byte countCol() const;
	byte countRow() const;
	byte countBytes() const;

private:
	byte _colCount;
	byte _rowCount;
	byte _bytesPerRow;
	byte _bytesLength;

	byte * _pScreen;

	inline byte getIndex(byte col, byte row) const;
};

#endif /* DOTMATRIX_H_ */
