/*
 * adxl345.h
 *
 *  Created on: Aug 23, 2013
 *      Author: agu
 */

#ifndef ADXL345_H_
#define ADXL345_H_

#include "i2c/i2c.h"
#include "stm32-template.h"
#include "common/bit_op.h"

#include <cmath>

class Adxl345 {
public:
	Adxl345(I2c & i2c, uint8_t address = 0x53);
	virtual ~Adxl345();
	void init();
	void setOffsets(uint8_t index, uint8_t offset);
	void measure(uint8_t index);
	void measureRawSums();
	void calc();
	const int16_t *getAcceleratins();

private:
	I2c & _i2c;

	const uint8_t _address;
	int16_t _accelerations[3];
	static float _scales[3];
	int32_t _rawdata_sums[3];
	uint16_t _measure_count;
	uint8_t _bias[3];

	float _unit_g;
};

#endif /* ADXL345_H_ */
