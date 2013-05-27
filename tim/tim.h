/*
 * tim.h
 *
 *  Created on: May 27, 2013
 *      Author: agu
 */

#ifndef TIM_H_
#define TIM_H_

#include "stm32f10x_tim.h"

class Tim {
public:
	Tim(TIM_TypeDef * tim, uint32_t rcc_apbx_periph,
			void (*p)(uint32_t, FunctionalState));
	virtual ~Tim();
	void init(uint32_t real_clock = SystemCoreClock, uint16_t real_period = 0,
			uint16_t counter_mode = TIM_CounterMode_Up,
			uint16_t clock_division = TIM_CKD_DIV1, uint8_t repition_counter =
					0x0000);
private:
	TIM_TypeDef * const _tim;
};

#endif /* TIM_H_ */
