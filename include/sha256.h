#ifndef __SHA256_H__
#define __SHA256_H__

#include <sys/types.h>

int sha256(const void *buffer, size_t buffer_size, char sha256_key[32]);

#endif /* __SHA256_H__ */
