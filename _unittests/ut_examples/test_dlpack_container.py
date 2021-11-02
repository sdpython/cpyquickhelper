"""
@brief      test log(time=28s)
"""

import unittest
import numpy
from pyquickhelper.pycode import ExtTestCase
from cpyquickhelper.examples.dlpack_container_python import (  # pylint: disable=E0611
    PyDLPackContainer, DLDeviceType)


class TestCustomContainer(ExtTestCase):

    def test_float_copy(self):
        by = numpy.array([0, 2, 8, 8e-2, 1], dtype=numpy.float32)
        cc = PyDLPackContainer(by)
        self.assertEqual(cc.dtype.bits, 32)
        self.assertEqual(cc.dtype.code, 2)
        self.assertEqual(cc.dtype.lanes, 1)
        self.assertEqual(cc.device.device_type, DLDeviceType.kDLCPU)
        self.assertEqual(cc.device.device_id, 0)
        self.assertEqual(cc.data_size, 20)
        self.assertEqual(cc.shape_size, 5)
        self.assertEqual(cc.element_size, 4)

    def test_float_nocopy(self):
        by = numpy.array([0, 2, 8, 8e-2, 1], dtype=numpy.float32)
        cc = PyDLPackContainer(by, copy=False)
        self.assertEqual(cc.dtype.bits, 32)
        self.assertEqual(cc.dtype.code, 2)
        self.assertEqual(cc.dtype.lanes, 1)
        self.assertEqual(cc.device.device_type, DLDeviceType.kDLCPU)
        self.assertEqual(cc.device.device_id, 0)
        self.assertEqual(cc.data_size, 20)
        self.assertEqual(cc.shape_size, 5)
        self.assertEqual(cc.element_size, 4)


if __name__ == "__main__":
    unittest.main()
