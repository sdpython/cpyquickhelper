"""
@brief      test log(time=3s)
"""

import unittest
import numpy
from pyquickhelper.pycode import ExtTestCase
from cpyquickhelper.numbers import measure_time


class TestMeasureTime(ExtTestCase):

    def test_vector_count(self):
        def fct():
            X = numpy.ones((1000, 5))
            return X
        res = measure_time(
            fct, context={"fct": fct}, div_by_number=False, number=100)
        self.assertIn("average", res)
        res = measure_time(
            fct, context={"fct": fct}, div_by_number=True, number=100)
        self.assertIn("average", res)
        res = measure_time(
            fct, context={"fct": fct}, div_by_number=True, number=1000)
        self.assertIn("average", res)

    def test_vector_count_str(self):
        def fct():
            X = numpy.ones((1000, 5))
            return X
        res = measure_time(
            "fct", context={"fct": fct}, div_by_number=False, number=100)
        self.assertIn("average", res)
        res = measure_time(
            "fct", context={"fct": fct}, div_by_number=True, number=100)
        self.assertIn("average", res)
        res = measure_time(
            "fct", context={"fct": fct}, div_by_number=True, number=1000)
        self.assertIn("average", res)

    def test_vector_count_none(self):
        def fct():
            X = numpy.ones((1000, 5))
            return X
        res = measure_time(
            fct, context=None, div_by_number=False, number=100)
        self.assertIn("average", res)

    def test_vector_count_exc(self):
        def fct():
            X = numpy.ones((1000, 5))
            return X
        self.assertRaise(
            lambda: measure_time(
                "fct", context=None, div_by_number=False, max_time=1),
            ValueError)
        self.assertRaise(
            lambda: measure_time(
                fct, context={"fct": fct}, div_by_number=False, max_time=1),
            ValueError)

    def test_vector_count_max_time(self):
        def fct():
            X = numpy.ones((1000, 5))
            return X
        res1 = measure_time(
            fct, context={"fct": fct}, div_by_number=True,
            repeat=100, number=100)
        res2 = measure_time(
            fct, context={"fct": fct}, div_by_number=True,
            max_time=0.1)
        self.assertEqual(list(sorted(res1)), list(sorted(res2)))
        self.assertGreater(res2['ttime'], 0.1)


if __name__ == "__main__":
    unittest.main()
