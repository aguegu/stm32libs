/*
 * adc.cpp
 *
 *  Created on: Jul 5, 2013
 *      Author: agu
 */

#include "adc.h"

Adc::Adc(ADC_TypeDef *adc, uint32_t rcc_apb2periph_adcx, uint32_t rcc_pclk2_div) :
		_adc(adc) {
	RCC_APB2PeriphClockCmd(rcc_apb2periph_adcx, ENABLE);
	RCC_ADCCLKConfig(rcc_pclk2_div);
}

Adc::~Adc() {
}

void Adc::init(uint32_t mode,
	FunctionalState scan_conv_mode,
	FunctionalState continuous_conv_mode,
	uint32_t external_trig_conv,
	uint32_t data_align,
	uint8_t nbr_of_channel) {

	ADC_InitTypeDef adc_init_type;

	adc_init_type.ADC_Mode = mode;
	adc_init_type.ADC_ScanConvMode = scan_conv_mode;
	adc_init_type.ADC_ContinuousConvMode = continuous_conv_mode;
	adc_init_type.ADC_ExternalTrigConv = external_trig_conv;
	adc_init_type.ADC_DataAlign = data_align;
	adc_init_type.ADC_NbrOfChannel = nbr_of_channel;

	ADC_Init(_adc, &adc_init_type);
	ADC_Cmd(_adc, ENABLE);
}

void Adc::configChannel(uint8_t channel, uint8_t rank, uint8_t sample_time) {
	ADC_RegularChannelConfig(_adc, channel, rank, sample_time);
}

void Adc::calibrate() {
	while (ADC_GetResetCalibrationStatus(_adc))
		;
	ADC_StartCalibration(_adc);
	while (ADC_GetCalibrationStatus(_adc))
		;
}

void Adc::startSoftwareConvert(FunctionalState state) {
	ADC_SoftwareStartConvCmd(ADC1, state);
}

uint16_t Adc::getValue() {
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
		;
	return ADC_GetConversionValue(ADC1);
}
