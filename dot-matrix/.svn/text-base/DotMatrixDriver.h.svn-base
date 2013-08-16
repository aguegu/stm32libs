/*
	DotMatrixDriver.h
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

#ifndef DOT_MATRIX_DRIVER_H_
#define DOT_MATRIX_DRIVER_H_

#include "Arduino.h"
#include "DotMatrix.h"

class DotMatrixDriver
{
public:

	DotMatrixDriver(DotMatrix * pDM, uint8_t pin_C_IN, uint8_t pin_C_OE, uint8_t pin_C_ST, uint8_t pin_C_SH, uint8_t pin_R_OE, uint8_t pin_R_A2, uint8_t pin_R_A1, uint8_t pin_R_A0, uint16_t speed = 0x400);

	void display(byte times = 1);
	void setSpeed(uint16_t speed = 0x400);

private:

	DotMatrix * _pDM;

	byte _length;
	byte _rowCount;
	byte _bytesPerRow;

	uint16_t _speed;

	uint8_t _pin_595_ST;
	uint8_t _pin_595_SH;
	uint8_t _pin_595_DS;
	uint8_t _pin_595_OE;

	uint8_t _pin_138_OE;
	uint8_t _pin_138_A0;
	uint8_t _pin_138_A1;
	uint8_t _pin_138_A2;

	void setRow(byte r);
	void setCol(byte *p);

	void setSize(byte length, byte rowCount);
};

#endif /* DOTMATRIXDRIVER_H_ */
