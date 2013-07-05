/*
 * adc.h
 *
 *  Created on: Jul 5, 2013
 *      Author: agu
 */

#ifndef ADC_H_
#define ADC_H_

#include "stm32f10x_adc.h"

class Adc {
public:
	Adc(ADC_TypeDef *adc, uint32_t rcc_apb2periph_adcx);
	~Adc();

	void init(uint32_t mode = ADC_Mode_Independent,
		FunctionalState scan_conv_mode = DISABLE,
		FunctionalState continuous_conv_mode = DISABLE,
		uint32_t external_trig_conv = ADC_ExternalTrigConv_T1_CC1,
		uint32_t data_align = ADC_DataAlign_Right,
		uint8_t nbr_of_channel = 1);

	void configChannel(uint8_t channel, uint8_t rank, uint8_t sample_time = ADC_SampleTime_239Cycles5);
	void calibrate();
	void startSoftwareConvert(FunctionalState state = ENABLE);
	uint16_t getValue();
private:
	ADC_TypeDef* const _adc;
};

#endif /* ADC_H_ */
