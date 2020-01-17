# -*- coding: utf-8 -*-
"""
@brief      test log(time=10s)
"""
import unittest
import numpy
from pyquickhelper.pycode import ExtTestCase
from cpyquickhelper.numbers.slowcode import dgemm, sgemm  # pylint: disable=E0611


class TestGemm(ExtTestCase):

    def test_dgemm(self):
        A = numpy.array([[2., 3.], [5., 7.], [11., 13.]])
        B = - A.T.copy()
        C = (A @ B) * 3. - 4.

        Cr = C.copy()
        alpha, beta = 0., 2.
        exp = A @ B * alpha + Cr * beta
        dgemm(False, False, alpha, A, B, beta, Cr)
        self.assertEqualArray(exp, Cr)

        Cr = C.copy()
        alpha, beta = 0., 1.
        exp = A @ B * alpha + Cr * beta
        dgemm(False, False, alpha, A, B, beta, Cr)
        self.assertEqualArray(exp, Cr)

        Cr = C.copy()
        alpha, beta = 1., 0.
        exp = A @ B * alpha + Cr * beta
        dgemm(False, False, alpha, A, B, beta, Cr)
        self.assertEqualArray(exp, Cr)

        Cr = C.copy()
        alpha, beta = 1., 5.
        exp = A @ B * alpha + Cr * beta
        dgemm(False, False, alpha, A, B, beta, Cr)
        self.assertEqualArray(exp, Cr)

    def test_sgemm(self):
        A = numpy.array([[2., 3.], [5., 7.], [11., 13.]]).astype(numpy.float32)
        B = - A.T.copy().astype(numpy.float32)
        C = ((A @ B) * 3. - 4.).astype(numpy.float32)

        Cr = C.copy()
        alpha, beta = numpy.float32(0.), numpy.float32(2.)
        exp = A @ B * alpha + Cr * beta
        sgemm(False, False, alpha, A, B, beta, Cr)
        self.assertEqualArray(exp, Cr)

        Cr = C.copy()
        alpha, beta = numpy.float32(0.), numpy.float32(1.)
        exp = A @ B * alpha + Cr * beta
        sgemm(False, False, alpha, A, B, beta, Cr)
        self.assertEqualArray(exp, Cr)

        Cr = C.copy()
        alpha, beta = numpy.float32(1.), numpy.float32(0.)
        exp = A @ B * alpha + Cr * beta
        sgemm(False, False, alpha, A, B, beta, Cr)
        self.assertEqualArray(exp, Cr)

        Cr = C.copy()
        alpha, beta = numpy.float32(1.), numpy.float32(5.)
        exp = A @ B * alpha + Cr * beta
        sgemm(False, False, alpha, A, B, beta, Cr)
        self.assertEqualArray(exp, Cr)


if __name__ == "__main__":
    unittest.main()
