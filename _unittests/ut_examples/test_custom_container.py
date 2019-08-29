"""
@brief      test log(time=28s)
"""

import unittest
import numpy
from pyquickhelper.pycode import ExtTestCase
from cpyquickhelper.examples.custom_container_python import PyCContainer  # pylint: disable=E0611


class TestCustomContainer(ExtTestCase):

    def test_bytes(self):
        by = b"0ab34"
        cc = PyCContainer(by)
        dt = cc.dtype
        self.assertEqual(str(dt), "ContainerType.BYTE")
        s = cc.size
        self.assertEqual(s, 5)
        li = [cc[i] for i in range(s)]
        self.assertEqual(li, [48, 97, 98, 51, 52])

    def test_float(self):
        by = numpy.array([0, 2, 8, 8e-2, 1], dtype=numpy.float32)
        cc = PyCContainer(by)
        dt = cc.dtype
        self.assertEqual(str(dt), "ContainerType.FLOAT")
        s = cc.size
        self.assertEqual(s, 20)
        li = [cc[i] for i in range(s)]
        self.assertEqual(li, [0, 0, 0, 0, 0, 0, 0, 64, 0,
                              0, 0, 65, 10, 215, 163, 61, 0, 0, 128, 63])

    def test_double(self):
        by = numpy.array([0, 2], dtype=numpy.float64)
        cc = PyCContainer(by)
        dt = cc.dtype
        self.assertEqual(str(dt), "ContainerType.DOUBLE")
        s = cc.size
        self.assertEqual(s, 16)
        li = [cc[i] for i in range(s)]
        self.assertEqual(li, [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 64])


if __name__ == "__main__":
    unittest.main()
