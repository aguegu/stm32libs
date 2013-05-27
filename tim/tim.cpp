/*
 * tim.cpp
 *
 *  Created on: May 27, 2013
 *      Author: agu
 */

#include "tim.h"

Tim::Tim(TIM_TypeDef * tim, uint32_t rcc_apbx_periph) :
		_tim(tim) {
	if (IS_RCC_APB1_PERIPH(rcc_apbx_periph))
		RCC_APB1PeriphClockCmd(rcc_apbx_periph, ENABLE);
	else
		RCC_APB2PeriphClockCmd(rcc_apbx_periph, ENABLE);
}

Tim::~Tim() {
	// TODO Auto-generated destructor stub
}

void Tim::init(uint32_t real_clock, uint16_t real_period, uint16_t counter_mode,
		uint16_t clock_division, uint8_t repition_counter) {
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Prescaler = SystemCoreClock / real_clock - 1;
	TIM_TimeBaseStructure.TIM_Period = real_period - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = clock_division;
	TIM_TimeBaseStructure.TIM_CounterMode = counter_mode;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = repition_counter;

	TIM_TimeBaseInit(_tim, &TIM_TimeBaseStructure);
}
