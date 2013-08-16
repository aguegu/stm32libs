/*
 * st7920-dm.h
 *
 *  Created on: Aug 16, 2013
 *      Author: agu
 */

#ifndef ST7920_DM_H_
#define ST7920_DM_H_

#include "st7920.h"
#include "dot-matrix/dot-matrix.h"

class St7920Dm: public St7920 {
public:
	St7920Dm(Gpio & pin_rs, Gpio & pin_en, Gpio & pin_d4, Gpio & pin_d5,
		Gpio & pin_d6, Gpio & pin_d7);
	~St7920Dm();

	void putDM();
	DotMatrix & getDotMatrix();

private:
	DotMatrix _dm;
};

#endif /* ST7920_DM_H_ */
