#include "MultithreatedSystems/SimdInstructions/simdRandom.h"
#include <chrono>
#include <intrin.h>
void InitializeRandomState(RandomStateSIMD& state) {
    // Pobierz aktualny czas w nanosekundach
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
    uint64_t seed = std::chrono::duration_cast<std::chrono::nanoseconds>(now).count();

    // Wygeneruj stan generatora oparty na zegarze procesora
    state.state256 = _mm256_set_epi32(
        seed + 7, seed + 6, seed + 5, seed + 4, seed + 3, seed + 2, seed + 1, seed);
}




void InitializeRandomStateWithRDTSC(RandomStateSIMD& state) {
    // Pobierz cykle zegara procesora
    uint64_t seed = __rdtsc();

    // Wygeneruj r騜ne stany na podstawie cykli CPU
    __m128i seed128 = _mm_set_epi32(seed + 3, seed + 2, seed + 1, seed);
    /*
    __m256i seed256 = _mm256_set_epi32(
        seed + 7, seed + 6, seed + 5, seed + 4, seed + 3, seed + 2, seed + 1, seed);
    __m512i seed512 = _mm512_set_epi32(
        seed + 15, seed + 14, seed + 13, seed + 12, seed + 11, seed + 10, seed + 9, seed + 8,
        seed + 7, seed + 6, seed + 5, seed + 4, seed + 3, seed + 2, seed + 1, seed);
*/
   // state.state128 = seed128;
   // state.state256 = seed256;
   // state.state512 = seed512;
}
/*
__m128 RandomSIMD(float min, float max)
{
  
    static thread_local RandomStateSIMD state;
    InitializeRandomStateWithRDTSC(state);
    

    __m128i x = state.state128;
    x = _mm_xor_si128(x, _mm_slli_epi32(x, 13));
    InitializeRandomStateWithRDTSC(state);
    x = _mm_xor_si128(x, _mm_srli_epi32(x, 17));
    InitializeRandomStateWithRDTSC(state);
    x = _mm_xor_si128(x, _mm_slli_epi32(x, 5));
    state.state128 = x;

    __m128 randoms = _mm_cvtepi32_ps(x);
    randoms = _mm_mul_ps(randoms, _mm_set1_ps(1.0f / UINT32_MAX));

    __m128 range = _mm_set1_ps(max - min);
    __m128 offset = _mm_set1_ps(min);
    return _mm_add_ps(_mm_mul_ps(randoms, range), offset);
    
}
*/
__m256 RandomSIMD8(float min, float max)
{
    static thread_local RandomStateSIMD state;
    InitializeRandomState(state);
    __m256i x = state.state256;

    // XORShift: wykonaj operacje na stanie generatora
    x = _mm256_xor_si256(x, _mm256_slli_epi32(x, 13));
    InitializeRandomState(state);
    x = state.state256;
    x = _mm256_xor_si256(x, _mm256_srli_epi32(x, 17));
    InitializeRandomState(state);
     x = state.state256;
    x = _mm256_xor_si256(x, _mm256_slli_epi32(x, 5));

    state.state256 = x;

    // Przekszta單 liczby z 32-bitowych do float
    __m256 randoms = _mm256_cvtepi32_ps(x);

    // Normalizuj do zakresu [0, 1]
    randoms = _mm256_mul_ps(randoms, _mm256_set1_ps(1.0f / UINT32_MAX));

    // Przekszta單 do zakresu [min, max]
    __m256 range = _mm256_set1_ps(max - min);
    __m256 offset = _mm256_set1_ps(min);

    return _mm256_add_ps(_mm256_mul_ps(randoms, range), offset);
}
/*
__m512 RandomSIMD16(float min, float max) {
    
    static thread_local RandomStateSIMD state;
    InitializeRandomStateWithRDTSC(state);
    __m512i x = state.state512;

    // XORShift: wykonaj operacje na stanie generatora
    x = _mm512_xor_si512(x, _mm512_slli_epi32(x, 13));
    InitializeRandomStateWithRDTSC(state);
    x = state.state512;
    x = _mm512_xor_si512(x, _mm512_srli_epi32(x, 17));
    InitializeRandomStateWithRDTSC(state);
    x = state.state512;
    x = _mm512_xor_si512(x, _mm512_slli_epi32(x, 5));
    state.state512 = x;

    // Przekszta單 liczby z 32-bitowych do float
    __m512 randoms = _mm512_cvtepi32_ps(x);

    // Normalizuj do zakresu [0, 1]
    randoms = _mm512_mul_ps(randoms, _mm512_set1_ps(1.0f / UINT32_MAX));

    // Przekszta單 do zakresu [min, max]
    __m512 range = _mm512_set1_ps(max - min);
    __m512 offset = _mm512_set1_ps(min);
    return _mm512_add_ps(_mm512_mul_ps(randoms, range), offset);
    
}
*/
__m128i xorshift128(__m128i& state1, __m128i& state2)
{
    __m128i tmp = state1;

    state1 = state2;
    tmp = _mm_xor_si128(tmp, _mm_slli_epi32(tmp, 23)); // Shift left o 23
    tmp = _mm_xor_si128(tmp, _mm_srli_epi32(tmp, 17)); // Shift right o 17
    tmp = _mm_xor_si128(tmp, state2);
    tmp = _mm_xor_si128(tmp, _mm_srli_epi32(tmp, 26)); // Shift right o 26

    state2 = tmp;
    return _mm_add_epi32(state1, state2); // Zwraca wynik
}

// Funkcja pomocnicza do konwersji na float (0.0 - 1.0)
__m128 xorshift128_to_float(__m128i random_ints) {
    __m128 scaled = _mm_cvtepi32_ps(random_ints); // Zamiana int na float
    return _mm_div_ps(scaled, _mm_set1_ps(static_cast<float>(UINT32_MAX))); // Skaluje do [0, 1)
}
