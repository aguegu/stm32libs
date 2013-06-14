/*
 * isl1208-i2csoft.h
 *
 *  Created on: Jun 14, 2013
 *      Author: agu
 */

#ifndef ISL1208_I2CSOFT_H_
#define ISL1208_I2CSOFT_H_

#include "i2c-soft/i2c-soft.h"

class Isl1208I2cSoft: public I2cSoft {
public:
	Isl1208I2cSoft(Gpio &sda, Gpio &scl);
	~Isl1208I2cSoft();
	void init();
	void refresh();
	uint8_t getVal(uint8_t index);
private:
	static const uint8_t _ADDRESS = 0x6f;
	uint8_t _data[7];
};

#endif /* ISL1208_I2CSOFT_H_ */
