/*
 * nvic.cpp
 *
 *  Created on: May 28, 2013
 *      Author: agu
 */

#include "nvic.h"

void Nvic::configureGroup(uint32_t priority_group) {
	NVIC_PriorityGroupConfig(priority_group );
}

void Nvic::configure(uint8_t irq_channel,
	uint8_t preemption_priority,
	uint8_t sub_priority,
	FunctionalState cmd) {

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = irq_channel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =
			preemption_priority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = sub_priority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = cmd;
	NVIC_Init(&NVIC_InitStructure);
}

Nvic nvic;
