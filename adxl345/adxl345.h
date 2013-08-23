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
	Adxl345(I2c & i2c);
	virtual ~Adxl345();
	void init();
	void measure();
	void measureRawSums();
	void calibrate();
	void calc();
	const float *getAcceleratins();

private:
	I2c & _i2c;

	static const uint8_t _address = 0x53;
	static float _accelerations[3];
	static float _scales[3];
	static float _bias[3];
	static int32_t _rawdata_sums[3];
	uint16_t _measure_count;

	float _unit_g;
};

#endif /* ADXL345_H_ */
