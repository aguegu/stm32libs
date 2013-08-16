/*
 DotMatrixDriver.cpp
 DotMatrixDriver Class for receiving output from DotMatrix, with which to control the display
 library for Arduino to drive LED Dot Matrix Display dirven by 74HC595(column) and 74HC138(row)

 Created on: 2012-1-15
 Updated on: 2012-2-26
 Author: Weihong Guan
 Blog: http://aguegu.net
 E-mail: weihong.guan@gmail.com
 Code license: Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
 http://creativecommons.org/licenses/by-nc-sa/3.0/

 source hosting: http://code.google.com/p/aguegu-arduino-library/
 */

#include "DotMatrixDriver.h"

DotMatrixDriver::DotMatrixDriver(DotMatrix * pDM, uint8_t pin_C_IN,
		uint8_t pin_C_OE, uint8_t pin_C_ST, uint8_t pin_C_SH, uint8_t pin_R_OE,
		uint8_t pin_R_A2, uint8_t pin_R_A1, uint8_t pin_R_A0, uint16_t speed)
{
	_pDM = pDM;

	_pin_595_ST = pin_C_ST;
	_pin_595_SH = pin_C_SH;
	_pin_595_DS = pin_C_IN;
	_pin_595_OE = pin_C_OE;

	_pin_138_OE = pin_R_OE;
	_pin_138_A0 = pin_R_A0;
	_pin_138_A1 = pin_R_A1;
	_pin_138_A2 = pin_R_A2;

	pinMode(_pin_138_A1, OUTPUT);
	pinMode(_pin_138_A2, OUTPUT);
	pinMode(_pin_595_ST, OUTPUT);
	pinMode(_pin_595_SH, OUTPUT);
	pinMode(_pin_595_DS, OUTPUT);
	pinMode(_pin_595_OE, OUTPUT);

	pinMode(_pin_138_OE, OUTPUT);
	pinMode(_pin_138_A0, OUTPUT);

	this->setSpeed(speed);

	this->setSize(_pDM->countBytes(), _pDM->countRow());
}

void DotMatrixDriver::setSpeed(uint16_t speed)
{
	_speed = speed;
}

void DotMatrixDriver::setSize(byte length, byte rowCount)
{
	_length = length;
	_rowCount = rowCount;
	_bytesPerRow = _length / _rowCount;
}

void DotMatrixDriver::setRow(byte r)
{
	digitalWrite(_pin_138_A0, r & 0x01);
	digitalWrite(_pin_138_A1, r & 0x02);
	digitalWrite(_pin_138_A2, r & 0x04);
}

void DotMatrixDriver::setCol(byte * p)
{
	for (byte i = 0; i < _bytesPerRow; i++)
		shiftOut(_pin_595_DS, _pin_595_SH, LSBFIRST, ~*(p++));
}

void DotMatrixDriver::display(byte times)
{
	byte* p = _pDM->output();

	while (times--)
	{
		for (byte r = 0; r < _rowCount; r++)
		{
			setCol(p + _bytesPerRow * r);
			digitalWrite(_pin_138_OE, LOW);

			digitalWrite(_pin_595_ST, LOW);
			digitalWrite(_pin_595_ST, HIGH);

			setRow(r);
			digitalWrite(_pin_138_OE, HIGH);
			delayMicroseconds(_speed);
		}
	}
}

