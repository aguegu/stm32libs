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
	//this->write(address, &data, 1);
	uint8_t command[5] = { 0x02, (uint8_t) (address >> 16), (uint8_t) (address
			>> 8), (uint8_t) address, data };
	this->transmit(0x06, 0, 0);
	this->transmit(command, 5, 0, 0);
}

uint8_t Sst25SpiSoft::read(uint32_t address) {
	uint8_t data;
	this->read(address, &data, 1);
	return data;
}

void Sst25SpiSoft::setStatus(uint8_t status) {
	this->transmit(0x06, 0, 0);
	this->transmit(0x50, 0, 0);
	uint8_t command[2] = { 0x01, status };
	this->transmit(command, 2, 0, 0);
}

void Sst25SpiSoft::erase(EraseType type, uint32_t address) {

	uint8_t command[4] = { 0x20, (uint8_t) (address >> 16), (uint8_t) (address
			>> 8), (uint8_t) address };

	this->transmit(0x06, 0, 0);

	if (type == Chip)
		this->transmit(0x60, 0, 0);
	else {
		command[0] = (uint8_t) type;
		this->transmit(command, 4, 0, 0);
	}

	while (isBusy())
		;
}

uint8_t Sst25SpiSoft::isBusy() {
	return this->getStatus() & 0x01;
}

void Sst25SpiSoft::write(uint32_t address, uint8_t *data, uint32_t length) {

	assert_param((length & 0x01) == 0);
	assert_param(length >= 2);

	uint8_t command[4] = { 0xad, (uint8_t) (address >> 16), (uint8_t) (address
			>> 8), (uint8_t) address };

	uint8_t len = 4;
	uint8_t * p = command;

	// Byte-Program:
	//	while (length--)
	//		this->write(address++, *data++);

	// AAI-Word_Program
	this->transmit(0x70, 0, 0);
	this->transmit(0x06, 0, 0);

	while (length) {
		_ss.set(Bit_RESET);

		if (len == 255)
			this->transmitByte(0xad);
		else
			while (len--)
				this->transmitByte(*p++);

		this->transmitByte(*data++);
		this->transmitByte(*data++);

		_ss.set(Bit_SET);

		_ss.set(Bit_RESET);
		while (_miso.getInput() == Bit_RESET)
			;
		_ss.set(Bit_SET);

		length -= 2;
	}

	this->transmit(0x04, 0, 0);
	this->transmit(0x80, 0, 0);
}

void Sst25SpiSoft::read(uint32_t address, uint8_t *data, uint32_t length) {
	uint8_t command[4] = { 0x03, (uint8_t) (address >> 16), (uint8_t) (address
			>> 8), (uint8_t) address };
	this->transmit(command, 4, data, length);
}

