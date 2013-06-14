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
	I2cSoft(Gpio &sda, Gpio &scl, uint16_t flat_timeout = 0x100);
	~I2cSoft();
	void init();

	void start();
	void stop();

	uint8_t write(uint8_t address, uint8_t *data, uint8_t length,
			uint8_t send_stop = true);

	void read(uint8_t address, uint8_t *data, uint8_t length,
			uint8_t send_stop = true);

	u16 waitAck();
	void sendAck(uint8_t ack);

	void transmit(uint8_t data);
	uint8_t receive();
private:
	Gpio & _sda;
	Gpio & _scl;

	const uint16_t _FLAG_TIMEOUT;
};

#endif /* I2C_SOFT_H_ */
