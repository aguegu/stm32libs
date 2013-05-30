#ifndef I2C_H
#define I2C_H

#include <stm32f10x_i2c.h>

typedef enum {
	Error = 0, Success = !Error
} Status;

Status I2C_Read(I2C_TypeDef* I2Cx, uint8_t* buf, uint32_t nbuf,
		uint8_t SlaveAddress);
Status I2C_Write(I2C_TypeDef* I2Cx, const uint8_t* buf, uint32_t nbuf,
		uint8_t SlaveAddress);
void I2C_LowLevel_Init(int ClockSpeed, int OwnAddress);

class I2c {
public:
	I2c(I2C_TypeDef * i2c, uint32_t rcc_apb1periph_i2cx, uint16_t flat_timeout =
			0x1000, uint16_t long_timeout = 0xf000);
	~I2c();
	void init(uint16_t mode = I2C_Mode_I2C, uint32_t clock_speed = 100000,
			uint16_t ack = I2C_Ack_Enable, uint16_t acknowledged_address = 0,
			uint16_t duty_cycle = I2C_DutyCycle_2 );

	uint8_t write(uint8_t slave_address, const uint8_t* buf, uint32_t length);
	uint8_t read(uint8_t slave_address, uint8_t* buf, uint32_t length);
private:
	I2C_TypeDef * const _i2c;
	const uint16_t _FLAG_TIMEOUT;
	const uint16_t _LONG_TIMEOUT;
};

#endif
