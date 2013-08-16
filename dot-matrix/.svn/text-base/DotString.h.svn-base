/*
	DotString.h
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

#ifndef DOTSTRING_H_
#define DOTSTRING_H_


#include "DotMatrix.h"
#include "DotFont.h"

class DotString
{
public:
	DotString(DotMatrix * pDM);
	virtual ~DotString();

	void printString(char s[]);
private:
	DotMatrix * _pDM;
	DotFont _df;
};

#endif /* DOTSTRING_H_ */
