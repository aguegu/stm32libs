/*
 * st7735r.h
 *
 *  Created on: Jun 4, 2013
 *      Author: agu
 */

#ifndef ST7735R_H_
#define ST7735R_H_

#include "stm32-template.h"
#include "spi/spi.h"
#include "gpio/gpio.h"

class St7735r {
public:
	St7735r(SPI_TypeDef * spi, uint32_t spi_rcc_apb2periph_spi,
			void (*rcc_apbx_periph_clock_cmd)(uint32_t, FunctionalState), GPIO_TypeDef *ss_port,
			uint16_t ss_pin, uint32_t ss_rcc_apb2_periph,
			GPIO_TypeDef *reset_port, uint16_t reset_pin,
			uint32_t reset_rcc_apb2_periph, GPIO_TypeDef *rs_port,
			uint16_t rs_pin, uint32_t rs_rcc_apb2_periph);
	virtual ~St7735r();
	void init();
	void write8(uint8_t is_data, const uint8_t * data, uint8_t length);
	void write16(uint8_t is_data, const uint16_t * data, uint8_t length);
	void command(uint8_t cmd);
	void setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
	void pushColor(uint16_t color);
private:
	Spi _spi;
	Gpio _pin_ss;
	Gpio _pin_reset;
	Gpio _pin_rs;
};

#endif /* ST7735R_H_ */
