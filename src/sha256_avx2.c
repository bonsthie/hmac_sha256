#include "print_bits.h"
#include "sha256_utils.h"
#include "sha256_utils_avx2.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/cdefs.h>

static uint8_t *create_padded_buffer(const uint8_t *buffer, size_t buffer_size, size_t *padded_size)
{
    *padded_size = buffer_size + 9; // 1 byte for 0x80, 8 bytes for length
    *padded_size = (*padded_size + 63) & ~63; // Round up to the next multiple of 64

    assert((*padded_size % 64) == 0);

    uint8_t *padded_buffer = _mm_malloc(*padded_size * sizeof(uint8_t), 32);
    if (!padded_buffer)
    {
        *padded_size = 0;
        return (NULL);
    }
	assert((((uintptr_t)padded_buffer) % 32) == 0);

    // Copy original message and append '1' bit (0x80)
    memcpy(padded_buffer, buffer, buffer_size);
    padded_buffer[buffer_size] = 0x80;
    memset(padded_buffer + buffer_size + 1, 0, (*padded_size - buffer_size - 9));

    // Append the length in bits as a 64-bit big-endian integer
    uint64_t bit_len = buffer_size * 8;
    uint8_t *len_ptr = &padded_buffer[*padded_size - 8];
    len_ptr[0] = (bit_len >> 56) & 0xFF;
    len_ptr[1] = (bit_len >> 48) & 0xFF;
    len_ptr[2] = (bit_len >> 40) & 0xFF;
    len_ptr[3] = (bit_len >> 32) & 0xFF;
    len_ptr[4] = (bit_len >> 24) & 0xFF;
    len_ptr[5] = (bit_len >> 16) & 0xFF;
    len_ptr[6] = (bit_len >> 8) & 0xFF;
    len_ptr[7] = bit_len & 0xFF;

    return (padded_buffer);
}

static void prepare_message_schedule(uint32_t words[64], const uint8_t *block)
{
    // First 16 words
    for (int i = 0; i < 16; i++)
    {
        words[i] = (block[i * 4] << 24)
            | (block[i * 4 + 1] << 16)
            | (block[i * 4 + 2] << 8)
            | block[i * 4 + 3];
    }

    // Words 16-63
    /* for (int i = 16; i < 64; i++) */
    /* { */
    /*     uint32_t s0 = __gamma0_32(words[i - 15]); */
    /*     uint32_t s1 = __gamma1_32(words[i - 2]); */
    /*     words[i] = words[i - 16] + s0 + words[i - 7] + s1; */
    /* } */
	for (int i = 16; i < 64; i+= 4)
	{
		__m128i w15 = _mm_loadu_si128((__m128i *)&words[i - 15]);
		__m128i w16 = _mm_loadu_si128((__m128i *)&words[i - 16]);
		__m128i w7 = _mm_loadu_si128((__m128i *)&words[i - 7]);
	
		__m128i s0 = __mm_gamma0_32(w15);
	
		__m128i new_word = _mm_add_epi32(w16, s0);
		new_word = _mm_add_epi32(new_word, w7);
	
		_mm_storeu_si128((__m128i *)&words[i], new_word);
		const uint32_t *store = &words[i - 2];
		for (int y = 0; y < 4; y++)
		{
			words[i + y] += __gamma1_32(store[y]);
	
		}
	}

}

static void compute_hash_block(uint32_t hash[8], uint32_t words[64])
{
    uint32_t a = hash[0];
    uint32_t b = hash[1];
    uint32_t c = hash[2];
    uint32_t d = hash[3];
    uint32_t e = hash[4];
    uint32_t f = hash[5];
    uint32_t g = hash[6];
    uint32_t h = hash[7];

    for (int i = 0; i < 64; i++) {
        uint32_t T1 = h + __sigma1_32(e) + __ch32(e, f, g) + K[i] + words[i];
        uint32_t T2 = __sigma0_32(a) + __maj32(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + T1;
        d = c;
        c = b;
        b = a;
        a = T1 + T2;
    }

    hash[0] += a;
    hash[1] += b;
    hash[2] += c;
    hash[3] += d;
    hash[4] += e;
    hash[5] += f;
    hash[6] += g;
    hash[7] += h;
}

static void calculate_sha256(const uint8_t *buffer, size_t buffer_size, uint8_t sha256_key[32])
{
    uint32_t words[64];
    uint32_t hash[8] = DEFAULT_SHA256_HASH;
    size_t nb_block = buffer_size / 64;

    while (nb_block--)
    {
        prepare_message_schedule(words, buffer);
        compute_hash_block(hash, words);
        buffer += 64;
    }

    for (int i = 0; i < 8; i++)
    {
        sha256_key[i * 4]     = (hash[i] >> 24) & 0xFF;
        sha256_key[i * 4 + 1] = (hash[i] >> 16) & 0xFF;
        sha256_key[i * 4 + 2] = (hash[i] >> 8) & 0xFF;
        sha256_key[i * 4 + 3] = hash[i] & 0xFF;
    }
}

int sha256_avx2(const void *buffer, size_t buffer_size, uint8_t sha256_key[32])
{
    size_t padded_size;
    uint8_t *padded_buffer = create_padded_buffer(buffer, buffer_size, &padded_size);
    if (!padded_buffer)
        return (1);

    calculate_sha256(padded_buffer, padded_size, sha256_key);

    // Optionally print the hash
    for (int i = 0; i < 32; i++) {
        printf("%02x", sha256_key[i]);
    }
    printf("\n");

    free(padded_buffer);
    return (0);
}
