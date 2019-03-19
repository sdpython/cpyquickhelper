# -*- coding: utf-8 -*-
"""
@brief      test log(time=33s)
"""

import unittest
from pandas import DataFrame, Series
from pyquickhelper.pycode import ExtTestCase

from cpyquickhelper.numbers.weighted_number import WeightedDouble  # pylint: disable=E0611
from cpyquickhelper.numbers.weighted_dataframe import WeightedArray


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
