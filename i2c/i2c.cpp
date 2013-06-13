#include <stm32f10x.h>
#include <stm32f10x_i2c.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>

#include "i2c/i2c.h"

#define within(time, fun) for (t = (time); (fun) && --t;)

I2c::I2c(I2C_TypeDef * i2c, uint32_t rcc_apb1periph_i2cx, uint16_t flat_timeout,
		uint16_t long_timeout) :
		_i2c(i2c), _FLAG_TIMEOUT(flat_timeout), _LONG_TIMEOUT(long_timeout) {
	RCC_APB1PeriphClockCmd(rcc_apb1periph_i2cx, ENABLE);
}

I2c::~I2c() {

}

void I2c::init(uint16_t mode, uint32_t clock_speed, uint16_t ack,
		uint16_t acknowledged_address, uint16_t duty_cycle) {

	I2C_DeInit(_i2c);

	I2C_InitTypeDef I2C_InitStructure;
	I2C_StructInit(&I2C_InitStructure);
	I2C_InitStructure.I2C_Mode = mode;
	I2C_InitStructure.I2C_DutyCycle = duty_cycle;
	I2C_InitStructure.I2C_OwnAddress1 = acknowledged_address;
	I2C_InitStructure.I2C_Ack = ack;
	I2C_InitStructure.I2C_ClockSpeed = clock_speed;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

	I2C_Init(_i2c, &I2C_InitStructure);
	I2C_Cmd(_i2c, ENABLE);
}

uint8_t I2c::write(uint8_t slave_address, const uint8_t* buf, uint32_t length) {
	vu32 t = 0;

	if (!length) return 0;

	within(_LONG_TIMEOUT, I2C_GetFlagStatus(_i2c, I2C_FLAG_BUSY ));
	if (!t) return 0;

	I2C_GenerateSTART(_i2c, ENABLE);
	within(_FLAG_TIMEOUT, !I2C_CheckEvent(_i2c, I2C_EVENT_MASTER_MODE_SELECT ));
	if (!t) return 1;

	I2C_Send7bitAddress(_i2c, slave_address, I2C_Direction_Transmitter );
	within(_FLAG_TIMEOUT,
			!I2C_CheckEvent(_i2c, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ));
	if (!t) return 2;

	do {
		I2C_SendData(_i2c, *buf++);
		within(_FLAG_TIMEOUT, !I2C_GetFlagStatus(_i2c, I2C_FLAG_BTF ));
		if (!t) return 3;
	} while (--length);

	I2C_GenerateSTOP(_i2c, ENABLE);
	within(_LONG_TIMEOUT, I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF ));
	if (!t) return 4;

	return 5;
}

uint8_t I2c::read(uint8_t slave_address, uint8_t* data, uint32_t length) {
	vu32 t = 0;
	if (!length) return 0;

	within(_LONG_TIMEOUT, I2C_GetFlagStatus(_i2c, I2C_FLAG_BUSY ));
	if (!t) return 0;

	I2C_AcknowledgeConfig(_i2c, ENABLE);
	I2C_NACKPositionConfig(_i2c, I2C_NACKPosition_Current );
	I2C_GenerateSTART(_i2c, ENABLE);

	within(_FLAG_TIMEOUT, !I2C_CheckEvent(_i2c, I2C_EVENT_MASTER_MODE_SELECT ));
	if (!t) return 1;

	I2C_Send7bitAddress(_i2c, slave_address, I2C_Direction_Receiver );
	within(_FLAG_TIMEOUT, !I2C_GetFlagStatus(_i2c, I2C_FLAG_ADDR ));
	if (!t) return 2;

	I2C_AcknowledgeConfig(_i2c, DISABLE);

	__disable_irq();
	(void) _i2c->SR2;
	I2C_GenerateSTOP(_i2c, ENABLE);
	__enable_irq();

	within(_FLAG_TIMEOUT, !I2C_GetFlagStatus(_i2c, I2C_FLAG_RXNE ));
	if (!t) return 3;

	*data = I2C_ReceiveData(_i2c);

	within(_FLAG_TIMEOUT, I2C_GetFlagStatus(_i2c, I2C_FLAG_STOPF ));
	if (!t) return 4;

	return 5;
}
