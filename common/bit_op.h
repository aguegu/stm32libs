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
#define makeWord(h, l) (((h) << 8) | (l))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

#endif /* BIT_OP_H_ */
