/*
 * usart.h
 *
 *  Created on: May 28, 2013
 *      Author: agu
 */

#ifndef USART_H_
#define USART_H_

#include "stm32f10x_usart.h"

struct ring_buff {
	uint16_t *buffer;
	volatile uint8_t index_write;	// to receive the incoming byte
	volatile uint8_t index_read;	// to send byte
};

class Usart {
public:
	Usart(USART_TypeDef * usart, uint32_t rcc_apbx_periph,
			void (*p)(uint32_t, FunctionalState), uint8_t buff_size = 64);
	virtual ~Usart();

	void init(uint32_t baudrate = 9600, uint16_t word_length =
			USART_WordLength_8b, uint16_t stop_bits = USART_StopBits_1,
			uint16_t parity = USART_Parity_No,
			uint16_t mode = USART_Mode_Rx | USART_Mode_Tx,
			uint16_t hardware_flow_control = USART_HardwareFlowControl_None );

	uint8_t available(void);
	void write(uint16_t c);
	uint16_t read(void);
	void transmit();
	void receive();
	void flush();

private:
	USART_TypeDef * const _usart;

	ring_buff _tx_buff;
	ring_buff _rx_buff;
	const uint8_t _buff_size;
};

#endif /* USART_H_ */
