/*
 * spi.cpp
 *
 *  Created on: Jun 4, 2013
 *      Author: agu
 */

#include "spi.h"

Spi::Spi(SPI_TypeDef * spi, uint32_t rcc_apbx_periph_spi,
		void (*rcc_apbx_periph_clock_cmd)(uint32_t, FunctionalState)) :
		_spi(spi) {
	(*rcc_apbx_periph_clock_cmd)(rcc_apbx_periph_spi, ENABLE);
}

Spi::~Spi() {

}

void Spi::init(uint16_t direction, uint16_t mode, uint16_t data_size,
		uint16_t cpol, uint16_t cpha, uint16_t nss, uint16_t baudrate_prescaler,
		uint16_t first_bit, uint16_t crc_polynomial) {

	SPI_InitTypeDef spi_init_type;

	spi_init_type.SPI_Direction = direction;
	spi_init_type.SPI_Mode = mode;
	spi_init_type.SPI_DataSize = data_size;
	spi_init_type.SPI_CPOL = cpol;
	spi_init_type.SPI_CPHA = cpha;
	spi_init_type.SPI_NSS = nss;
	spi_init_type.SPI_BaudRatePrescaler = baudrate_prescaler;
	spi_init_type.SPI_FirstBit = first_bit;
	spi_init_type.SPI_CRCPolynomial = crc_polynomial;

	SPI_Init(_spi, &spi_init_type);
	SPI_Cmd(_spi, ENABLE);
}

void Spi::write8(const uint8_t * out, uint8_t length, uint8_t * in) {
	SPI_DataSizeConfig(_spi, SPI_DataSize_8b );
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

void Spi::write16(const uint16_t * out, uint8_t length, uint16_t * in) {
	SPI_DataSizeConfig(_spi, SPI_DataSize_16b );
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
