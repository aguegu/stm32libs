#ifndef I2C_H
#define I2C_H

#include "stm32f10x_i2c.h"
#include <cstdlib>
#include <cstring>

class I2c {
public:
	I2c(I2C_TypeDef * i2c, uint16_t flat_timeout =
			0x100, uint16_t long_timeout = 0x400);
	~I2c();
	void init(uint16_t mode = I2C_Mode_I2C, uint32_t clock_speed = 100000,
			uint16_t ack = I2C_Ack_Enable, uint16_t acknowledged_address = 0,
			uint16_t duty_cycle = I2C_DutyCycle_2);

	uint8_t write(uint8_t slave_address, const uint8_t* buf, uint32_t length,
			FunctionalState sendstop = ENABLE);
	uint8_t read(uint8_t slave_address, uint8_t* buf, uint32_t length,
			FunctionalState checkbusy = ENABLE);
	uint8_t setReg(uint8_t slave_address, uint8_t reg_address,
			const uint8_t* buf, uint32_t length);
	uint8_t setReg(uint8_t slave_address, uint8_t reg_address,
			const uint8_t value);
	uint8_t getReg(uint8_t slave_address, uint8_t reg_address, uint8_t* buf,
			uint32_t length);

private:
	I2C_TypeDef * const _i2c;
	const uint16_t _FLAG_TIMEOUT;
	const uint16_t _LONG_TIMEOUT;
};

#endif
