/*
 * i2c-soft.h
 *
 *  Created on: Jun 9, 2013
 *      Author: agu
 */

#ifndef I2C_SOFT_H_
#define I2C_SOFT_H_

#include "gpio/gpio.h"

class I2cSoft {
public:
	I2cSoft(Gpio &sda, Gpio &scl);
	~I2cSoft();
	void init();

	void start();
	void stop();

//	void readFrom(uint8_t address, uint8_t *data, uint8_t length,
//			uint8_t send_stop);

	vu32 waitAck();
	void send(uint8_t data);
private:
	Gpio & _sda;
	Gpio & _scl;
	void delayus(vu16 t);

};

#endif /* I2C_SOFT_H_ */
