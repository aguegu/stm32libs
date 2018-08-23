/*
 * spi.h
 *
 *  Created on: Jun 4, 2013
 *      Author: agu
 */

#ifndef SPI_H_
#define SPI_H_

#include <cstddef>
#include "stm32f10x_spi.h"

class Spi {
public:
	Spi(SPI_TypeDef * spi);
	void init(uint16_t direction = SPI_Direction_2Lines_FullDuplex,
			uint16_t mode = SPI_Mode_Master,
			uint16_t data_size = SPI_DataSize_8b, uint16_t cpol = SPI_CPOL_Low,
			uint16_t cpha = SPI_CPHA_1Edge, uint16_t _nss = SPI_NSS_Soft,
			uint16_t baudrate_prescaler = SPI_BaudRatePrescaler_8,
			uint16_t first_bit = SPI_FirstBit_MSB, uint16_t crc_polynomial = 7);
	~Spi();

	void write(const u8 * out, u16 length, u8 * in = NULL);
	void write(const u16 * out, u16 length, u16 * in = NULL);

	void readWithNop(u8 * in, u16 length);
	void readWithNop(u16 * in, u16 length);

	void configDataSize(u16);

	u16 write(u16);
	SPI_TypeDef * base();
private:
	SPI_TypeDef * const _spi;
};

#endif /* SPI_H_ */
