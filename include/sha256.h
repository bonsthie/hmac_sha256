#ifndef __SHA256_H__
#define __SHA256_H__

#include <stdint.h>
#include <sys/types.h>

int sha256(const void *buffer, size_t buffer_size, uint8_t sha256_key[32]);
int sha256_avx2(const void *buffer, size_t buffer_size, uint8_t sha256_key[32]);

#endif /* __SHA256_H__ */
