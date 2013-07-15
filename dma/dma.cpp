/*
 * dma.cpp
 *
 *  Created on: Jun 6, 2013
 *      Author: agu
 */

#include "dma.h"

Dma::Dma(DMA_Channel_TypeDef * dma, uint32_t rcc_apb_periph_dmax, uint32_t flag) :
		_dma(dma), _flag(flag) {
	RCC_AHBPeriphClockCmd(rcc_apb_periph_dmax, ENABLE);
}

Dma::~Dma() {

}

void Dma::init(uint32_t peripheral_base_addr,
	uint32_t memory_base_addr,
	uint32_t direction,
	uint32_t buffer_size,
	uint32_t peripheral_increment,
	uint32_t memory_increment,
	uint32_t peripheral_data_size,
	uint32_t memory_data_size,
	uint32_t mode,
	uint32_t priority,
	uint32_t m2m) {

	this->setPeriph(peripheral_base_addr,
			peripheral_data_size,
			peripheral_increment);
//	_dma_init_type.DMA_PeripheralBaseAddr = peripheral_base_addr;
//	_dma_init_type.DMA_PeripheralDataSize = peripheral_data_size;
//	_dma_init_type.DMA_PeripheralInc = peripheral_increment;

	this->setMemory(memory_base_addr, memory_data_size, memory_increment);
//	_dma_init_type.DMA_MemoryBaseAddr = memory_base_addr;
//	_dma_init_type.DMA_MemoryDataSize = memory_data_size;
//	_dma_init_type.DMA_MemoryInc = memory_increment;

	this->setTransfer(buffer_size, direction);
//	_dma_init_type.DMA_DIR = direction;
//	_dma_init_type.DMA_BufferSize = buffer_size;

	this->setFunction(mode, priority, m2m);
//	_dma_init_type.DMA_Mode = mode;
//	_dma_init_type.DMA_Priority = priority;
//	_dma_init_type.DMA_M2M = m2m;

	DMA_DeInit(_dma);
	DMA_Init(_dma, &_dma_init_type);
}

DMA_Channel_TypeDef * const Dma::base() {
	return _dma;
}

void Dma::run() {
	DMA_Cmd(_dma, ENABLE);
	while (!DMA_GetFlagStatus(_flag))
		;
	DMA_Cmd(_dma, DISABLE);
}

void Dma::setEnable(FunctionalState enable) {
	DMA_Cmd(_dma, enable);
}

void Dma::setPeriph(uint32_t base_addr,
	uint32_t data_size,
	uint32_t increment) {
	_dma_init_type.DMA_PeripheralBaseAddr = base_addr;
	_dma_init_type.DMA_PeripheralDataSize = data_size;
	_dma_init_type.DMA_PeripheralInc = increment;
}

void Dma::setMemory(uint32_t base_addr,
	uint32_t data_size,
	uint32_t increment) {
	_dma_init_type.DMA_MemoryBaseAddr = base_addr;
	_dma_init_type.DMA_MemoryDataSize = data_size;
	_dma_init_type.DMA_MemoryInc = increment;
}

void Dma::setFunction(uint32_t mode, uint32_t priority, uint32_t m2m) {
	_dma_init_type.DMA_Mode = mode;
	_dma_init_type.DMA_Priority = priority;
	_dma_init_type.DMA_M2M = m2m;
}

void Dma::setTransfer(uint32_t length, uint32_t direction) {
	_dma_init_type.DMA_BufferSize = length;
	_dma_init_type.DMA_DIR = direction;
}
