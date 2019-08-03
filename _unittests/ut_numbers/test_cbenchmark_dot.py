"""
@brief      test log(time=3s)
"""

import unittest
import random
import numpy
import pandas
from pyquickhelper.pycode import ExtTestCase
from pyquickhelper.loghelper import noLOG
from cpyquickhelper.numbers import check_speed
from cpyquickhelper.numbers.cbenchmark import get_simd_available_option  # pylint: disable=W0611, E0611
from cpyquickhelper.numbers.cbenchmark_dot import measure_scenario_A, measure_scenario_B  # pylint: disable=W0611, E0611
from cpyquickhelper.numbers.cbenchmark_dot import measure_scenario_C, measure_scenario_D  # pylint: disable=W0611, E0611
from cpyquickhelper.numbers.cbenchmark_dot import measure_scenario_E, measure_scenario_F  # pylint: disable=W0611, E0611
from cpyquickhelper.numbers.cbenchmark_dot import measure_scenario_G, measure_scenario_H  # pylint: disable=W0611, E0611
from cpyquickhelper.numbers.cbenchmark_dot import measure_scenario_I, measure_scenario_J  # pylint: disable=W0611, E0611
from cpyquickhelper.numbers.cbenchmark_dot import vector_dot_product, empty_vector_dot_product  # pylint: disable=W0611, E0611
from cpyquickhelper.numbers.cbenchmark_dot import vector_dot_product16, vector_dot_product16_sse  # pylint: disable=W0611, E0611
from cpyquickhelper.numbers.cbenchmark_dot import vector_dot_product16_nofcall  # pylint: disable=W0611, E0611
from cpyquickhelper.numbers.cbenchmark_dot import vector_dot_product16_avx512  # pylint: disable=W0611, E0611
from cpyquickhelper.numbers.cbenchmark_dot import vector_dot_product_openmp  # pylint: disable=W0611, E0611


class TestCBenchmarkDot(ExtTestCase):

    funcs = [(k, v) for k, v in globals().copy().items()
             if k.startswith("measure_scenario")]

    def a_test_benchmark(self, label, values, th, repeat=5, number=10):
        rows = []
        for _, v in TestCBenchmarkDot.funcs:
            exe = v(values, th, repeat, number)
            d = exe.todict()
            d['doc'] = v.__doc__.split('``')[1]
            d['label'] = label
            rows.append(d)
            self.assertGreater(exe.average, 0)
        return rows

    def test_vector_count(self):
        li = list(range(300000))
        random.shuffle(li)
        rows = self.a_test_benchmark("shuffled", li, len(li) // 2)
        df = pandas.DataFrame(rows)
        self.assertEqual(df.shape[1], 8)
        self.assertGreater(df.shape[0], 1)

    def test_vector_count_sorted(self):
        li = list(range(300000))
        rows = self.a_test_benchmark("sorted", li, len(li) // 2)
        df = pandas.DataFrame(rows)
        self.assertEqual(df.shape[1], 8)
        self.assertGreater(df.shape[0], 1)

    def test_vector_count_sorted_numpy(self):
        li = numpy.array(range(300000))
        rows = self.a_test_benchmark("sorted", li, len(li) // 2)
        df = pandas.DataFrame(rows)
        self.assertEqual(df.shape[1], 8)
        self.assertGreater(df.shape[0], 1)

    def test_vector_dot_product(self):
        a = numpy.array([3, 4, 5], dtype=numpy.float32)
        b = numpy.array([3.1, 4.1, 5.1], dtype=numpy.float32)
        d1 = numpy.dot(a, b)
        d2 = vector_dot_product(a, b)
        d3 = vector_dot_product16(a, b)
        d4 = vector_dot_product16_sse(a, b)
        d5 = vector_dot_product16_avx512(a, b)
        d6 = vector_dot_product16_nofcall(a, b)
        dE = empty_vector_dot_product(a, b)
        self.assertEqual(dE, 0)
        res = [d1, d2, d3, d4, d5, d6]
        self.assertEqual(d1, d2)
        self.assertEqual(d1, d3)
        self.assertEqual(d1, d4)
        self.assertEqual(d1, d5)
        self.assertEqual(d1, d6)
        self.assertEqual(len(res), 6)

    def test_vector_dot_product_openmp(self):
        a = numpy.array([3, 4, 5], dtype=numpy.float32)
        b = numpy.array([3.1, 4.1, 5.1], dtype=numpy.float32)
        d1 = numpy.dot(a, b)
        d2 = vector_dot_product(a, b)
        d3 = vector_dot_product_openmp(a, b)
        self.assertEqual(d1, d2)
        self.assertEqual(d1, d3)

    def test_vector_dot_product_openmp_1000(self):
        for N in range(1002, 1010):
            a = numpy.array([i % 100 for i in range(0, N)],
                            dtype=numpy.float32)
            b = numpy.array([(i % 100) + 0.1 for i in range(0, N)],
                            dtype=numpy.float32)
            d1 = numpy.dot(a, b)
            d2 = vector_dot_product(a, b)
            self.assertEqualFloat(d1, d2)
            try:
                d3 = vector_dot_product_openmp(a, b, -1)
            except RuntimeError as e:
                if "OPENMP is not enabled" in str(e):
                    return
            self.assertEqualFloat(d1, d3)
            d3 = vector_dot_product_openmp(a, b, 4)
            self.assertEqualFloat(d1, d3)

    def test_vector_dot_product16(self):
        a = numpy.array([3, 4, 5] * 6, dtype=numpy.float32)
        b = numpy.array([3.1, 4.1, 5.1] * 6, dtype=numpy.float32)
        d1 = numpy.dot(a, b)
        d2 = vector_dot_product(a, b)
        d3 = vector_dot_product16(a, b)
        d4 = vector_dot_product16_sse(a, b)
        d5 = vector_dot_product16_avx512(a, b)
        d6 = vector_dot_product16_nofcall(a, b)
        res = [d1, d2, d3, d4, d5, d6]
        self.assertAlmostEqual(d1, d2, places=4)
        self.assertAlmostEqual(d1, d3, places=4)
        self.assertAlmostEqual(d1, d4, places=4)
        self.assertAlmostEqual(d1, d5, places=4)
        self.assertAlmostEqual(d1, d6, places=4)
        self.assertEqual(len(res), 6)

    def test_get_simd_available_option(self):
        vers = get_simd_available_option()
        self.assertIn("options", vers)
        self.assertIn("__SSE__", vers)

    def test_measure_speed(self):
        res = list(check_speed([100], fLOG=noLOG, number=10))
        self.assertIsInstance(res, list)
        self.assertIsInstance(res[0], dict)
        self.assertIn('average', res[0])


if __name__ == "__main__":
    unittest.main()
