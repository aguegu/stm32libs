/*
	DotFont.cpp
	DotFont Class for printing characters on DotMatrix
	library for Arduino to drive LED Dot Matrix Display dirven by 74HC595(column) and 74HC138(row)

	Created on: 2012-1-22
	Updated on: 2012-3-09
	Author: Weihong Guan
	Blog: http://aguegu.net
	E-mail: weihong.guan@gmail.com
	Code license: Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
	http://creativecommons.org/licenses/by-nc-sa/3.0/

	source hosting: http://code.google.com/p/aguegu-arduino-library/
*/

#include "DotFont.h"

DotFont::DotFont(DotMatrix * pDM, byte * pattern, byte unitWidth,
		byte unitHeight, byte index, byte col, byte row, boolean direction)
{
	_pDM = pDM;
	_pattern = pattern;
	_unitWidth = pgm_read_byte_near(unitWidth);
	_unitHeight = pgm_read_byte_near(unitHeight);
	_col = col;
	_row = row;
	_index = index;
	_direction = direction;
}

DotFont::DotFont()
{

}

byte DotFont::getWidth()
{
	return _unitWidth;
}

byte DotFont::getHeight()
{
	return _unitHeight;
}

DotFont::DotFont(DotMatrix * pDM)
{
	this->setDotMatrix(pDM);
}

void DotFont::setDotMatrix(DotMatrix * pDM)
{
	_pDM = pDM;
}

DotFont::~DotFont()
{
	// TODO Auto-generated destructor stub
}

void DotFont::setPattern(byte * pattern, byte * unitWidth, byte * unitHeight)
{
	_pattern = pattern;
	_unitWidth = pgm_read_byte_near(unitWidth);
	_unitHeight = pgm_read_byte_near(unitHeight);
}

void DotFont::setIndex(byte index)
{
	_index = index;
}

void DotFont::setDirection(boolean direction)
{
	_direction = direction;
}

void DotFont::moveTo(byte col, byte row)
{
	_col = col;
	_row = row;
}

void DotFont::clear(byte col, byte row)
{
	for (byte c = 0; c < _unitWidth; c++)
	{
		for (byte r = 0; r < _unitHeight; r++)
		{
			_pDM->setDot(col + c, row + r, false);
		}
	}
}

void DotFont::fill(byte col, byte row)
{
	for (byte c = 0; c < _unitWidth; c++)
	{
		for (byte r = 0; r < _unitHeight; r++)
		{
			_pDM->setDot(col + c, row + r, true);
		}
	}
}

void DotFont::show()
{
	_direction ? showV() : showH();
}

void DotFont::showH()
{
	for (byte c = 0; c < _unitWidth; c++)
	{
		for (byte r = 0; r < _unitHeight; r++)
		{
			if (boolean b = bitRead(pgm_read_byte_near(_pattern + _unitWidth * _index + c), r))
				_pDM->setDot(_col + c, _row + r, b);
		}
	}
}

void DotFont::showV()
{
	for (byte c = 0; c < _unitWidth; c++)
	{
		for (byte r = 0; r < _unitHeight; r++)
		{
			if (boolean b = bitRead(pgm_read_byte_near(_pattern + _unitWidth * _index + c), r))
				_pDM->setDot(_col + r, _row + _unitWidth - c - 1, b);
		}
	}
}

byte DotFont::calcFontRealWidth()
{
	byte i=_unitWidth-1;

	while(pgm_read_byte_near(_pattern + _unitWidth * _index + i)==0 && i) i--;
	return i+1;
}

