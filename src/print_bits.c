#include "print_bits.h"

static void print_bites_epi8(char c, int fd)
{
	char buffer[8];
	int i = 8;

	while (i--)
	{
		buffer[i] = (c & 1) + '0';
		c >>= 1;
	}
	write(fd, buffer, 8);
}

void print_bits(const void *buffer, size_t buffer_len, enum bits_format format, int fd)
{
	const char *str_buf = buffer;

	while (buffer_len--)
	{
		print_bites_epi8(*str_buf, fd);
		if (format && (buffer_len % format) == 0)
			write(fd, " ", 1);
		str_buf++;
	}
}
