"""
@brief      test log(time=28s)
"""

import unittest
import numpy
from pyquickhelper.pycode import ExtTestCase
from cpyquickhelper.examples.vector_container_python import (  # pylint: disable=E0611
    OneTensor, OneTensor2, OneTensorVector,
    RandomTensorVectorFloat, RandomTensorVectorFloat2)


class TestVectorContainer(ExtTestCase):

    def test_one_tensor(self):
        v = numpy.array([[0, 1, -5], [4, 5, 6]], dtype=numpy.float32)
        t = OneTensor()
        t.set_content(v)
        self.assertEqual(v.shape, tuple(t.shape))
        self.assertEqual(t.version, 1)

    def test_one_tensor2(self):
        v = numpy.array([[0, 1, -5], [4, 5, 6]], dtype=numpy.float32)
        t = OneTensor2()
        t.set_content(v)
        self.assertEqual(v.shape, tuple(t.shape))
        self.assertEqual(t.version, 2)

    def test_one_tensor_vector(self):
        v = numpy.array([[0, 1, -5], [4, 5, 6]], dtype=numpy.float32)
        t = OneTensor()
        t.set_content(v)
        vv = OneTensorVector()
        self.assertEqual(len(vv), 0)
        vv.push_back(t)
        self.assertEqual(len(vv), 1)
        self.assertEqual(tuple(vv[0].shape), v.shape)

    def test_random_tensor_vector_float(self):
        c = RandomTensorVectorFloat(10, 11)
        r = c.get_tensor_vector()
        self.assertEqual(len(r), 10)
        self.assertIsInstance(r, OneTensorVector)
        dim = list(map(len, r))
        self.assertEqual(min(dim), 11)
        self.assertEqual(max(dim), 11)

    def test_random_tensor_vector_float2(self):
        c = RandomTensorVectorFloat2(10, 11)
        r = c.get_tensor_vector()
        self.assertEqual(len(r), 10)
        self.assertIsInstance(r, list)
        dim = list(map(len, r))
        self.assertEqual(min(dim), 11)
        self.assertEqual(max(dim), 11)


if __name__ == "__main__":
    unittest.main()
