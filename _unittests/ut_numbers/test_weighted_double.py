"""
@brief      test log(time=4s)
"""
import platform
import unittest
from inspect import signature, isbuiltin, isclass, ismethod, isfunction, _signature_fromstr, Signature
from pyquickhelper.helpgen import rst2html
from pyquickhelper.pycode import ExtTestCase
from pyquickhelper.pycode.ci_helper import is_travis_or_appveyor
from cpyquickhelper.numbers.weighted_number import WeightedDouble, WeightedFloat  # pylint: disable=E0611
from cpyquickhelper.numbers.weighted_dataframe import WeightedSeriesDtype


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

    @unittest.skipIf(
        platform.system() == 'Windows' and
        is_travis_or_appveyor() == 'appveyor',
        reason='crash')
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
            ".. autosignature:: cpyquickhelper.numbers.weighted_number.WeightedDouble.__init__"]
        newstring = "\n".join(newstring)
        res = rst2html(newstring, writer="rst", layout="sphinx")
        self.assertIn(
            "cpyquickhelper.numbers.weighted_number.WeightedDouble.__init__", res)
        self.assertIn("(*self*, *value*, *weight* = *1.0*)", res)

    def test_attribute(self):
        n = WeightedDouble(4, 5)
        self.assertEqual(n.value, 4)
        self.assertEqual(n.weight, 5)

    def test_construct_from_string(self):
        res = WeightedSeriesDtype.construct_from_string("WD(1, 1)")
        self.assertEqual(WeightedDouble(1, 1), res)


if __name__ == "__main__":
    unittest.main()
