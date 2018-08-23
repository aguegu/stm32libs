/*
 * at24c.h
 *
 *  Created on: Aug 28, 2013
 *      Author: agu
 */

#ifndef AT24C_H_
#define AT24C_H_

#include "i2c/i2c.h"
#include "stm32-template.h"

class At24c {
public:
	At24c(I2c & i2c, u8 pagesize, u8 slave_address = 0x50);
	virtual ~At24c();

	u8 write_raw(u16 reg_address, const void * buff, u16 length);
	u8 write(u16 reg_address, const void * buff, u16 length);
	u8 read(u16 reg_address, void * buff, u16 length = 1);

private:
	I2c & _i2c;
	const u8 _pagesize;
	const u8 _slave_address;
};

#endif /* AT24C_H_ */
