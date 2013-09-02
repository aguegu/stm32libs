/*
 * adxl345.cpp
 *
 *  Created on: Aug 23, 2013
 *      Author: agu
 */

#include "adxl345/adxl345.h"

Adxl345::Adxl345(I2c & i2c, uint8_t address) :
		_i2c(i2c), _address(address) {
	_measure_count = 0;
	_unit_g = 0.;
	_rawdata_sums[0] = _rawdata_sums[1] = _rawdata_sums[2] = 0;
	_bias[0] = _bias[1] = _bias[2] = 0;
}

Adxl345::~Adxl345() {

}

void Adxl345::init() {
	_i2c.setReg(_address, 0x2d, 0x08);
	_i2c.setReg(_address, 0x2c, 0x0b);
	_i2c.setReg(_address, 0x31, 0x09);
	delay(10);
}

void Adxl345::setOffsets(uint8_t index, uint8_t offset) {
       _i2c.setReg(_address, 0x1e + index, offset);
}

void Adxl345::measure(uint8_t index) {
	uint8_t data[2];
	_i2c.getReg(_address, 0x32 + index + index, data, 2);
	_accelerations[index] = (int16_t) make16(data[1], data[0]);
}

void Adxl345::measureRawSums() {
	static uint8_t data[6];
	_i2c.getReg(_address, 0x32, data, 6);
	for (uint8_t i = 0; i < 3; i++)
		_rawdata_sums[i] += (int16_t) make16(data[i + i + 1], data[i + i]);

	_measure_count++;
}

void Adxl345::calc() {
	for (uint8_t i = 0; i < 3; i++) {
		_accelerations[i] = (_rawdata_sums[i] / (float) _measure_count)
				* _scales[i] + _bias[i];
		_rawdata_sums[i] = 0;
	}
	_measure_count = 0;
}

const int16_t * Adxl345::getAcceleratins() {
	return _accelerations;
}
