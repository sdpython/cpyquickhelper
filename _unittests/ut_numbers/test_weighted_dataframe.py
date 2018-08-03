"""
@brief      test log(time=2s)
"""


import sys
import os
import unittest
import numpy
from pandas import DataFrame
from pandas.core.dtypes.base import ExtensionDtype
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

from src.cpyquickhelper.numbers import WeightedDouble, WeightedSeries
from src.cpyquickhelper.numbers.weighted_dataframe import WeightedSeriesDtype


class TestWeightedSeries(ExtTestCase):

    def test_inheritance(self):
        self.assertIsInstance(WeightedSeriesDtype(), ExtensionDtype)

    def test_series1(self):
        npdtype = numpy.dtype(WeightedDouble)
        self.assertEqual(npdtype.kind, 'O')
        n1 = WeightedDouble(1, 1)
        n2 = WeightedDouble(3, 2)
        ser = WeightedSeries([n1, n2])
        self.assertEqual(list(ser.values), [n1, n2])
        self.assertEqual(ser.values[0], n1)
        self.assertEqual(ser.values[1], n2)
        self.assertEqual(list(ser.value), [1, 3])
        self.assertEqual(list(ser.weight), [1, 2])

    def test_dataframe(self):
        n1 = WeightedDouble(1, 1)
        n2 = WeightedDouble(3, 2)
        ser = WeightedSeries([n1, n2])
        df = DataFrame(data=dict(wd=ser, x=[6., 7.]))
        s = str(df).replace("\n", " ").replace(" ", "_")
        self.assertEqual(
            s, "_____________wd____x_0__1.000000_(1)__6.0_1__3.000000_(2)__7.0")
        self.assertEqual(df.shape, (2, 2))
        df["A"] = df.wd + df.x
        e1 = WeightedDouble(7, 2)
        e2 = WeightedDouble(10, 3)
        self.assertEqual(list(df["A"]), [e1, e2])

    def test_isnan(self):
        n1 = WeightedDouble(1, 1)
        n2 = WeightedDouble(numpy.nan, 2)
        ser = WeightedSeries([n1, n2])
        na1 = ser.isnan()
        self.assertEqual(list(na1), [False, True])


if __name__ == "__main__":
    unittest.main()
