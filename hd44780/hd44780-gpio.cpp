/*
 * hd44780-gpio.cpp
 *
 *  Created on: Aug 11, 2013
 *      Author: agu
 */

#include "hd44780/hd44780-gpio.h"

Hd44780Gpio::Hd44780Gpio(Gpio & pin_rs, Gpio & pin_en, Gpio & pin_d4,
		Gpio & pin_d5, Gpio & pin_d6, Gpio & pin_d7, uint8_t row_count,
		uint8_t col_count) :Hd44780Basic(row_count, col_count) ,
_pin_rs(pin_rs), _pin_en(pin_en)
//,_pin_dt[0](pin_d4), _pin_dt[1](pin_d5), _pin_dt[2](pin_d6), _pin_dt[3](pin_d7)
{

}

Hd44780Gpio::~Hd44780Gpio() {

}

