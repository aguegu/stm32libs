/*
 * spi.cpp
 *
 *  Created on: Jun 4, 2013
 *      Author: agu
 */

#include "spi.h"

Spi::Spi(SPI_TypeDef * spi) :
		_spi(spi) {

}

Spi::~Spi() {

}

void Spi::init(uint16_t direction, uint16_t mode, uint16_t data_size,
		uint16_t cpol, uint16_t cpha, uint16_t nss, uint16_t baudrate_prescaler,
		uint16_t first_bit, uint16_t crc_polynomial) {
	SPI_InitTypeDef spi_init_type;
	spi_init_type.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	spi_init_type.SPI_Mode = SPI_Mode_Slave;
	spi_init_type.SPI_DataSize = SPI_DataSize_8b;
	spi_init_type.SPI_CPOL = SPI_CPOL_Low;
	spi_init_type.SPI_CPHA = SPI_CPHA_1Edge;
	spi_init_type.SPI_NSS = SPI_NSS_Hard;
	spi_init_type.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	spi_init_type.SPI_FirstBit = SPI_FirstBit_MSB;
	spi_init_type.SPI_CRCPolynomial = 7;

	SPI_Init(_spi, &spi_init_type);
	SPI_Cmd(_spi, ENABLE);
}

void Spi::send(const uint8_t * out, uint8_t length, uint8_t * in) {

	while (length--) {
		while (!SPI_I2S_GetFlagStatus(_spi, SPI_I2S_FLAG_TXE ))
			;
		SPI_I2S_SendData(_spi, *out++);
		if (in) {
			while (!SPI_I2S_GetFlagStatus(_spi, SPI_I2S_FLAG_RXNE ))
				;
			*in++ = SPI_I2S_ReceiveData(_spi);
		}
	}
}

