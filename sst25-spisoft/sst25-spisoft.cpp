/*
 * sst25-spisoft.cpp
 *
 *  Created on: Jun 8, 2013
 *      Author: agu
 */

#include "sst25-spisoft.h"

Sst25SpiSoft::Sst25SpiSoft(Gpio & mosi, Gpio & miso, Gpio & sck, Gpio & ss) :
		SpiSoft(mosi, miso, sck, ss) {

}

Sst25SpiSoft::~Sst25SpiSoft() {

}

uint16_t Sst25SpiSoft::getId() {
	const static uint8_t out[] = { 0x90, 0x00, 0x00, 0x00 };
	uint8_t in[2];
	transmit(out, 4, in, 2);
	return in[0] << 8 | in[1];
}

uint8_t Sst25SpiSoft::getStatus() {
	uint8_t status;
	transmit(0x05, &status, 1);
	return status;
}

void Sst25SpiSoft::write(uint32_t address, uint8_t data) {
	uint8_t command[5] = { 0x02, (uint8_t) (address >> 16), (uint8_t) (address
			>> 8), (uint8_t) address, data };
	transmit(command, 5, 0, 0);
}

uint8_t Sst25SpiSoft::read(uint32_t address) {
	uint8_t command[4] = { 0x03, (uint8_t) (address >> 16), (uint8_t) (address
			>> 8), (uint8_t) address };

	uint8_t data;
	transmit(command, 4, &data, 1);
	return data;
}

void Sst25SpiSoft::setStatus(uint8_t status) {
	this->transmit(0x06, 0, 0);
	this->transmit(0x50, 0, 0);
	uint8_t command[2] = {0x01, status};
	this->transmit(command, 2, 0, 0);
}

