/*
 * tim.cpp
 *
 *  Created on: May 27, 2013
 *      Author: agu
 */

#include "tim.h"

Tim::Tim(TIM_TypeDef * tim,
	uint32_t rcc_apbx_periph,
	void (*p)(uint32_t, FunctionalState)) :
		_tim(tim) {

	(*p)(rcc_apbx_periph, ENABLE);
}

Tim::~Tim() {

}

void Tim::init(uint32_t real_clock,
	uint16_t real_period,
	uint16_t counter_mode,
	uint16_t clock_division,
	uint8_t repition_counter) {
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	uint32_t prescaler = SystemCoreClock / real_clock - 1;
	assert_param(prescaler < 65536);

	TIM_TimeBaseStructure.TIM_Prescaler = prescaler;
	TIM_TimeBaseStructure.TIM_Period = real_period - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = clock_division;
	TIM_TimeBaseStructure.TIM_CounterMode = counter_mode;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = repition_counter;

	TIM_TimeBaseInit(_tim, &TIM_TimeBaseStructure);
	TIM_Cmd(_tim, ENABLE);
}

void Tim::setCounter(uint16_t counter) {
	TIM_SetCounter(_tim, counter);
}

void Tim::configureIT(uint16_t it, FunctionalState enable) {
	TIM_ITConfig(_tim, it, enable);
}

void Tim::setState(FunctionalState enable) {
	TIM_Cmd(_tim, enable);
}

void Tim::configureArrPreload(FunctionalState enable) {
	TIM_ARRPreloadConfig(_tim, enable);
}

//////////////////////////////////////////////////////////

TimOc::TimOc(TIM_TypeDef * tim,
	void (*tim_ocx_init)(TIM_TypeDef* TIMx,
		TIM_OCInitTypeDef* TIM_OCInitStruct),
	void (*tim_set_comparex)(TIM_TypeDef* TIMx, uint16_t compare)) :
		_tim(tim), _tim_ocx_init(tim_ocx_init), _tim_set_comparex(
			tim_set_comparex) {
}

TimOc::~TimOc() {

}

void TimOc::init(uint16_t oc_mode,
	uint16_t output_state,
	uint16_t output_nstate,
	uint16_t pulse,
	uint16_t oc_polarity,
	uint16_t oc_npolarity,
	uint16_t oc_idle_state,
	uint16_t oc_nidle_state) {

	TIM_OCInitTypeDef tim_oc_init_type;
	tim_oc_init_type.TIM_OCMode = oc_mode;
	tim_oc_init_type.TIM_OutputState = output_state;
	tim_oc_init_type.TIM_OutputNState = output_nstate;
	tim_oc_init_type.TIM_Pulse = pulse;
	tim_oc_init_type.TIM_OCPolarity = oc_polarity;
	tim_oc_init_type.TIM_OCNPolarity = oc_npolarity;
	tim_oc_init_type.TIM_OCIdleState = oc_idle_state;
	tim_oc_init_type.TIM_OCNIdleState = oc_nidle_state;

	(*_tim_ocx_init)(_tim, &tim_oc_init_type);
}

void TimOc::setCompare(uint16_t pulse) {
	(*_tim_set_comparex)(_tim, pulse);
}

//////////////////////////////////////////////////////////////

TimIc::TimIc(TIM_TypeDef * tim) :
		_tim(tim) {

}

TimIc::~TimIc() {

}

void TimIc::init(uint16_t channel,
	uint16_t ic_polarity,
	uint16_t ic_selection,
	uint16_t ic_prescaler,
	uint16_t ic_filter) {
	TIM_ICInitTypeDef tim_ic_init_type;
	tim_ic_init_type.TIM_Channel = channel;
	tim_ic_init_type.TIM_ICPolarity = ic_polarity;
	tim_ic_init_type.TIM_ICSelection = ic_selection;
	tim_ic_init_type.TIM_ICPrescaler = ic_prescaler;
	tim_ic_init_type.TIM_ICFilter = ic_filter;
	TIM_ICInit(_tim, &tim_ic_init_type);
}
