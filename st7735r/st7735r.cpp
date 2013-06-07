/*
 * st7735r.cpp
 *
 *  Created on: Jun 4, 2013
 *      Author: agu
 */

#include "st7735r.h"

#define ST7735_CASET 0x2A
#define ST7735_RASET 0x2B
#define ST7735_MADCTL 0x36
#define ST7735_RAMWR 0x2C
#define ST7735_RAMRD 0x2E
#define ST7735_COLMOD 0x3A

#define MADCTLGRAPHICS 0x6
#define MADCTLBMP      0x2

#define MADVAL(x) (((x) << 5) | 8)
//static uint8_t madctlcurrent = MADVAL(MADCTLGRAPHICS);

typedef struct {
	uint8_t command;   // ST7735 command byte
	uint8_t delay;     // ms delay after
	uint8_t len;       // length of parameter data
	uint8_t data[16];  // parameter data
} ST7735_cmdBuf;

static const ST7735_cmdBuf initializers[] = {
		// SWRESET Software reset
		{ 0x01, 150, 0, { 0 } },
		// SLPOUT Leave sleep mode
		{ 0x11, 150, 0, { 0 } },
		// FRMCTR1, FRMCTR2 Frame Rate configuration -- Normal mode, idle
		// frame rate = fosc / (1 x 2 + 40) * (LINE + 2C + 2D)
		{ 0xB1, 0, 3, { 0x01, 0x2C, 0x2D } },
		{ 0xB2, 0, 3, { 0x01, 0x2C, 0x2D } },
		// FRMCTR3 Frame Rate configureation -- partial mode
		{ 0xB3, 0, 6, { 0x01, 0x2C, 0x2D, 0x01, 0x2C, 0x2D } },
		// INVCTR Display inversion (no inversion)
		{ 0xB4, 0, 1, { 0x07 } },
		// PWCTR1 Power control -4.6V, Auto mode
		{ 0xC0, 0, 3, { 0xA2, 0x02, 0x84 } },
		// PWCTR2 Power control VGH25 2.4C, VGSEL -10, VGH = 3 * AVDD
		{ 0xC1, 0, 1, { 0xC5 } },
		// PWCTR3 Power control, opamp current smal, boost frequency
		{ 0xC2, 0, 2, { 0x0A, 0x00 } },
		// PWCTR4 Power control, BLK/2, opamp current small and medium low
		{ 0xC3, 0, 2, { 0x8A, 0x2A } },
		// PWRCTR5, VMCTR1 Power control
		{ 0xC4, 0, 2, { 0x8A, 0xEE } }, { 0xC5, 0, 1, { 0x0E } },
		// INVOFF Don't invert display
		{ 0x20, 0, 0, { 0 } },
		// Memory access directions. row address/col address, bottom to top refesh (10.1.27)
		{ ST7735_MADCTL, 0, 1, { MADVAL(MADCTLGRAPHICS) } },
		// Color mode 16 bit (10.1.30
		{ ST7735_COLMOD, 0, 1, { 0x05 } },
//		// Column address set 0..127
//		{ ST7735_CASET, 0, 4, { 0x00, 0x00, 0x00, 0x7F } },
//		// Row address set 0..127
//		{ ST7735_RASET, 0, 4, { 0x00, 0x00, 0x00, 0x7F } },
		// GMCTRP1 Gamma correction
		{ 0xE0, 0, 16, { 0x02, 0x1C, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2D, 0x29,
				0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10 } },
		// GMCTRP2 Gamma Polarity corrction
		{ 0xE1, 0, 16, { 0x03, 0x1d, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D, 0x2E,
				0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10 } },
		// DISPON Display on
		{ 0x29, 100, 0, { 0 } },
		// NORON Normal on
		{ 0x13, 10, 0, { 0 } },
		// End
		{ 0, 0, 0, { 0 } } };

St7735r::St7735r(Spi & spi, Dma & dma, Gpio &pin_ss, Gpio &pin_reset,
		Gpio &pin_rs, uint16_t offset_x, uint16_t offset_y) :
		_spi(spi), _dma(dma), _pin_ss(pin_ss), _pin_reset(pin_reset), _pin_rs(
				pin_rs), _offset_x(offset_x), _offset_y(offset_y) {
}

St7735r::~St7735r() {

}

void St7735r::init() {

	_pin_ss.init(GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
	_pin_reset.init(GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
	_pin_rs.init(GPIO_Mode_Out_PP, GPIO_Speed_50MHz);

	_spi.init(SPI_Direction_2Lines_FullDuplex, SPI_Mode_Master, SPI_DataSize_8b,
			SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_NSS_Soft, SPI_BaudRatePrescaler_2,
			SPI_FirstBit_MSB, 7);

	SPI_I2S_DMACmd(_spi.base(), SPI_I2S_DMAReq_Tx, ENABLE);

	_pin_ss.set(Bit_SET);
	_pin_reset.set(Bit_SET);
	delay(10);
	_pin_reset.set(Bit_RESET);
	delay(10);
	_pin_reset.set(Bit_SET);
	delay(10);

	for (const ST7735_cmdBuf * cmd = initializers; cmd->command; cmd++) {
		this->command(cmd->command);
		if (cmd->len) write8(1, cmd->data, cmd->len);
		if (cmd->delay) delay(cmd->delay);
	}

	_dma.setPeriph((uint32_t) &(_spi.base()->DR),
			DMA_PeripheralDataSize_HalfWord, DMA_PeripheralInc_Disable );
	_dma.setFunction();
}

void St7735r::write8(uint8_t is_data, const uint8_t * data, uint16_t length) {
	_pin_rs.set(is_data ? Bit_SET : Bit_RESET);
	_pin_ss.set(Bit_RESET);
	_spi.write8(data, length);
	_pin_ss.set(Bit_SET);
}

void St7735r::write16(uint8_t is_data, const uint16_t * data, uint16_t length) {

	SPI_DataSizeConfig(_spi.base(), SPI_DataSize_16b );

	_pin_rs.set(is_data ? Bit_SET : Bit_RESET);
	_pin_ss.set(Bit_RESET);
//	_spi.write16(data, length);

	_dma.setMemory((uint32_t) &data, DMA_MemoryDataSize_HalfWord,
			DMA_MemoryInc_Enable );
	_dma.setTransfer(length, DMA_DIR_PeripheralDST);
	_dma.run();

	_pin_ss.set(Bit_SET);
}

void St7735r::write16(uint8_t is_data, const uint16_t data, uint16_t length) {

	SPI_DataSizeConfig(_spi.base(), SPI_DataSize_16b );

	_pin_rs.set(is_data ? Bit_SET : Bit_RESET);
	_pin_ss.set(Bit_RESET);
//	while (length--)
//		_spi.write16(&data, 1);

	_dma.setMemory((uint32_t) &data, DMA_MemoryDataSize_HalfWord,
			DMA_MemoryInc_Disable );
	_dma.setTransfer(length, DMA_DIR_PeripheralDST);
	_dma.run();

	_pin_ss.set(Bit_SET);
}

void St7735r::command(uint8_t cmd) {
	this->write8(0, &cmd, 1);
}

void St7735r::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1,
		uint16_t y1) {

	command(ST7735_CASET);
	this->write16(1, x0 + _offset_x);
	this->write16(1, x1 + _offset_x);

	command(ST7735_RASET);
	this->write16(1, y0 + _offset_y);
	this->write16(1, y1 + _offset_y);

	command(ST7735_RAMWR);
}
