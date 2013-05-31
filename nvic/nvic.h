/*
 * nvic.h
 *
 *  Created on: May 28, 2013
 *      Author: agu
 */

#ifndef NVIC_H_
#define NVIC_H_

#include "misc.h"

class Nvic {
public:
	Nvic();
	~Nvic();
	void init(uint8_t NVIC_IRQChannel,
			uint8_t NVIC_IRQChannelPreemptionPriority,
			uint8_t NVIC_IRQChannelSubPriority,
			FunctionalState NVIC_IRQChannelCmd);
};

#endif /* NVIC_H_ */
