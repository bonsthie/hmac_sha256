#ifndef __SHA256_UTILS_H__
#define __SHA256_UTILS_H__

#include "bits_manipulation.h"
#include <stdint.h>
#include <sys/cdefs.h>

static const uint32_t K[64] = 
{
    0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL, 0x3956c25bUL,
    0x59f111f1UL, 0x923f82a4UL, 0xab1c5ed5UL, 0xd807aa98UL, 0x12835b01UL,
    0x243185beUL, 0x550c7dc3UL, 0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL,
    0xc19bf174UL, 0xe49b69c1UL, 0xefbe4786UL, 0x0fc19dc6UL, 0x240ca1ccUL,
    0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL, 0x983e5152UL,
    0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL, 0xc6e00bf3UL, 0xd5a79147UL,
    0x06ca6351UL, 0x14292967UL, 0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL,
    0x53380d13UL, 0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL,
    0xa2bfe8a1UL, 0xa81a664bUL, 0xc24b8b70UL, 0xc76c51a3UL, 0xd192e819UL,
    0xd6990624UL, 0xf40e3585UL, 0x106aa070UL, 0x19a4c116UL, 0x1e376c08UL,
    0x2748774cUL, 0x34b0bcb5UL, 0x391c0cb3UL, 0x4ed8aa4aUL, 0x5b9cca4fUL,
    0x682e6ff3UL, 0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL,
    0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL
};

#define DEFAULT_SHA256_HASH                                                    \
  ((uint32_t[8]){                                                              \
      0x6A09E667UL,                                                            \
      0xBB67AE85UL,                                                            \
      0x3C6EF372UL,                                                            \
      0xA54FF53AUL,                                                            \
      0x510E527FUL,                                                            \
      0x9B05688CUL,                                                            \
      0x1F83D9ABUL,                                                            \
      0x5BE0CD19UL,                                                            \
                                                                               \
  })

__always_inline
uint32_t __gamma0_32(uint32_t __value)
{
	return (__rotr32(__value, 7) ^ __rotr32(__value, 18) ^ __shr32(__value, 3));
}

__always_inline
uint32_t __gamma1_32(uint32_t __value)
{
	return (__rotr32(__value, 17) ^ __rotr32(__value, 19) ^ __shr32(__value, 10));
}

__always_inline
uint32_t __sigma0_32(uint32_t __value)
{
	return (__rotr32(__value, 2) ^ __rotr32(__value, 13) ^ __rotr32(__value, 22));
}

__always_inline
uint32_t __sigma1_32(uint32_t __value)
{
	return (__rotr32(__value, 6) ^ __rotr32(__value, 11) ^ __rotr32(__value, 25));
}

/* Ch (choose) */
__always_inline
uint32_t __ch32(uint32_t __x, uint32_t __y, uint32_t __z)
{
	return (__z ^ (__x & (__y ^ __z)));
}

/* Maj (majority) */
__always_inline
uint32_t __maj32(uint32_t __x, uint32_t __y, uint32_t __z)
{
    return ((__x & __y) ^ (__x & __z) ^ (__y & __z));
}

#endif /* __SHA256_UTILS_H__ */
