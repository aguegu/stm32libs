/*
 * spi.h
 *
 *  Created on: Jun 4, 2013
 *      Author: agu
 */

#ifndef SPI_H_
#define SPI_H_

#include "stm32f10x_spi.h"

class Spi {
public:
	Spi(SPI_TypeDef * spi, uint32_t rcc_apbx_periph_spi,
			void (*rcc_apbx_periph_clock_cmd)(uint32_t, FunctionalState));
	void init(uint16_t direction = SPI_Direction_2Lines_FullDuplex,
			uint16_t mode = SPI_Mode_Slave,
			uint16_t data_size = SPI_DataSize_8b, uint16_t cpol = SPI_CPOL_Low,
			uint16_t cpha = SPI_CPHA_1Edge, uint16_t _nss = SPI_NSS_Hard,
			uint16_t baudrate_prescaler = SPI_BaudRatePrescaler_2,
			uint16_t first_bit = SPI_FirstBit_MSB, uint16_t crc_polynomial = 7);
	~Spi();

	void write8(const uint8_t * out, uint16_t length, uint8_t * in = 0);
	void write16(const uint16_t * out, uint16_t length, uint16_t * in = 0);
private:
	SPI_TypeDef * const _spi;
};

#endif /* SPI_H_ */
