/*
 * usart.cpp
 *
 *  Created on: May 28, 2013
 *      Author: agu
 */

#include "usart.h"

Usart::Usart(u8 sn, u8 buff_size) :
		_buff_size(buff_size) {

	if (sn == 1) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		_usart = USART1;
	} else {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 + (RCC_APB1Periph_USART3 - RCC_APB1Periph_USART2) * (sn - 2), ENABLE);
		_usart = (USART_TypeDef *) (USART2_BASE
				+ (USART3_BASE - USART2_BASE) * (sn - 2));
	}

	_tx_buff.index_write = 0;
	_tx_buff.index_read = 0;
	_tx_buff.buffer = (u16 *) malloc(sizeof(u16) * _buff_size);

  memset(_tx_buff.buffer, 0, _buff_size);

	_rx_buff.index_write = 0;
	_rx_buff.index_read = 0;
	_rx_buff.buffer = (u16 *) malloc(sizeof(u16) * _buff_size);

  memset(_rx_buff.buffer, 0, _buff_size);
}

Usart::~Usart() {
	free(_tx_buff.buffer);
	free(_rx_buff.buffer);
}

void Usart::init(uint32_t baudrate, u16 word_length, u16 stop_bits, u16 parity,
		u16 mode, u16 hardware_flow_control) {
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

u8 Usart::available(void) {
	return (_buff_size + _rx_buff.index_write - _rx_buff.index_read)
			% _buff_size;
}

u8 Usart::cached(void) {
	return (_buff_size + _tx_buff.index_write - _tx_buff.index_read)
			% _buff_size;
}

int Usart::read(void) {
	if (_rx_buff.index_write != _rx_buff.index_read) {
		u16 c = _rx_buff.buffer[_rx_buff.index_read];
		_rx_buff.index_read = (_rx_buff.index_read + 1) % _buff_size;
		return c;
	}
	return -1;
}

void Usart::flush() {
	while (_tx_buff.index_write != _tx_buff.index_read)
		;

	while (USART_GetFlagStatus(_usart, USART_FLAG_TC) == RESET)
		;
}

void Usart::write(u16 c) {
	volatile u8 i = (_tx_buff.index_write + 1) % _buff_size;
	while (i == _tx_buff.index_read)
		;

	_tx_buff.buffer[_tx_buff.index_write] = c;
	_tx_buff.index_write = i;

	USART_ITConfig(_usart, USART_IT_TXE, ENABLE);
}

void Usart::onTXE() {

	USART_SendData(_usart, _tx_buff.buffer[_tx_buff.index_read]);
	_tx_buff.index_read = (_tx_buff.index_read + 1) % _buff_size;

	if (_tx_buff.index_read == _tx_buff.index_write)
		USART_ITConfig(_usart, USART_IT_TXE, DISABLE);
}

void Usart::onRXNE() {
	u8 i = (_rx_buff.index_write + 1) % _buff_size;

	// when buffer is full, next income would be ignored
	if (i != _rx_buff.index_read) {

		if (USART_GetFlagStatus(_usart, USART_FLAG_PE)
				|| USART_GetFlagStatus(_usart, USART_FLAG_ORE)
				|| USART_GetFlagStatus(_usart, USART_FLAG_FE)
				|| USART_GetFlagStatus(_usart, USART_FLAG_NE)) {
			USART_ReceiveData(_usart);
		} else {
			_rx_buff.buffer[_rx_buff.index_write] = USART_ReceiveData(_usart);
			_rx_buff.index_write = i;
		}
	}
}

int Usart::timedRead(u8 timeout) {
	extern uint32_t millis;
	uint32_t start = millis;

	while (timeout) {
		int c = read();
		if (c >= 0)
			return c;

		if (millis - start) {
			timeout--;
			start++;
		}
	}
	return -1; // -1 indicates timeout
}

void Usart::write(const u8 *p, uint32_t length) {
	while (length--)
		this->write(*p++);
}

int Usart::readBytes(u8 *buffer, int length, u8 timeout) {
	int index = 0;
	while (index < length) {
		int c = timedRead(timeout);
		if (c == -1)
			break;
		*buffer++ = (u8) c;
		index++;
	}
	return index;
}

// as readBytes with terminator character
// terminates if length characters have been read, timeout, or if the terminator character detected
// returns the number of characters placed in the buffer (0 means no valid data found)

int Usart::readBytesUntil(char terminator, char *buffer, int length, u8 timeout) {

	int index = 0;
	while (index < length) {
		int c = timedRead(timeout);
		if (c == -1)
			return -1;
		if (c == terminator)
			break;
		*buffer++ = (char) c;
		index++;
	}
	return index; // return number of characters, not including null terminator
}

void Usart::ithandler() {
	if (USART_GetITStatus(_usart, USART_IT_TXE))
		this->onTXE();
	if (USART_GetITStatus(_usart, USART_IT_RXNE))
		this->onRXNE();
}

ITStatus Usart::getInterruptStatus(uint16_t interrupt) {
	return USART_GetITStatus(_usart, interrupt);
}
