"""
@brief      test log(time=4s)
"""


import sys
import os
import unittest
import random
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


class TestCBenchmark(ExtTestCase):

    funcs = [(k, v) for k, v in globals().copy().items()
             if k.startswith("measure_scenario")]

    def _test_benchmark(self, label, values, th, repeat=10, number=20):
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
        rows = self._test_benchmark("shuffled", li, len(li) // 2)
        df = pandas.DataFrame(rows)
        self.assertEqual(df.shape[1], 8)
        self.assertGreater(df.shape[0], 1)

    def test_vector_count_sorted(self):
        li = list(range(300000))
        rows = self._test_benchmark("sorted", li, len(li) // 2)
        df = pandas.DataFrame(rows)
        self.assertEqual(df.shape[1], 8)
        self.assertGreater(df.shape[0], 1)


if __name__ == "__main__":
    unittest.main()
