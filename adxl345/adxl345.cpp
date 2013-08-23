/*
 * adxl345.cpp
 *
 *  Created on: Aug 23, 2013
 *      Author: agu
 */

#include "adxl345/adxl345.h"

float Adxl345::_accelerations[3] = { 0.0, 0.0, 0.0 };
float Adxl345::_scales[3] = { -0.03828125, -0.03828125, -0.03828125 };
float Adxl345::_bias[3] = { 0.0, 0.0, 0.0 };
int32_t Adxl345::_rawdata_sums[3] = { 0, 0, 0 };

Adxl345::Adxl345(I2c & i2c) :
		_i2c(i2c) {
	_measure_count = 0;
	_unit_g = 0.;
}

Adxl345::~Adxl345() {

}

void Adxl345::init() {
	_i2c.setReg(0x53, 0x2d, 0x08); // 345
	_i2c.setReg(0x53, 0x2c, 0x0b); // 345
	_i2c.setReg(0x53, 0x31, 0x09); // 345
	delay(10);
}

void Adxl345::measure() {
	static uint8_t data[6];
	_i2c.getReg(_address, 0x32, data, 6);
	for (uint8_t i = 0; i < 3; i++)
		_accelerations[i] = (int16_t) make16(data[i + i + 1], data[i + i])
				* _scales[i]
				+ _bias[i];
}

void Adxl345::measureRawSums() {
	static uint8_t data[6];
	_i2c.getReg(_address, 0x32, data, 6);
	for (uint8_t i = 0; i < 3; i++)
		_rawdata_sums[i] += (int16_t) make16(data[i + i + 1], data[i + i]);

	_measure_count++;
}

void Adxl345::calibrate() {
	_rawdata_sums[0] = _rawdata_sums[1] = _rawdata_sums[2] = 0;

	for (uint16_t i = 0; i < 400; i++) {
		this->measureRawSums();
		delayMicroseconds(2500);
	}

	_bias[0] = _bias[1] = _bias[2] = 0.0;
	calc();

	_bias[0] = -_accelerations[0];
	_bias[1] = -_accelerations[1];
	_bias[2] = -9.8065 - _accelerations[2];

	_unit_g = fabs(_bias[2] + _accelerations[2]);
}

void Adxl345::calc() {
	for (uint8_t i = 0; i < 3; i++) {
		_accelerations[i] = (_rawdata_sums[i] / (float) _measure_count)
				* _scales[i]
				+ _bias[i];
		_rawdata_sums[i] = 0;
	}
	_measure_count = 0;
}

const float * Adxl345::getAcceleratins() {
	return _accelerations;
}
