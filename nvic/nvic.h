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

	static void configureGroup(uint32_t priority_group = NVIC_PriorityGroup_0);

	static void configure(uint8_t irq_channel,
		uint8_t preemption_priority,
		uint8_t sub_priority,
		FunctionalState cmd);
};

extern Nvic nvic;

#endif /* NVIC_H_ */
