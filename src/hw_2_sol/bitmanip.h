/************************************************************
 * Basic bit manipulation.
 */
#ifndef BITMANIP_H__
#define BITMANIP_H__

#define bit_is_set(reg, bit)	\
		((reg) & (bit))

#define bit_is_clear(reg, bit)	\
		(((!(reg)) & (bit)))

#define loop_until_bit_is_set(reg, bit) \
		do { } while(bit_is_clear(reg, bit))

#define loop_until_bit_is_clear(reg, bit) \
		do { } while(bit_is_set(reg, bit))

#endif	/* BITMANIP_H__ */

/* vim:set ts=4 tw=79 syntax=c et: */
