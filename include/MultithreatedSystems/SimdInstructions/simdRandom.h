#pragma once
#include <immintrin.h>
#include <random>

struct RandomStateSIMD {
   // __m128i state128; // 4x 32-bitowy stan
    __m256i state256; // 8x 32-bitowy stan
  //  __m512i state512; // 16x 32-bitowy stan
};

void InitializeRandomState(RandomStateSIMD& state);
void InitializeRandomStateWithRDTSC(RandomStateSIMD& state);
__m128 RandomSIMD(float min, float max);


__m256 RandomSIMD8(float min, float max);

__m512 RandomSIMD16(float min, float max);

__m128i xorshift128(__m128i& state1, __m128i& state2);