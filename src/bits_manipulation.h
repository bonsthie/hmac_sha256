#ifndef __BITS_MANIP_H__
#define __BITS_MANIP_H__

#include <stdint.h>
#include <sys/cdefs.h>

__always_inline
uint32_t __shr32(uint32_t value, uint8_t bits)
{
	return (value >> bits);
}

__always_inline
uint32_t __rotr32(uint32_t value, uint8_t bits)
{
	return ((value >> bits ) | value << (32 - bits));
}

#endif /* __BITS_MANIP_H__ */
