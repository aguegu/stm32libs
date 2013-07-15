/*
 * dma.h
 *
 *  Created on: Jun 6, 2013
 *      Author: agu
 */

#ifndef DMA_H_
#define DMA_H_

#include "stm32f10x_dma.h"

class Dma {
public:
	Dma(DMA_Channel_TypeDef * dma, uint32_t rcc_apb_periph_dmax, uint32_t flag);
	~Dma();

	void init(uint32_t peripheral_base_addr = 0,
		uint32_t memory_base_addr = 0,
		uint32_t direction = DMA_DIR_PeripheralSRC,
		uint32_t buffer_size = 0,
		uint32_t peripheral_increment = DMA_PeripheralInc_Disable,
		uint32_t memory_increment = DMA_MemoryInc_Disable,
		uint32_t peripheral_data_size = DMA_PeripheralDataSize_Byte,
		uint32_t memory_data_size = DMA_MemoryDataSize_Byte,
		uint32_t mode = DMA_Mode_Normal,
		uint32_t priority = DMA_Priority_Low,
		uint32_t m2m = DMA_M2M_Disable);
	void setPeriph(uint32_t base_addr, uint32_t data_size =
		DMA_PeripheralDataSize_Byte, uint32_t increment =
		DMA_PeripheralInc_Disable);
	void setMemory(uint32_t base_addr, uint32_t data_size =
		DMA_MemoryDataSize_Byte, uint32_t increment = DMA_MemoryInc_Enable);

	void setFunction(uint32_t mode = DMA_Mode_Normal, uint32_t priority =
		DMA_Priority_Low, uint32_t m2m = DMA_M2M_Disable);

	void setTransfer(uint32_t length,
		uint32_t direction = DMA_DIR_PeripheralDST);

	DMA_Channel_TypeDef * const base();
	void run();
	void setEnable(FunctionalState enable = ENABLE);
private:
	DMA_Channel_TypeDef * const _dma;
	uint32_t _flag;
	DMA_InitTypeDef _dma_init_type;
};

#endif /* DMA_H_ */
