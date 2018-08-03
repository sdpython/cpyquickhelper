"""
@brief      test log(time=2s)
"""


import sys
import os
import unittest
from inspect import signature, isbuiltin, isclass, ismethod, isfunction, _signature_fromstr, Signature
from pyquickhelper.pycode import ExtTestCase
from pyquickhelper.helpgen import rst2html


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
        self.assertRaise(lambda: signature(
            WeightedDouble.__init__), ValueError)
        self.assertFalse(isbuiltin(WeightedDouble))
        self.assertFalse(isbuiltin(WeightedDouble.__init__))
        self.assertTrue(isclass(WeightedDouble))
        self.assertFalse(ismethod(WeightedDouble.__init__))
        self.assertFalse(isfunction(WeightedDouble.__init__))
        sig = "__init__(value, weight=1)"
        res = _signature_fromstr(Signature, WeightedDouble.__init__, sig)
        self.assertNotEmpty(res)
        newstring = [
            ".. autosignature:: src.cpyquickhelper.numbers.weighted_number.WeightedDouble.__init__"]
        newstring = "\n".join(newstring)
        res = rst2html(newstring, writer="rst", layout="sphinx")
        self.assertIn(
            "src.cpyquickhelper.numbers.weighted_number.WeightedDouble.__init__", res)
        self.assertIn("(*self*, *value*, *weight* = *1.0*)", res)

    def test_attribute(self):
        n = WeightedDouble(4, 5)
        self.assertEqual(n.value, 4)
        self.assertEqual(n.weight, 5)


if __name__ == "__main__":
    unittest.main()
