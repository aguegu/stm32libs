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
	~Tim();
	void init(uint32_t real_clock = SystemCoreClock, uint16_t real_period = 0,
			uint16_t counter_mode = TIM_CounterMode_Up,
			uint16_t clock_division = TIM_CKD_DIV1, uint8_t repition_counter =
					0x0000);
private:
	TIM_TypeDef * const _tim;
};

class TimOc {
public:
	TimOc();
	~TimOc();

	void init(uint16_t oc_mode = TIM_OCMode_Timing, uint16_t output_state =
	TIM_OutputState_Disable, uint16_t output_nstate =
	TIM_OutputNState_Disable, uint16_t pulse = 0x0000, uint16_t oc_polarity =
	TIM_OCPolarity_High, uint16_t oc_npolarity =
	TIM_OCPolarity_High, uint16_t oc_idle_state =
	TIM_OCIdleState_Reset, uint16_t oc_nidle_state =
	TIM_OCNIdleState_Reset);

	void apply(TIM_TypeDef * tim,
			void (*p)(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct));

private:
	TIM_OCInitTypeDef _tim_oc_init_type;
};

class TimIc {
public:
	TimIc();
	~TimIc();
	void init(uint16_t channel = TIM_Channel_1, uint16_t ic_polarity =
			TIM_ICPolarity_Rising, uint16_t ic_selection =
			TIM_ICSelection_DirectTI, uint16_t ic_prescaler = TIM_ICPSC_DIV1,
			uint16_t ic_filter = 0x00);
	void apply();
private:
	TIM_ICInitTypeDef _tim_ic_init_type;
};

#endif /* TIM_H_ */
