#pragma once


#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#define undef__CRT_SECURE_NO_WARNINGS 1
#endif

#include <vector>
#include <iostream>

float vector_float_sum(const float *p1, size_t size);
float vector_double_sum(const float *p1, size_t size);

#if defined(__AVX512F__)
float vector_dot_product_pointer16_avx512(const float *p1, const float *p2, size_t size);
#endif

#if defined(undef_CRT_SECURE_NO_WARNINGS)
#undef _CRT_SECURE_NO_WARNINGS
#endif
