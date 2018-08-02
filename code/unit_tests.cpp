#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <exception>
#include <string>
#include "unit_tests.h"
#include "weighted_number.h"

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
}
