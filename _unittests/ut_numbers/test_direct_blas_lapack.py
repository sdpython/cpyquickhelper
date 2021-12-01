# -*- coding: utf-8 -*-
"""
@brief      test log(time=10s)
"""
import unittest
import numpy
from scipy.linalg.lapack import dgelss as scipy_dgelss  # pylint: disable=E0611
from pyquickhelper.pycode import ExtTestCase
from cpyquickhelper.numbers.direct_blas_lapack import dgelss  # pylint: disable=E0611
from cpyquickhelper.numbers.direct_blas_lapack import cblas_ddot, cblas_sdot  # pylint: disable=E0611
from cpyquickhelper.numbers.direct_blas_lapack import (  # pylint: disable=E0611
    cblas_daxpy, cblas_saxpy, cblas_daxpy_void, cblas_saxpy_void)


class TestDirectBlasLapack(ExtTestCase):

    def test_dgels0(self):
        A = numpy.array([[1., 1.], [2., 1.], [3., 1.]])
        C = numpy.array([[-1., 2.]])
        B = numpy.matmul(A, C.T)

        ____, x, ___, __, _, info = scipy_dgelss(A, B)
        self.assertEqual(x.ravel()[:2], C.ravel())
        A = A.T.copy()
        info = dgelss(A, B)
        self.assertEqual(info, 0)
        self.assertEqual(B.ravel()[:2], x.ravel()[:2])

    def test_dgels01(self):
        A = numpy.array([[1., 1.], [2., 1.], [3., 1.]])
        C = numpy.array([[-1., 2.]])
        B = numpy.matmul(A, C.T)
        C[0, 0] = -0.9

        ____, x, ___, __, _, info = scipy_dgelss(A, B)
        A = A.T.copy()
        info = dgelss(A, B)
        self.assertEqual(info, 0)
        self.assertEqual(B.ravel()[:2], x.ravel()[:2])

    def test_dgels1(self):
        A = numpy.array([[10., 1.], [12., 1.], [13., 1]])
        B = numpy.array([[20., 22., 23.]]).T
        ____, x, ___, __, _, info = scipy_dgelss(A, B)
        A = A.T.copy()
        info = dgelss(A, B)
        self.assertEqual(info, 0)
        self.assertEqual(B.ravel()[:2], x.ravel()[:2])

    def test_ddot(self):
        A = numpy.array([1., 2., 3.])
        B = numpy.array([-1., -2.2, 3.])
        dot1 = A @ B
        dot2 = cblas_ddot(A, B)
        self.assertAlmostEqual(dot1, dot2, delta=1e-5)

    def test_sdot(self):
        A = numpy.array([1., 2., 3.], dtype=numpy.float32)
        B = numpy.array([-1., -2.2, 3.], dtype=numpy.float32)
        dot1 = A @ B
        dot2 = cblas_sdot(A, B)
        self.assertAlmostEqual(dot1, dot2)

    def test_daxpy(self):
        A = numpy.array([1., 2., 3.], dtype=numpy.float64)
        B = numpy.array([-1., -2.2, 5], dtype=numpy.float64)
        C = B + A * 5
        cblas_daxpy(A, B, 5)
        self.assertEqualArray(C, B)

    def test_saxpy(self):
        A = numpy.array([1., 2., 3.], dtype=numpy.float32)
        B = numpy.array([-1., -2.2, 5], dtype=numpy.float32)
        C = B + A * 5
        cblas_saxpy(A, B, 5)
        self.assertEqualArray(C, B)

    def test_daxpy_void(self):
        A = numpy.array([1., 2., 3.], dtype=numpy.float64)
        B = numpy.array([-1., -2.2, 5], dtype=numpy.float64)
        C = B + A * 5
        pA, _ = A.__array_interface__['data']
        pB, _ = B.__array_interface__['data']
        cblas_daxpy_void(3, pA, pB, 5)
        self.assertEqualArray(C, B)

    def test_saxpy_void(self):
        A = numpy.array([1., 2., 3.], dtype=numpy.float32)
        B = numpy.array([-1., -2.2, 5], dtype=numpy.float32)
        C = B + A * 5
        pA, _ = A.__array_interface__['data']
        pB, _ = B.__array_interface__['data']
        cblas_saxpy_void(3, pA, pB, 5)
        self.assertEqualArray(C, B)


if __name__ == "__main__":
    unittest.main()
