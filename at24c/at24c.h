/*
 * at24c.h
 *
 *  Created on: Aug 28, 2013
 *      Author: agu
 */

#ifndef AT24C_H_
#define AT24C_H_

#include "i2c/i2c.h"

class At24c {
public:
	At24c(I2c & i2c, uint8_t pagesize, uint8_t slave_address = 0x50);
	virtual ~At24c();

	uint8_t write(uint16_t reg_address, const uint8_t * buff, uint16_t length);
	uint8_t write(uint16_t reg_address, uint8_t data);

	uint8_t read(uint16_t reg_address, uint8_t * buff, uint16_t length = 1);

private:
	I2c & _i2c;
	const uint8_t _pagesize;	// in bytes
	const uint8_t _slave_address;

};

#endif /* AT24C_H_ */
