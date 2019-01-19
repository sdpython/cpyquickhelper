#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <chrono>
#include <exception>
#include <string>
#include <vector>
#include "unit_tests.h"
#include "weighted_number.h"
#include "cbenchmark.h"

void ASSERT_EQUAL(double a, double b)
{
    if (a != b) {
        char buffer[100];
        sprintf(buffer, "%f != %f", a, b);
        throw std::exception(buffer);
    }
}

void ASSERT_EQUAL_STRING(std::string a, std::string b)
{
    if (a != b) {
        std::string quote("'");
        std::string message = quote + a + quote + std::string(" != ") + quote + b + quote;
        throw new std::exception(message.c_str());
    }
}

void test_addition()
{
    WeightedDouble n1(1, 1);
    WeightedDouble n2(1, 1);
    auto res = n1 + n2;
    ASSERT_EQUAL(res.value, 2);
    ASSERT_EQUAL(res.weight, 2);

    n1 += n2;
    ASSERT_EQUAL(n1.value, 2);
    ASSERT_EQUAL(n1.weight, 2);
}

void test_addition2()
{
    WeightedDouble n1(1, 1);
    auto res = n1 + 1;
    ASSERT_EQUAL(res.value, 2);
    ASSERT_EQUAL(res.weight, 2);

    n1 += 1;
    ASSERT_EQUAL(n1.value, 2);
    ASSERT_EQUAL(n1.weight, 2);
}

void test_multiplication()
{
    WeightedDouble n1(1, 1);
    WeightedDouble n2(1, 1);
    auto res = n1 * n2;
    ASSERT_EQUAL(res.value, 1);
    ASSERT_EQUAL(res.weight, 1);

    n1 *= n2;
    ASSERT_EQUAL(n1.value, 1);
    ASSERT_EQUAL(n1.weight, 1);
}

void test_division()
{
    WeightedDouble n1(1, 1);
    WeightedDouble n2(1, 1);
    auto res = n1 / n2;
    ASSERT_EQUAL(res.value, 1);
    ASSERT_EQUAL(res.weight, 1);

    n1 /= n2;
    ASSERT_EQUAL(n1.value, 1);
    ASSERT_EQUAL(n1.weight, 1);
}

void test_soustraction()
{
    WeightedDouble n1(1, 1);
    WeightedDouble n2(1, 1);
    auto res = n1 - n2;
    ASSERT_EQUAL(res.value, 0);
    ASSERT_EQUAL(res.weight, 0);

    n1 -= n2;
    ASSERT_EQUAL(n1.value, 0);
    ASSERT_EQUAL(n1.weight, 0);
}

void test_soustraction2()
{
    WeightedDouble n1(1, 1);
    auto res = n1 - 1;
    ASSERT_EQUAL(res.value, 0);
    ASSERT_EQUAL(res.weight, 0);

    n1 -= 1;
    ASSERT_EQUAL(n1.value, 0);
    ASSERT_EQUAL(n1.weight, 0);
}

void test_multiplication2()
{
    WeightedDouble n1(1, 1);
    auto res = n1 * 2;
    ASSERT_EQUAL(res.value, 2);
    ASSERT_EQUAL(res.weight, 2);

    n1 *= 2;
    ASSERT_EQUAL(n1.value, 2);
    ASSERT_EQUAL(n1.weight, 2);
}

void test_division2()
{
    WeightedDouble n1(1, 1);
    auto res = n1 / 2.;
    ASSERT_EQUAL(res.value, 0.5);
    ASSERT_EQUAL(res.weight, 0.5);

    n1 /= 2.;
    ASSERT_EQUAL(n1.value, 0.5);
    ASSERT_EQUAL(n1.weight, 0.5);
}

void test_equal()
{
    WeightedDouble n1(1, 1);
    WeightedDouble n2(0, 0);
    auto v = -n1;
    WeightedDouble op(v);
    n2 = op;
    ASSERT_EQUAL(n2.value, -1);
    ASSERT_EQUAL(n2.weight, -1);
}

void test_string()
{
    WeightedDouble n1(1, 2);
    auto s = n1.__repr__();
    ASSERT_EQUAL_STRING(s, "WeightedDouble(1.000000, 2)");
    s = n1.__str__();
    ASSERT_EQUAL_STRING(s, "1.000000 (2)");
}

void test_comparisons()
{
    WeightedDouble n1(1, 1);
    WeightedDouble n2(1, 1);
    if (!(n1 == n2)) throw std::exception("Issue");
    if (n1 != n2) throw std::exception("Issue");
    WeightedDouble m1(1, 2);
    WeightedDouble m2(2, 1);
    if (!(m1 == m2)) throw std::exception("Issue");
    if (m1 != m2) throw std::exception("Issue");
    if (m1 == n2) throw std::exception("Issue");
    if (m1 < n2) throw std::exception("Issue");
    if (m1 <= n2) throw std::exception("Issue");
    if (!(m1 > n2)) throw std::exception("Issue");
    if (!(m1 >= n2)) throw std::exception("Issue");
}

#define THROWERRORMSG(d1, d2) \
{ \
    char buffer[200]; \
    sprintf(buffer, "%g != %g", d1, d2); \
    throw std::runtime_error(buffer); \
}

void test_vector_dot()
{
    std::vector<float> v1{ 1, 2, 3 };
    std::vector<float> v2{ 1.1, 2.1, 3.1 };
    float d1 = vector_dot_product_pointer(v1.data(), v2.data(), v1.size());
    float d2 = vector_dot_product_pointer16(v1.data(), v2.data(), v1.size());
    float d3 = vector_dot_product_pointer16_nofcall(v1.data(), v2.data(), v1.size());
    float d4 = vector_dot_product_pointer16_sse(v1.data(), v2.data(), v1.size());
    if (!(d1 == d2)) THROWERRORMSG(d1, d2);
    if (!(d1 == d3)) THROWERRORMSG(d1, d3);
    if (!(d1 == d4)) THROWERRORMSG(d1, d4);
#if defined(__AVX512F__)
    float d5 = vector_dot_product_pointer16_avx512(v1.data(), v2.data(), v1.size());
    if (!(d1 == d5)) THROWERRORMSG(d1, d5);
#endif
}

typedef float dot_type(const float*, const float*, size_t size);

double benchmark(dot_type * fct, const std::vector<float>& v1, const std::vector<float>& v2, int repeat, int number)
{
    const float * p1 = v1.data();
    const float * p2 = v2.data();
    double sum = 0;
    auto start = std::chrono::high_resolution_clock::now();
    for (int r = 0; r < repeat; ++r)
    {
        for (int i = 0; i < number; ++i) {
            sum += (*fct)(p1, p2, v1.size());
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    if (sum < 0)
        throw std::runtime_error("issue 2");
    return std::chrono::duration<double>(end - start).count() / repeat;
}

void test_vector_dot_benchmark(int n, int repeat = 10, int number = 50)
{
    std::vector<float> v1(n);
    std::vector<float> v2(n);
    for (int i = 0; i < v1.size(); ++i) {
        v1[i] = i;
        v2[i] = i + 0.1;
    }
    double t;
    t = benchmark(&vector_dot_product_pointer, v1, v2, repeat, number);
    std::cout << v1.size() << " -- vector_dot_product_pointer:          " << t << "\n";
    t = benchmark(&vector_dot_product_pointer16, v1, v2, repeat, number);
    std::cout << v1.size() << " -- vector_dot_product_pointer16:        " << t << "\n";
    t = benchmark(&vector_dot_product_pointer16_nofcall, v1, v2, repeat, number);
    std::cout << v1.size() << " -- vector_dot_product_pointer16_nofcall:" << t << "\n";
    t = benchmark(&vector_dot_product_pointer16_sse, v1, v2, repeat, number);
    std::cout << v1.size() << " -- vector_dot_product_pointer16_sse:    " << t << "\n";
}

void run_all_tests()
{
    test_addition();
    test_addition2();
    test_division();
    test_division2();
    test_multiplication();
    test_multiplication2();
    test_soustraction();
    test_soustraction2();
    test_equal();
    test_string();
    test_vector_dot();
    for (int n = 10; n < 1000000; n += 10000) {
        std::cout << "\n";
        test_vector_dot_benchmark(n);
    }
    printf("end\n");
}
