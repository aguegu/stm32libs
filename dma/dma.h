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
	Dma(DMA_Channel_TypeDef * dma, uint32_t rcc_apb_periph_dmax);
	void init(uint32_t peripheral_base_addr, uint32_t memory_base_addr,
			uint32_t direction, uint32_t buffer_size,
			uint32_t peripheral_increment, uint32_t memory_increment,
			uint32_t peripheral_data_size, uint32_t memory_data_size,
			uint32_t mode, uint32_t priority, uint32_t m2m);
	~Dma();
private:
	DMA_Channel_TypeDef *_dma;
};

#endif /* DMA_H_ */
