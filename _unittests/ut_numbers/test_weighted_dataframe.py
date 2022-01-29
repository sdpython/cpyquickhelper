"""
@brief      test log(time=2s)
"""

import unittest
from io import StringIO
import numpy
from pandas import DataFrame, Series, concat
from pandas.core.dtypes.base import ExtensionDtype
from pyquickhelper.pycode import ExtTestCase
from cpyquickhelper.numbers.weighted_number import WeightedDouble  # pylint: disable=E0611
from cpyquickhelper.numbers.weighted_dataframe import WeightedSeriesDtype, WeightedArray, WeightedSeries


class TestWeightedSeries(ExtTestCase):

    def test_inheritance(self):
        self.assertIsInstance(WeightedSeriesDtype(), ExtensionDtype)

    def test_series_0(self):
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

    def test_series0(self):
        npdtype = numpy.dtype(WeightedDouble)
        self.assertEqual(npdtype.kind, 'O')
        n1 = WeightedDouble(1, 1)
        n2 = WeightedDouble(3, 2)
        ser = Series([n1, n2])
        self.assertEqual(list(ser.values), [n1, n2])
        self.assertEqual(ser.values[0], n1)
        self.assertEqual(ser.values[1], n2)
        self.assertEqual(list(ser.wdouble.value), [1, 3])
        self.assertEqual(list(ser.wdouble.weight), [1, 2])

    def test_series1(self):
        npdtype = numpy.dtype(WeightedDouble)
        self.assertEqual(npdtype.kind, 'O')
        n1 = WeightedDouble(1, 1)
        n2 = WeightedDouble(3, 2)
        ser = WeightedSeries([n1, n2])
        self.assertEqual(list(ser.values), [n1, n2])
        self.assertEqual(ser.values[0], n1)
        self.assertEqual(ser.values[1], n2)
        self.assertEqual(list(ser.wdouble.value), [1, 3])
        self.assertEqual(list(ser.wdouble.weight), [1, 2])

    def test_series2(self):
        npdtype = numpy.dtype(WeightedDouble)
        self.assertEqual(npdtype.kind, 'O')
        n1 = WeightedDouble(1, 1)
        n2 = WeightedDouble(3, 2)
        ser = WeightedSeries([n1, n2])
        self.assertEqual(ser.dtype.name, "object")
        df = DataFrame(data=dict(ser=ser))
        ser = df.ser
        self.assertEqual(list(ser.values), [n1, n2])
        self.assertEqual(ser.values[0], n1)
        self.assertEqual(ser.values[1], n2)
        self.assertEqual(list(ser.wdouble.value), [1, 3])
        self.assertEqual(list(ser.wdouble.weight), [1, 2])
        self.assertEqual(list(ser.wdouble.isnan()), [False, False])
        st = StringIO()
        df.to_csv(st)
        val = st.getvalue()
        self.assertIn("(", val)

    def test_numpy(self):
        n1 = WeightedDouble(1, 1)
        n2 = WeightedDouble(3, 2)
        arr = numpy.array([n1, n2])
        self.assertEqual(arr.shape, (2,))

    def test_numpy2(self):
        n1 = WeightedDouble(1, 1)
        n2 = WeightedDouble(3, 2)
        arr = numpy.array([n1, n2], dtype=WeightedDouble)
        self.assertEqual(arr.shape, (2,))

    def test_dataframe(self):
        n1 = WeightedDouble(1, 1)
        n2 = WeightedDouble(3, 2)
        ser = WeightedSeries([n1, n2])
        df = DataFrame(data=dict(wd=ser, x=[6., 7.]))
        self.assertEqual(df.shape, (2, 2))
        df["A"] = df.wd + df.x
        e1 = WeightedDouble(7, 2)
        e2 = WeightedDouble(10, 3)
        self.assertEqual(list(df["A"]), [e1, e2])

    def test_isnan(self):
        n1 = WeightedDouble(1, 1)
        n2 = WeightedDouble(numpy.nan, 2)
        ser = WeightedSeries([n1, n2])
        na1 = ser.wdouble.isnan()
        self.assertEqual(list(na1), [False, True])

    def test_weighted_array(self):
        n1 = WeightedDouble(1, 1)
        n2 = WeightedDouble(3, 2)
        ser = WeightedArray([n1, n2])
        self.assertEqual(ser.name, "WeightedArray")
        self.assertEqual(ser.dtype.name, "object")
        df = DataFrame(data=dict(wd=ser, x=[6., 7.]))
        s = str(df).replace("\n", " ").replace(" ", "_")
        self.assertEqual(
            s, "_____________wd____x_0__1.000000_(1)__6.0_1__3.000000_(2)__7.0")
        self.assertEqual(df.shape, (2, 2))
        df["A"] = df.wd + df.x
        e1 = WeightedDouble(7, 2)
        e2 = WeightedDouble(10, 3)
        self.assertEqual(list(df["A"]), [e1, e2])
        self.assertEqual(df.wd.dtype.name, "object")
        self.assertEqual(df.A.dtype.name, "object")

        df["A1"] = df.wd + df.x
        df["A2"] = df.wd - df.x
        df["A3"] = df.wd * df.x
        df["A4"] = df.wd / df.x
        for c in ['A%d' % i for i in range(1, 5)]:
            self.assertEqual(df[c].dtype.name, "object")

        df["A4"] += df.x
        self.assertEqual(df["A4"].dtype.name, "object")

        df2 = concat([df, df])
        self.assertEqual(df2["A4"].dtype.name, "object")
        self.assertRaise(lambda: df.wd % df.x, TypeError)


if __name__ == "__main__":
    unittest.main()
