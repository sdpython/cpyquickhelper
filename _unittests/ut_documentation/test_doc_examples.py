# -*- coding: utf-8 -*-
"""
@brief      test log(time=33s)
"""

import sys
import os
import unittest
from pandas import DataFrame, Series
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

from src.cpyquickhelper.numbers.weighted_number import WeightedDouble
from src.cpyquickhelper.numbers.weighted_dataframe import WeightedArray


class TestDocumentationExample(ExtTestCase):

    def test_doc_example1(self):
        n1 = WeightedDouble(1, 1)
        n2 = WeightedDouble(3, 2)
        ser = Series([n1, n2])
        df = DataFrame(data=dict(wd=ser, x=[6., 7.]))
        df["A"] = df.wd + df.x
        # Whole dataframe.
        self.assertIn("1.000000 (1)", str(df))
        # Show only the values for column 'wd'.
        val = df.wd.wdouble.value
        self.assertIn("WeightedArray", str(val))
        # About types
        self.assertIn("object", str(df.dtypes))

    def test_doc_example2(self):
        n1 = WeightedDouble(1, 1)
        n2 = WeightedDouble(3, 2)
        ser = WeightedArray([n1, n2])
        df = DataFrame(data=dict(wd=ser, x=[6., 7.]))
        df["A"] = df.wd + df.x
        # Whole dataframe.
        self.assertIn("1.000000 (1)", str(df))
        # Show only the values for column 'wd'.
        val = df.wd.wdouble.value
        self.assertIn("WeightedArray", str(val))
        # About types
        self.assertIn("object", str(df.dtypes))


if __name__ == "__main__":
    unittest.main()
