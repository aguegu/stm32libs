/*
 DotChar.h
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

#ifndef DOT_CHAR_H_
#define DOT_CHAR_H_

#include "dot-matrix.h"
#include "dot-font.h"

class DotChar {
public:
	DotChar(DotMatrix & dm, const DotFont & df);
	virtual ~DotChar();

	void setChar(const char chr);
	void setVertical(const bool vertical = true);

	void postAt(const uint8_t col, const uint8_t row);

	void clear(const bool on = false) const;

	uint8_t calcFontRealWidth() const;
	uint8_t getWidth() const ;
	uint8_t getHeight() const ;

	bool getVertical() const ;

	uint8_t getMaxCol() const ;

	uint8_t getMaxRow() const ;

private:
	DotMatrix & _dm;
	const DotFont & _df;

	uint8_t _col;
	uint8_t _row;
	uint8_t _index;
	uint16_t _indent;
	bool _vertical;

	void print() const ;
	void printH() const ;
	void printV() const;
};

#endif /* DOT_CHAR_H_ */
