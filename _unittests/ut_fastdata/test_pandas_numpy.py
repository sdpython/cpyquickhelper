"""
@brief      test log(time=28s)
"""

import unittest
import numpy
import pandas
from pyquickhelper.loghelper import fLOG
from pyquickhelper.pycode import ExtTestCase
from cpyquickhelper.fastdata import df2array, df2arrays


class TestPandasNumpy(ExtTestCase):

    def test_df2array(self):
        fLOG(
            __file__,
            self._testMethodName,
            OutputPrint=__name__ == "__main__")

        df = pandas.DataFrame([dict(a=3.4, b=5.6), dict(a=3.5, b=5.7)])
        arr = df2array(df)
        self.assertIsInstance(arr, numpy.ndarray)
        self.assertEqual(arr.shape, (2, 2))
        self.assertEqual(arr.dtype, numpy.float)

        df = pandas.DataFrame(
            [dict(a=3.4, b=5.6, c="e"), dict(a=3.5, b=5.7, c="r")])
        self.assertRaise(lambda: df2array(df), ValueError)

        df = pandas.DataFrame([dict(a=3, b=5), dict(a=3, b=5)])
        arr = df2array(df)
        self.assertIsInstance(arr, numpy.ndarray)
        self.assertEqual(arr.shape, (2, 2))
        self.assertEqual(arr.dtype, numpy.int64)

        df = pandas.DataFrame([dict(a="3", b="5a"), dict(a="3", b="5a")])
        arr = df2array(df)
        self.assertIsInstance(arr, numpy.ndarray)
        self.assertEqual(arr.shape, (2, 2))
        self.assertEqual(arr.dtype, object)

    def test_df2arrays(self):
        fLOG(
            __file__,
            self._testMethodName,
            OutputPrint=__name__ == "__main__")

        df = pandas.DataFrame(
            [dict(a=3.4, b=5.6, c="e"), dict(a=3.5, b=5.7, c="r")])
        arr = df2arrays(df)
        self.assertIsInstance(arr, list)
        keys = [_[0] for _ in arr]
        self.assertIn("a,b", keys)
        self.assertIn("c", keys)
        arr = {k: v for k, v in arr}  # pylint: disable=R1721
        self.assertIsInstance(arr["a,b"], numpy.ndarray)
        self.assertEqual(arr["a,b"].shape, (2, 2))
        self.assertEqual(arr["a,b"].dtype, numpy.float)
        self.assertIsInstance(arr["c"], numpy.ndarray)
        self.assertEqual(arr["c"].shape, (1, 2))
        self.assertEqual(arr["c"].dtype, object)


if __name__ == "__main__":
    unittest.main()
