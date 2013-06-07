/*
 * usart.cpp
 *
 *  Created on: May 28, 2013
 *      Author: agu
 */

#include "usart.h"

Usart::Usart(USART_TypeDef * usart, uint32_t rcc_apbx_periph,
		void (*rcc_apbx_periph_clock_cmd)(uint32_t, FunctionalState),
		uint8_t buff_size, uint16_t time_out) :
		_usart(usart), _buff_size(buff_size), _time_out(time_out) {

	(*rcc_apbx_periph_clock_cmd)(rcc_apbx_periph, ENABLE);

	_tx_buff.index_write = 0;
	_tx_buff.index_read = 0;
	_tx_buff.buffer = (uint16_t *) malloc(sizeof(uint16_t) * _buff_size);

	_rx_buff.index_write = 0;
	_rx_buff.index_read = 0;
	_rx_buff.buffer = (uint16_t *) malloc(sizeof(uint16_t) * _buff_size);
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

	USART_Cmd(_usart, ENABLE);
	USART_ITConfig(_usart, USART_IT_RXNE, ENABLE);
}

uint8_t Usart::available(void) {
	return (_buff_size + _rx_buff.index_write - _rx_buff.index_read)
			% _buff_size;
}

int Usart::read(void) {
	if (_rx_buff.index_write != _rx_buff.index_read) {
		uint16_t c = _rx_buff.buffer[_rx_buff.index_read];
		_rx_buff.index_read = (_rx_buff.index_read + 1) % _buff_size;
		return c;
	}
	return -1;
}

void Usart::flush() {
	while (_tx_buff.index_write != _tx_buff.index_read)
		;
}

void Usart::write(uint16_t c) {
	uint8_t i = (_tx_buff.index_write + 1) % _buff_size;
	while (i == _tx_buff.index_read)
		;

	_tx_buff.buffer[_tx_buff.index_write] = c;
	_tx_buff.index_write = i;

	USART_ITConfig(_usart, USART_IT_TXE, ENABLE);
}

void Usart::transmit() {
	if (_tx_buff.index_read == _tx_buff.index_write) {
		USART_ITConfig(_usart, USART_IT_TXE, DISABLE);
	} else {
		uint16_t c = _tx_buff.buffer[_tx_buff.index_read];
		_tx_buff.index_read = (_tx_buff.index_read + 1) % _buff_size;

		USART_SendData(_usart, c);
	}
}

void Usart::receive() {
	uint8_t i = (_rx_buff.index_write + 1) % _buff_size;

	// when buffer is full, next income would be ignored
	if (i != _rx_buff.index_read) {
		_rx_buff.buffer[_rx_buff.index_write] = USART_ReceiveData(_usart);
		_rx_buff.index_write = i;
	}
}

int Usart::timedRead() {
	extern uint32_t millis;
	uint32_t start = millis;
	uint16_t t = _time_out;

	while (t) {
		int c = read();
		if (c >= 0) return c;

		if (millis - start) {
			t--;
			start++;
		}
	}
	return -1; // -1 indicates timeout
}

int Usart::readBytes(uint8_t *buffer, int length) {
	int index = 0;
	while (index < length) {
		int c = timedRead();
		if (c == -1) break;
		*buffer++ = (uint8_t) c;
		index++;
	}
	return index;
}

// as readBytes with terminator character
// terminates if length characters have been read, timeout, or if the terminator character detected
// returns the number of characters placed in the buffer (0 means no valid data found)

int Usart::readBytesUntil(char terminator, char *buffer, int length) {

	int index = 0;
	while (index < length) {
		int c = timedRead();
		if (c == -1) return -1;
		if (c == terminator) break;
		*buffer++ = (char) c;
		index++;
	}
	return index; // return number of characters, not including null terminator
}

USART_TypeDef * const Usart::base() {
	return _usart;
}

void Usart::ithandler() {
	if (USART_GetITStatus(_usart, USART_IT_TXE )) this->transmit();
	if (USART_GetITStatus(_usart, USART_IT_RXNE )) this->receive();
}
