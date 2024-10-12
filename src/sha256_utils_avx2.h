#ifndef __SHA256_UTILS_AVX2_H__
#define __SHA256_UTILS_AVX2_H__

#include <immintrin.h>
#include <stdint.h>
#include <sys/cdefs.h>

union u_block 
{
	__m256i	block;
	uint32_t str[8];
	struct {
		uint32_t a;
		uint32_t b;
		uint32_t c;
		uint32_t d;
		uint32_t e;
		uint32_t f;
		uint32_t g;
		uint32_t h;
	};
};

__always_inline
__m256i shift_right_by_one(__m256i __v) {
    __m256i idx = _mm256_setr_epi32(8, 0, 1, 2, 3, 4, 5, 6);

    __m256i result = _mm256_permutevar8x32_epi32(__v, idx);

    return result;
}

__always_inline
__m128i __mm_rotr32(__m128i __value, size_t __bits)
{
    __m128i right_shifted = _mm_srli_epi32(__value, __bits);
    __m128i left_shifted = _mm_slli_epi32(__value, 32 - __bits);
    return _mm_or_si128(right_shifted, left_shifted);
}

// Shift right (logical) for 32-bit integers
__always_inline
__m128i __mm_chr32(__m128i __value, size_t __bits)
{
    return _mm_srli_epi32(__value, __bits);
}

__always_inline
__m128i __mm_gamma0_32(__m128i __value)
{
    __m128i rotr7 = __mm_rotr32(__value, 7);
    __m128i rotr18 = __mm_rotr32(__value, 18);
    __m128i shr3 = __mm_chr32(__value, 3);
    __m128i result = _mm_xor_si128(rotr7, rotr18);
    return _mm_xor_si128(result, shr3);
}

__always_inline
__m128i __mm_gamma1_32(__m128i __value)
{
    __m128i rotr17 = __mm_rotr32(__value, 17);
    __m128i rotr19 = __mm_rotr32(__value, 19);
    __m128i shr10 = __mm_chr32(__value, 10);
    __m128i result = _mm_xor_si128(rotr17, rotr19);
    return _mm_xor_si128(result, shr10);
}


#endif /* __SHA256_UTILS_AVX2_H__ */
