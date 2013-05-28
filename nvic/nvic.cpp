/*
 * nvic.cpp
 *
 *  Created on: May 28, 2013
 *      Author: agu
 */

#include "nvic.h"

Nvic::Nvic() {

}

Nvic::~Nvic() {

}

void Nvic::init(uint8_t irq_channel,
		uint8_t irq_channel_preemption_priority,
		uint8_t irq_channel_sub_priority,
		FunctionalState irq_channel_cmd) {

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = irq_channel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = irq_channel_preemption_priority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = irq_channel_sub_priority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = irq_channel_cmd;
	NVIC_Init(&NVIC_InitStructure);

}

