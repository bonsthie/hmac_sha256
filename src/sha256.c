#include "print_bits.h"
#include <sys/types.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static char *create_padded_buffer(const char *buffer, size_t buffer_size, size_t *padded_size)
{
	if (buffer_size == 0) return (NULL); 

	*padded_size = buffer_size + 9; // 9 for the size;
	*padded_size = (*padded_size + 63) & ~63; // round to 512 bits

	assert((*padded_size % 64) == 0);

	char *padded_buffer = malloc(*padded_size * sizeof(char));
	if (!padded_buffer)
	{
		*padded_size = 0;
		return (NULL);
	}

	// copy + padding
	memcpy(padded_buffer, buffer, buffer_size);
	padded_buffer[buffer_size] = (char)(1 << 7);
	memset(padded_buffer + buffer_size + 1, 0, (*padded_size - buffer_size - 9));

	uint64_t bit_len = buffer_size * 8;
	uint64_t *len_ptr = (uint64_t *)&padded_buffer[*padded_size - 8];
	// force big endian
	*len_ptr = __builtin_bswap64(bit_len);

	return (padded_buffer);
}

int sha256(const char *buffer, size_t buffer_size, char sha256_key[256])
{
	size_t	padded_size;
	char *padded_buffer = create_padded_buffer(buffer, buffer_size, &padded_size);
	if (!padded_buffer)
		return (1);
	print_bites(padded_buffer, padded_size, U512, 1);
	free(padded_buffer);
	(void)sha256_key;
	return (0);
}
