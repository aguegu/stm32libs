/*
 * dma.cpp
 *
 *  Created on: Jun 6, 2013
 *      Author: agu
 */

#include "dma.h"

Dma::Dma(DMA_Channel_TypeDef * dma, uint32_t rcc_apb_periph_dmax) :
		_dma(dma) {
	RCC_AHBPeriphClockCmd(rcc_apb_periph_dmax, ENABLE);
}

Dma::~Dma() {

}

void Dma::init(uint32_t peripheral_base_addr, uint32_t memory_base_addr,
		uint32_t direction, uint32_t buffer_size, uint32_t peripheral_increment,
		uint32_t memory_increment, uint32_t peripheral_data_size,
		uint32_t memory_data_size, uint32_t mode, uint32_t priority,
		uint32_t m2m) {
	DMA_DeInit(_dma);

	DMA_InitTypeDef dma_init_type;

	dma_init_type.DMA_PeripheralBaseAddr = peripheral_base_addr;
	dma_init_type.DMA_MemoryBaseAddr = memory_base_addr;

	dma_init_type.DMA_DIR = direction;

	dma_init_type.DMA_BufferSize = buffer_size;

	dma_init_type.DMA_PeripheralInc = peripheral_increment;
	dma_init_type.DMA_MemoryInc = memory_increment;

	dma_init_type.DMA_PeripheralDataSize = peripheral_data_size;
	dma_init_type.DMA_MemoryDataSize = memory_data_size;

	dma_init_type.DMA_Mode = mode;
	dma_init_type.DMA_Priority = m2m;
	dma_init_type.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(_dma, &dma_init_type);
}

