/*
 * st7920-dm.cpp
 *
 *  Created on: Aug 16, 2013
 *      Author: agu
 */

#include "st7920/st7920-dm.h"

St7920Dm::St7920Dm(Gpio & pin_rs, Gpio & pin_en, Gpio & pin_d4, Gpio & pin_d5,
	Gpio & pin_d6, Gpio & pin_d7) :
		St7920(pin_rs, pin_en, pin_d4, pin_d5, pin_d6, pin_d7), _dm(128, 64) {

}

St7920Dm::~St7920Dm() {

}

void St7920Dm::putDM() {
	uint8_t *ppa = _dm.output();
	uint8_t *ppb = ppa + 512;

	for (uint8_t r = 0; r < 0x20; r++) {
		this->setDdRam(r); // y
		this->setDdRam(0x00); // x

		this->transmit(true, ppa, 0x10);
		this->transmit(true, ppb, 0x10);

		ppa += 0x10;
		ppb += 0x10;
	}
}

DotMatrix & St7920Dm::getDotMatrix() {
	return _dm;
}
