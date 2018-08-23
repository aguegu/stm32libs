/*
 * nrf24l01.cpp
 *
 *  Created on: Sep 21, 2016
 *      Author: agu
 */

#include "nrf24l01p.h"

Nrf24l01p::Nrf24l01p(Spi & spi, Gpio & sck, Gpio & miso, Gpio & mosi,
		Gpio & irq, Gpio & csn, Gpio & ce) :
		_spi(spi), _sck(sck), _miso(miso), _mosi(mosi), _irq(
				irq), _csn(csn), _ce(ce) {

}

void Nrf24l01p::init(u8 channel) {
	u8 status;

	_sck.init(GPIO_Mode_AF_PP, GPIO_Speed_10MHz);
	_mosi.init(GPIO_Mode_AF_PP, GPIO_Speed_10MHz);
	_miso.init(GPIO_Mode_IPU, GPIO_Speed_10MHz);

	_irq.init(GPIO_Mode_IPU, GPIO_Speed_2MHz);
	_csn.init(GPIO_Mode_Out_PP, GPIO_Speed_2MHz);
	_ce.init(GPIO_Mode_Out_PP, GPIO_Speed_2MHz);
	_spi.init();

//	_spi.configDataSize(SPI_DataSize_8b);

	_ce.set(false);
	writeReg(CONFIG, 0x08); // Power Down
	command(FLUSH_TX);
	command(FLUSH_RX);
	writeReg(RF_CH, channel);
	writeReg(RF_SETUP, 0x0f);
	writeReg(SETUP_AW, 0x03);

	writeReg(FEATURE, 0x04); // dynamic payload

	readReg(RF_SETUP, &status, 1);
	printf("%02x\r\n", status);
}

void Nrf24l01p::writeReg(u8 addr, u8*data, u8 length) {
	_csn.set(false);
	_spi.write(0x20 + addr);
	_spi.write(data, length);
	_csn.set(true);
}

void Nrf24l01p::writeReg(u8 addr, u8 data) {
	uint8_t status;
	_csn.set(false);
	_spi.write(0x20 + addr);
	status = _spi.write(data);
	_csn.set(true);

	printf("write_register(%02x,%02x) %02x\r\n", addr, data, status);
}

void Nrf24l01p::readReg(u8 addr, u8*data, u8 length) {
	_csn.set(false);
	_spi.write(addr);
	_spi.readWithNop(data, length);
	_csn.set(true);
}

void Nrf24l01p::setupTx() {
	_ce.set(false);
	writeReg(CONFIG, 0x0e); // power on and ptx
	delay(2);
}

void Nrf24l01p::setupRx() {
	_ce.set(false);
	writeReg(CONFIG, 0x0f); // power on and prx
	_ce.set(true);
}

u8 Nrf24l01p::send(u8 *data, u8 length) {
	_csn.set(false);
	_spi.write(W_TX_PLOAD);
	_spi.write(data, length);
	_csn.set(true);

	_ce.set(true);
	delayMicroseconds(10);
	_ce.set(false);

	while (_irq.getInput())
		;

	u8 status;
	readReg(STATUS, &status, 1);
	writeReg(STATUS, status);

 	if (status & MAX_TX) {
 		command(FLUSH_TX);
	}

	setupRx();

	return status;
}

u8 Nrf24l01p::sendFromSlave(u8 *data, u8 length) {
	setupTx();
	u8 r = send(data, length);
	setupRx();
	return r;
}

u8 Nrf24l01p::recv(u8 *data) {
	if (_irq.getInput() == false) {
		u8 status;
		readReg(STATUS, &status, 1);
		writeReg(STATUS, status);

		if (status & RX_OK) {
			_csn.set(false);
			_spi.write(R_RX_PL_WID);
			u8 len;
			_spi.readWithNop(&len, 1);
			_csn.set(true);

			if (len > 32) {
				return 0;
			}

			_csn.set(false);
			_spi.write(R_RX_PLOAD);
			_spi.readWithNop(data, len);
			_csn.set(true);
			command(FLUSH_RX);
			return len;
		}
	}
	return 0;
}

void Nrf24l01p::command(u8 cmd) {
	_csn.set(false);
	_spi.write(cmd);
	_csn.set(true);
}

void Nrf24l01p::initSlave(u8 * addr) {
	_ce.set(false);

	writeReg(TX_ADDR, addr, 5);
	writeReg(RX_ADDR_P0, addr, 5);

	writeReg(DNYPD, 0x01);
	writeReg(EN_AA, 0x01);
	writeReg(EN_RXADDR, 0x01);

	writeReg(SETUP_RETR, 0x3f);

	setupRx();
}

void Nrf24l01p::initMaster(u8 * addr_msbytes, u8 * lsbytes) {
	_ce.set(false);
	u8 addr[5] = {0};
	addr[0] = lsbytes[0];
	memcpy(addr+1, addr_msbytes, 4);

	writeReg(RX_ADDR_P1, addr, 5);

	for (u8 i = 0; i<4 ; i++) {
		writeReg(RX_ADDR_P2 + i, lsbytes[i+1]);
	}

	writeReg(DNYPD, 0x3e);
	writeReg(EN_AA, 0x3e);
	writeReg(EN_RXADDR, 0x3e);

	writeReg(SETUP_RETR, 0x1f);
}

u8 Nrf24l01p::sendFromMaster(u8 * slave_addr, u8 *data, u8 length) {
	_ce.set(false);
	writeReg(TX_ADDR, slave_addr, 5);
	writeReg(RX_ADDR_P0, slave_addr, 5);
	writeReg(DNYPD, 0x01);
	writeReg(EN_AA, 0x01);
	writeReg(EN_RXADDR, 0x01);

	setupTx();
	u8 result = send(data, length);

	writeReg(DNYPD, 0x3e);
	writeReg(EN_AA, 0x3e);
	writeReg(EN_RXADDR, 0x3e);
	setupRx();

	return result;
}
