/*
 * spi.cpp
 *
 *  Created on: Jun 4, 2013
 *      Author: agu
 */

#include "spi.h"

Spi::Spi(SPI_TypeDef * spi) :
		_spi(spi) {
	switch ((u32) _spi) {
	case (u32) SPI1_BASE:
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
		break;
	case (u32) SPI2_BASE:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
		break;
	case (u32) SPI3_BASE:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
		break;
	}
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

void Spi::configDataSize(uint16_t SPI_DataSize) {
	SPI_DataSizeConfig(_spi, SPI_DataSize);
}

void Spi::write(const u8 * out, u16 length, u8 * in) {
	while (length--) {
		u8 val = write(*out++);
		if (in)
			*in++ = val;
	}
}

void Spi::write(const u16 * out, u16 length, u16 * in) {
	while (length--) {
		u16 val = write(*out++);
		if (in)
			*in++ = val;
	}
}

u16 Spi::write(u16 val) {
	while (!SPI_I2S_GetFlagStatus(_spi, SPI_I2S_FLAG_TXE))
		;
	SPI_I2S_SendData(_spi, val);

	while (!SPI_I2S_GetFlagStatus(_spi, SPI_I2S_FLAG_RXNE))
		;
	return SPI_I2S_ReceiveData(_spi);
}

void Spi::readWithNop(u8 * in, u16 length) {
	while (length--) {
		*in++ = write(0xff);
	}
}

void Spi::readWithNop(u16 * in, u16 length) {
	while (length--) {
		*in++ = write(0xff);
	}
}


SPI_TypeDef * Spi::base() {
	return _spi;
}
