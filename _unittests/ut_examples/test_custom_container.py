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

    def test_int64(self):
        by = numpy.array([0, 2], dtype=numpy.int64)
        cc = PyCContainer(by)
        dt = cc.dtype
        self.assertEqual(str(dt), "ContainerType.INT64")
        s = cc.size
        self.assertEqual(s, 16)
        li = [cc[i] for i in range(s)]
        self.assertEqual(li, [0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0])

    def test_tuple(self):
        arrays = [numpy.ones((100, 100)) * i for i in range(0, 100)]
        pycs = [PyCContainer(a, True) for a in arrays]

        def copy_to_tuple(pycs):
            return tuple(map(lambda x: PyCContainer(x, False), pycs))

        res = copy_to_tuple(pycs)
        self.assertEqual(len(res), 100)
        self.assertIsInstance(res, tuple)


if __name__ == "__main__":
    unittest.main()
