/*
 * usart.h
 *
 *  Created on: May 28, 2013
 *      Author: agu
 */

#ifndef USART_H_
#define USART_H_

#include "stm32f10x_usart.h"
#include <stdlib.h>
#include <string.h>

#ifndef RING_BUFF
#define RING_BUFF
struct ring_buff {
	uint16_t *buffer;
	volatile uint8_t index_write;	// to receive the incoming byte
	volatile uint8_t index_read;	// to send byte
};
#endif

class Usart {
public:
	Usart(u8 sn, u8 buff_size = 128);
	virtual ~Usart();

	void init(uint32_t baudrate = 9600,
			u16 word_length = USART_WordLength_8b,
			u16 stop_bits = USART_StopBits_1,
			u16 parity = USART_Parity_No,
			u16 mode = USART_Mode_Rx | USART_Mode_Tx,
			u16 hardware_flow_control = USART_HardwareFlowControl_None);

	u8 available(void);
	u8 cached(void);

	virtual void flush();
	void write(u16 c);
	int read(void);

	int timedRead(u8 timeout=4);

	void ithandler();

	void write(const u8 * p, uint32_t length);

	int readBytes(u8 *buffer, int length, u8 timeout=4);
	int readBytesUntil(char terminator, char *buffer, int length, u8 timeout=4);

	void onTXE();
	void onRXNE();
	ITStatus getInterruptStatus(uint16_t interrupt);

private:
	USART_TypeDef * _usart;

	ring_buff _tx_buff;
	ring_buff _rx_buff;
	const u8 _buff_size;
};

#endif
