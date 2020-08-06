#pragma once


#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#define undef__CRT_SECURE_NO_WARNINGS 1
#endif

#include <vector>
#include <iostream>

float vector_dot_product_pointer(const float *p1, const float *p2, size_t size);
float vector_dot_product_pointer_openmp_default(const float* p1, const float* p2, size_t size);
float vector_dot_product_pointer16(const float *p1, const float *p2, size_t size);
float vector_dot_product_pointer16_nofcall(const float *p1, const float *p2, size_t size);
float vector_dot_product_pointer16_sse(const float *p1, const float *p2, size_t size);

int64_t sum_odd_test(const int64_t* p1, size_t size);
int64_t sum_odd_inline(const int64_t* p1, size_t size);
int64_t sum_odd_b(const int64_t* p1, size_t size);

int64_t sum_odd0_test(const int64_t* p1, size_t size);
int64_t sum_odd0_inline(const int64_t* p1, size_t size);
int64_t sum_odd0_b(const int64_t* p1, size_t size);

#if defined(__AVX512F__)
float vector_dot_product_pointer16_avx512(const float *p1, const float *p2, size_t size);
#endif

#if defined(undef_CRT_SECURE_NO_WARNINGS)
#undef _CRT_SECURE_NO_WARNINGS
#endif
