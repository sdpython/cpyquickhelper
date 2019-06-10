"""
@brief      test log(time=2s)
"""
import unittest
import numpy
from pyquickhelper.pycode import ExtTestCase
from cpyquickhelper.numbers.weighted_dataframe import WeightedSeriesDtype
from cpyquickhelper.numbers.weighted_dataframe import WeightedDouble, WeightedArray


class TestMissing(ExtTestCase):

    def test_weighted_dataframe(self):
        obj = WeightedSeriesDtype()
        s = str(obj)
        self.assertEqual(s, "WeightedDouble")
        t = obj.type()
        self.assertNotEmpty(t)
        n = t.name
        self.assertEqual(n, "WeightedDouble")
        cs = t.construct_from_string("WD(4)")
        self.assertEqual(cs, WeightedDouble(4))
        cs = t.construct_from_string("WD(4, 5)")
        self.assertEqual(cs, WeightedDouble(4, 5))
        cs = t.construct_from_string("WD()")
        self.assertNotEqual(cs, WeightedDouble(numpy.nan))

    def test_weighted_array(self):
        arr = WeightedArray([WeightedDouble(4)])
        s = str(arr)
        self.assertEqual(
            s, '<WeightedArray>\n[WeightedDouble(4.000000, 1)]\nLength: 1, dtype: object')
        a2 = arr + arr
        self.assertEqual(str(
            a2), '<WeightedArray>\n[WeightedDouble(8.000000, 2)]\nLength: 1, dtype: object')
        a2 = arr - arr
        self.assertEqual(str(
            a2), '<WeightedArray>\n[WeightedDouble(0.000000, 0)]\nLength: 1, dtype: object')
        a2 = arr * arr
        self.assertEqual(str(
            a2), '<WeightedArray>\n[WeightedDouble(16.000000, 1)]\nLength: 1, dtype: object')
        a2 = arr / arr
        self.assertEqual(str(
            a2), '<WeightedArray>\n[WeightedDouble(1.000000, 1)]\nLength: 1, dtype: object')
        a2 = arr.isna()
        self.assertEqual(str(a2), '[False]')
        serie = numpy.array([WeightedSeriesDtype()])
        a2 = arr._concat_same_type([serie])
        self.assertIn("WeightedDouble", str(a2))


if __name__ == "__main__":
    unittest.main()
