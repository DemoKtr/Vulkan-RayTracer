

#ifdef __cplusplus
extern "C" {
#endif
extern float stb_perlin_noise3(float x, float y, float z, int x_wrap, int y_wrap, int z_wrap);
extern float stb_perlin_noise3_seed(float x, float y, float z, int x_wrap, int y_wrap, int z_wrap, int seed);
extern float stb_perlin_ridge_noise3(float x, float y, float z, float lacunarity, float gain, float offset, int octaves);
extern float stb_perlin_fbm_noise3(float x, float y, float z, float lacunarity, float gain, int octaves);
extern float stb_perlin_turbulence_noise3(float x, float y, float z, float lacunarity, float gain, int octaves);
extern float stb_perlin_noise3_wrap_nonpow2(float x, float y, float z, int x_wrap, int y_wrap, int z_wrap, unsigned char seed);
#ifdef __cplusplus
}
#endif

#ifdef STB_PERLIN_IMPLEMENTATION




#endif  // STB_PERLIN_IMPLEMENTATION

