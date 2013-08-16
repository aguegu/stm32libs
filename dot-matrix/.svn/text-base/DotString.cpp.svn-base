/*
	DotString.cpp
	DotString Class is an array of DotFont0704 and provides a display method to DotMatrix
	library for Arduino to drive LED Dot Matrix Display dirven by 74HC595(column) and 74HC138(row)

	Created on: 2012-1-25
	Updated on: 2012-2-26
	Author: Weihong Guan
	Blog: http://aguegu.net
	E-mail: weihong.guan@gmail.com
	Code license: Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
	http://creativecommons.org/licenses/by-nc-sa/3.0/

	source hosting: http://code.google.com/p/aguegu-arduino-library/
 */
#include "DotString.h"
#include "Font0704.h"

DotString::DotString(DotMatrix * pDM)
{
	// TODO Auto-generated constructor stub
	_pDM = pDM;
	_df.setDotMatrix(_pDM);
	_df.setPattern(FONT_0704, FONT_0704_WIDTH, FONT_0704_HEIGHT);
	_df.setDirection(false);
}

DotString::~DotString()
{
	// TODO Auto-generated destructor stub
}

void DotString::printString(char s[])
{
	char *p = s;
	byte cursor = 0;

	_pDM->clear();

	while (*p && cursor < _pDM->countCol())
	{
		if (*p >= 0x20 && *p < 0x80)
		{
			_df.setIndex(*p - 0x20);
			_df.moveTo(cursor, 0);
			_df.show();
		}
		p++;
		cursor += _df.calcFontRealWidth() + 1;
	}
}
