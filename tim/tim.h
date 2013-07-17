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
	Tim(TIM_TypeDef * tim,
		uint32_t rcc_apbx_periph,
		void (*p)(uint32_t, FunctionalState));
	~Tim();
	void init(uint32_t real_clock = SystemCoreClock,
		uint16_t real_period = 0,
		uint16_t counter_mode = TIM_CounterMode_Up,
		uint16_t clock_division = TIM_CKD_DIV1,
		uint8_t repition_counter = 0x0000);
	void setCounter(uint16_t counter);
	void configureIT(uint16_t it, FunctionalState enable = ENABLE);
	void setState(FunctionalState enable = ENABLE);
	void configureArrPreload(FunctionalState enable = ENABLE);
	ITStatus getITStatus(uint16_t it);
	void clearITPendingBit(uint16_t it);
private:
	TIM_TypeDef * const _tim;
};

class TimOc {
public:
	TimOc(TIM_TypeDef * tim,
		void (*tim_ocx_init)(TIM_TypeDef* TIMx,
			TIM_OCInitTypeDef* TIM_OCInitStruct),
		void (*tim_set_comparex)(TIM_TypeDef* TIMx, uint16_t compare));
	~TimOc();

	void init(uint16_t oc_mode = TIM_OCMode_Timing,
		uint16_t output_state = TIM_OutputState_Disable,
		uint16_t output_nstate = TIM_OutputNState_Disable,
		uint16_t pulse = 0x0000,
		uint16_t oc_polarity = TIM_OCPolarity_High,
		uint16_t oc_npolarity = TIM_OCPolarity_High,
		uint16_t oc_idle_state = TIM_OCIdleState_Reset,
		uint16_t oc_nidle_state = TIM_OCNIdleState_Reset);

	void setCompare(uint16_t pulse);

private:
	TIM_TypeDef * const _tim;
	void (*_tim_ocx_init)(TIM_TypeDef* TIMx,
		TIM_OCInitTypeDef* TIM_OCInitStruct);
	void (*_tim_set_comparex)(TIM_TypeDef* TIMx, uint16_t compare);
};

class TimIc {
public:
	TimIc(TIM_TypeDef * tim);
	~TimIc();
	void init(uint16_t channel = TIM_Channel_1,
		uint16_t ic_polarity = TIM_ICPolarity_Rising,
		uint16_t ic_selection = TIM_ICSelection_DirectTI,
		uint16_t ic_prescaler = TIM_ICPSC_DIV1,
		uint16_t ic_filter = 0x00);

private:
	TIM_TypeDef * const _tim;
};

#endif /* TIM_H_ */
