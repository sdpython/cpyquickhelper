"""
@brief      test log(time=3s)
"""


import sys
import os
import unittest
import numpy
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

from src.cpyquickhelper.numbers import measure_time


class TestMeasureTime(ExtTestCase):

    def test_vector_count(self):
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


if __name__ == "__main__":
    unittest.main()
