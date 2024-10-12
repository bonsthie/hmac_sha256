#ifndef __PRINT_BITS_H__
#define __PRINT_BITS_H__

#include <sys/types.h>
#include <unistd.h>


enum bits_format 
{
	NONE,
	BITS,
	U8 = 8,
	U16 = 16,
	U32 = 32,
	U64 = 64,
	U128 = 128,
	U256 = 256,
	U512 = 512
};

void print_bits(const void *buffer, size_t buffer_len, enum bits_format format, int fd);

#endif /* __PRINT_BITS_H__ */
