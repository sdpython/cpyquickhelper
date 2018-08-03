"""
@brief      test log(time=28s)
"""


import sys
import os
import unittest
from pyquickhelper.pycode import ExtTestCase
from inspect import signature


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

from src.cpyquickhelper.numbers import WeightedDouble, WeightedFloat


class TestWeightedDouble(ExtTestCase):

    def test_creation(self):

        n1 = WeightedDouble(1, 1)
        n2 = WeightedDouble(1, 1)
        n3 = n1 + n2
        self.assertEqual(n3, WeightedDouble(2, 2))
        self.assertEqual(str(n3), "2.000000 (2)")
        self.assertEqual(repr(n3), "WeightedDouble(2.000000, 2)")

        n1 = WeightedDouble(1, 1)
        n2 = WeightedDouble(1, 1)
        n3 = n1 + 1
        self.assertEqual(n3, WeightedDouble(2, 2))

        n1 = WeightedFloat(1, 1)
        n2 = WeightedFloat(1, 1)
        n3 = n1 + n2
        self.assertEqual(n3, WeightedFloat(2, 2))

        n1 = WeightedFloat(1, 1)
        n2 = WeightedFloat(1, 1)
        n3 = n1 + 1
        self.assertEqual(n3, WeightedFloat(2, 2))

    def test_signature(self):
        self.assertRaise(lambda: signature(WeightedDouble.__init__), ValueError)


if __name__ == "__main__":
    unittest.main()
