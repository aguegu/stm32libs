/*
	DotMatrix.h
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

#include "DotMatrix.h"

DotMatrix::DotMatrix(byte colCount, byte rowCount)
{
	_colCount = colCount;
	_rowCount = rowCount;

	_bytesPerRow = _colCount / 8;
	_bytesLength = _bytesPerRow * _rowCount;

	_pScreen = (byte *) malloc(sizeof(byte) * _bytesLength);
}

DotMatrix::~DotMatrix()
{
	free(_pScreen);
}

void DotMatrix::clear(boolean b)
{
	byte c = b ? 0xff : 0x00;
	for (byte i = 0; i < _bytesLength; i++)
	{
		_pScreen[i] = c;
	}
}

void DotMatrix::reverseDot(byte col, byte row)
{
	if (getDot(col, row))
		setDot(col, row, false);
	else
		setDot(col, row, true);
}

void DotMatrix::setDot(byte col, byte row, boolean b)
{
	int i = getIndex(col, row);

	if (b != -1)
		bitWrite(_pScreen[i], (col & 0x07), b);
}

byte DotMatrix::getIndex(byte col, byte row) const
{
	if (col < 0 || col >= _colCount)
		return -1;
	if (row < 0 || row >= _rowCount)
		return -1;
	return (row * _bytesPerRow + (col >> 3));
}

boolean DotMatrix::getDot(byte col, byte row) const
{
	boolean b = false;
	int i = getIndex(col, row);

	if (i != -1)
		b = bitRead(_pScreen[i], (col & 0x07));

	return b;
}

byte * DotMatrix::output() const
{
	return _pScreen;
}

byte DotMatrix::countCol() const
{
	return _colCount;
}

byte DotMatrix::countRow() const
{
	return _rowCount;
}

byte DotMatrix::countBytes() const
{
	return _bytesLength;
}

void DotMatrix::setByte(byte index, byte value)
{
	if (index >= _bytesLength)
		return;
	_pScreen[index] = value;
}

void DotMatrix::move(Direction d, boolean recycle)
{
	byte pTemp[_bytesPerRow];
	switch (d)
	{
	case Left:
		for (byte r = 0; r < _rowCount; r++)
		{
			byte index = r * _bytesPerRow;
			boolean b0 = bitRead(_pScreen[index], 0);
			for (byte i = 1; i < _bytesPerRow; i++)
			{
				boolean b = bitRead(_pScreen[index + 1], 0);
				_pScreen[index] >>= 1;
				bitWrite(_pScreen[index], 7, b);
				index++;
			}

			_pScreen[index] >>= 1;

			if (recycle)
				bitWrite(_pScreen[index], 7, b0);
		}
		break;
	case Right:
		for (byte r = 0; r < _rowCount; r++)
		{
			byte index = r * _bytesPerRow + _bytesPerRow - 1;
			boolean b0 = bitRead(_pScreen[index], 7);
			for (byte i = 1; i < _bytesPerRow; i++)
			{
				boolean b = bitRead(_pScreen[index - 1], 7);
				_pScreen[index] <<= 1;
				bitWrite(_pScreen[index], 0, b);
				index--;
			}

			_pScreen[index] <<= 1;

			if (recycle)
				bitWrite(_pScreen[index], 0, b0);
		}
		break;
	case Up:
		for (byte i = 0; i < _bytesPerRow; i++)
			pTemp[i] = _pScreen[i];

		for (byte r = 0; r < _rowCount - 1; r++)
		{
			byte index = r * _bytesPerRow;
			for (byte i = 0; i < _bytesPerRow; i++)
			{
				_pScreen[index] = _pScreen[index + _bytesPerRow];
				index++;
			}
		}

		for (byte index = _bytesPerRow * (_rowCount - 1), i = 0;
				i < _bytesPerRow; i++)
			_pScreen[index++] = recycle ? pTemp[i] : 0x00;

		break;
	case Down:
		for (byte i = 0, index=(_rowCount-1) * _bytesPerRow; i < _bytesPerRow; i++)
			pTemp[i] = _pScreen[index++];

		for (byte r = _rowCount - 1; r > 0; r--)
		{
			byte index = r * _bytesPerRow;
			for (byte i = 0; i < _bytesPerRow; i++)
			{
				_pScreen[index] = _pScreen[index - _bytesPerRow];
				index++;
			}
		}

		for (byte i = 0;	i < _bytesPerRow; i++)
			_pScreen[i] = recycle ? pTemp[i] : 0x00;

		break;

	}
}
