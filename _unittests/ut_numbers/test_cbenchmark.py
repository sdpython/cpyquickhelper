"""
@brief      test log(time=8s)
"""


import sys
import os
import unittest
import random
import numpy
import pandas
from pyquickhelper.pycode import ExtTestCase


try:
    import src
except ImportError:
    path = os.path.normpath(
        os.path.abspath(
            os.path.join(
                os.path.split(__file__)[0],
                "..",
                "..")))
    if path not in sys.path:
        sys.path.append(path)
    import src

from src.cpyquickhelper.numbers.cbenchmark import measure_scenario_A, measure_scenario_B  # pylint: disable=W0611, E0611
from src.cpyquickhelper.numbers.cbenchmark import measure_scenario_C, measure_scenario_D  # pylint: disable=W0611, E0611
from src.cpyquickhelper.numbers.cbenchmark import measure_scenario_E, measure_scenario_F  # pylint: disable=W0611, E0611
from src.cpyquickhelper.numbers.cbenchmark import measure_scenario_G, measure_scenario_H  # pylint: disable=W0611, E0611
from src.cpyquickhelper.numbers.cbenchmark import measure_scenario_I, measure_scenario_J  # pylint: disable=W0611, E0611
from src.cpyquickhelper.numbers.cbenchmark import vector_dot_product, empty_vector_dot_product  # pylint: disable=W0611, E0611
from src.cpyquickhelper.numbers.cbenchmark import vector_dot_product16, vector_dot_product16_avx  # pylint: disable=W0611, E0611


class TestCBenchmark(ExtTestCase):

    funcs = [(k, v) for k, v in globals().copy().items()
             if k.startswith("measure_scenario")]

    def a_test_benchmark(self, label, values, th, repeat=10, number=20):
        rows = []
        for _, v in TestCBenchmark.funcs:
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
        d4 = vector_dot_product16_avx(a, b)
        d5 = empty_vector_dot_product(a, b)
        self.assertEqual(d5, 0)
        res = [d1, d2, d3, d4]
        self.assertEqual(d1, d2)
        self.assertEqual(d1, d3)
        self.assertEqual(d1, d4)
        self.assertEqual(len(res), 4)

    def test_vector_dot_product18(self):
        a = numpy.array([3, 4, 5] * 6, dtype=numpy.float32)
        b = numpy.array([3.1, 4.1, 5.1] * 6, dtype=numpy.float32)
        d1 = numpy.dot(a, b)
        d2 = vector_dot_product(a, b)
        d3 = vector_dot_product16(a, b)
        d4 = vector_dot_product16_avx(a, b)
        d5 = empty_vector_dot_product(a, b)
        self.assertEqual(d5, 0)
        res = [d1, d2, d3, d4]
        self.assertAlmostEqual(d1, d2, places=4)
        self.assertAlmostEqual(d1, d3, places=4)
        self.assertAlmostEqual(d1, d4, places=4)
        self.assertEqual(len(res), 4)


if __name__ == "__main__":
    unittest.main()
