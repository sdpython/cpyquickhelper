"""
@brief      test log(time=3s)
"""

import unittest
import random
import pandas
from pyquickhelper.pycode import ExtTestCase

from cpyquickhelper.numbers.cbenchmark_sum_type import measure_scenario_Float, measure_scenario_Double  # pylint: disable=W0611, E0611
from cpyquickhelper.numbers.cbenchmark_sum_type import vector_float_sum, vector_double_sum  # pylint: disable=W0611, E0611


class TestCBenchmarkSumType(ExtTestCase):

    funcs = [(k, v) for k, v in globals().copy().items()
             if k.startswith("measure_scenario")]

    def a_test_benchmark(self, label, values, repeat=5, number=10):
        rows = []
        for _, v in TestCBenchmarkSumType.funcs:
            exe = v(values, repeat, number)
            d = exe.todict()
            d['doc'] = label
            d['label'] = label
            rows.append(d)
            self.assertGreater(exe.average, 0)
        return rows

    def test_vector_sum(self):
        li = list(range(300000))
        random.shuffle(li)
        rows = self.a_test_benchmark("float", li, len(li) // 2)
        df = pandas.DataFrame(rows)
        self.assertEqual(df.shape[1], 8)
        self.assertEqual(df.shape[0], 2)

    def test_check_value(self):
        vals = list(range(0, 101))
        v1 = vector_float_sum(vals)
        v2 = vector_double_sum(vals)
        self.assertEqual(v1, v2)
        self.assertEqual(v1, 5050)


if __name__ == "__main__":
    unittest.main()
