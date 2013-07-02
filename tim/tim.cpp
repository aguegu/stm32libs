/*
 * tim.cpp
 *
 *  Created on: May 27, 2013
 *      Author: agu
 */

#include "tim.h"

Tim::Tim(TIM_TypeDef * tim, uint32_t rcc_apbx_periph,
		void (*p)(uint32_t, FunctionalState)) :
		_tim(tim) {

	(*p)(rcc_apbx_periph, ENABLE);
}

Tim::~Tim() {

}

void Tim::init(uint32_t real_clock, uint16_t real_period, uint16_t counter_mode,
		uint16_t clock_division, uint8_t repition_counter) {
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	uint32_t prescaler = SystemCoreClock / real_clock - 1;
	assert_param(prescaler < 65536);

	TIM_TimeBaseStructure.TIM_Prescaler = prescaler;
	TIM_TimeBaseStructure.TIM_Period = real_period - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = clock_division;
	TIM_TimeBaseStructure.TIM_CounterMode = counter_mode;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = repition_counter;

	TIM_TimeBaseInit(_tim, &TIM_TimeBaseStructure);
}

//////////////////////////////////////////////////////////

TimOc::TimOc() {
	this->init();
}

TimOc::~TimOc() {

}

void TimOc::init(uint16_t oc_mode, uint16_t output_state,
		uint16_t output_nstate, uint16_t pulse, uint16_t oc_polarity,
		uint16_t oc_npolarity, uint16_t oc_idle_state,
		uint16_t oc_nidle_state) {
	_tim_oc_init_type.TIM_OCMode = oc_mode;
	_tim_oc_init_type.TIM_OutputState = output_state;
	_tim_oc_init_type.TIM_OutputNState = output_nstate;
	_tim_oc_init_type.TIM_Pulse = pulse;
	_tim_oc_init_type.TIM_OCPolarity = oc_polarity;
	_tim_oc_init_type.TIM_OCNPolarity = oc_npolarity;
	_tim_oc_init_type.TIM_OCIdleState = oc_idle_state;
	_tim_oc_init_type.TIM_OCNIdleState = oc_nidle_state;
}

void TimOc::apply(TIM_TypeDef * tim,
		void (*p)(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct)) {
	(*p)(tim, &_tim_oc_init_type);
}

//////////////////////////////////////////////////////////////

TimIc::TimIc() {

}

TimIc::~TimIc() {

}

void TimIc::init(uint16_t channel, uint16_t ic_polarity, uint16_t ic_selection,
		uint16_t ic_prescaler, uint16_t ic_filter) {
	_tim_ic_init_type.TIM_Channel = channel;
	_tim_ic_init_type.TIM_ICPolarity = ic_polarity;
	_tim_ic_init_type.TIM_ICSelection = ic_selection;
	_tim_ic_init_type.TIM_ICPrescaler = ic_prescaler;
	_tim_ic_init_type.TIM_ICFilter = ic_filter;
}
