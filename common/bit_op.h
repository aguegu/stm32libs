/*
 * bit_op.h
 *
 *  Created on: Jul 19, 2013
 *      Author: agu
 */

#ifndef BIT_OP_H_
#define BIT_OP_H_

#define _BV(bit) (1 << (bit))

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))
#define make16(h, l) (((h) << 8) | (l))

#define rawmax(a, b) ((a)>(b)?(a):(b))
#define rawmin(a, b) ((a)<(b)?(a):(b))


#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

#define readLowBitInByte(value, bit) ((value) & _BV(bit))
#define setLowBitInByte(value, bit) ((value) |= _BV(bit))
#define clearLowbitInByte(value, bit) ((value) &= ~_BV(bit))
#define writeLowBitInByte(value, bit, bitvalue) (bitvalue ? setLowBitInByte(value, bit) : clearLowbitInByte(value, bit))

#define readHighBitInByte(value, bit) ((value) & (0x80 >> (bit)))
#define setHighBitInByte(value, bit) ((value) |= (0x80 >> (bit)))
#define clearHighBitInByte(value, bit) ((value) &= ~(0x80 >> (bit)))
#define writeHighBitInByte(value, bit, bitvalue) (bitvalue ? setHighBitInByte(value, bit) : clearHighBitInByte(value, bit))

#endif /* BIT_OP_H_ */
