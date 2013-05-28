/*
 * usart.cpp
 *
 *  Created on: May 28, 2013
 *      Author: agu
 */

#include "usart.h"
#include "stdlib.h"

Usart::Usart(USART_TypeDef * usart, uint32_t rcc_apbx_periph,
		void (*p)(uint32_t, FunctionalState), uint8_t buff_size) :
		_usart(usart), _buff_size(buff_size) {

	(*p)(rcc_apbx_periph, ENABLE);

	_tx_buff.index_write = 0;
	_tx_buff.index_read = 0;
	_tx_buff.buffer = (uint8_t *) malloc(sizeof(uint8_t) * _buff_size);

	_rx_buff.index_write = 0;
	_rx_buff.index_read = 0;
	_rx_buff.buffer = (uint8_t *) malloc(sizeof(uint8_t) * _buff_size);
}

Usart::~Usart() {
	free(_tx_buff.buffer);
	free(_rx_buff.buffer);
}

void Usart::init(uint32_t baudrate, uint16_t word_length, uint16_t stop_bits,
		uint16_t parity, uint16_t mode, uint16_t hardware_flow_control) {
	USART_InitTypeDef usart_init_type;
	usart_init_type.USART_BaudRate = baudrate;
	usart_init_type.USART_WordLength = word_length;
	usart_init_type.USART_StopBits = stop_bits;
	usart_init_type.USART_Parity = parity;
	usart_init_type.USART_Mode = mode;
	usart_init_type.USART_HardwareFlowControl = hardware_flow_control;

	USART_Init(_usart, &usart_init_type);
}

uint8_t Usart::available(void) {
	return (_buff_size + _rx_buff.index_write - _rx_buff.index_read)
			% _buff_size;
}

uint8_t Usart::read(void) {
	if (_rx_buff.index_write != _rx_buff.index_read) {
		uint8_t c = _rx_buff.buffer[_rx_buff.index_read];
		_rx_buff.index_read = (_rx_buff.index_read + 1) % _buff_size;
		return c;
	}
	return -1;
}

void Usart::flush() {
	while (_tx_buff.index_write != _tx_buff.index_read)
		;
}

uint8_t Usart::write(uint8_t c) {
	uint8_t i = (_tx_buff.index_write + 1) % _buff_size;

	while (i == _tx_buff.index_read)
		;

	_tx_buff.buffer[_tx_buff.index_write] = c;
	_tx_buff.index_write = i;

	USART_ITConfig(_usart, USART_IT_TXE, ENABLE);
	return 1;
}

void Usart::transmit() {
	if (_tx_buff.index_read == _tx_buff.index_write) {
		USART_ITConfig(_usart, USART_IT_TXE, DISABLE);
	} else {
		uint8_t c = _tx_buff.buffer[_tx_buff.index_read];
		_tx_buff.index_read = (_tx_buff.index_read + 1) % _buff_size;

		USART_SendData(_usart, c);
	}
}

void Usart::receive() {
	uint8_t i = (_rx_buff.index_write + 1) % _buff_size;

	// when buffer is full, next income would be ignored
	if (i != _rx_buff.index_read) {
		_rx_buff.buffer[_rx_buff.index_write] = USART_ReceiveData(_usart)
				& 0xff;
		_rx_buff.index_write = i;
	}
}
