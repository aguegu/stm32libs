/*
 * ds3231.h
 *
 *  Created on: Aug 15, 2013
 *      Author: agu
 */

#ifndef DS3231_H_
#define DS3231_H_

#include "i2c/i2c.h"
#include "common/bit_op.h"
#include <cstdlib>
#include <cstring>

#define SECOND	0
#define MINUTE	1
#define HOUR 	2
#define DATE	4
#define MONTH	5
#define WEEKDAY	3
#define YEAR	6

class Ds3231 {

public:
	Ds3231(I2c & i2c);
	virtual ~Ds3231();
	void init();

	void setTime(uint8_t hour, uint8_t minute, uint8_t second);
	void setDate(uint8_t year, uint8_t month, uint8_t date, uint8_t weekday);

	uint8_t getHexData(uint8_t index);
	uint8_t getDecData(uint8_t index);

	static uint8_t hex2dec(uint8_t hex);
	static uint8_t dec2hex(uint8_t dec);

	void refresh(bool only_time = false);
	float getTemperature();

private:
	static const uint8_t _address = 0x68;
	I2c & _i2c;
	uint8_t setReg(uint8_t reg, uint8_t data);
	uint8_t getReg(uint8_t reg);
	uint8_t *_datetime;
};
#endif /* DS3231_H_ */
