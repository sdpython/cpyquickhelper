# pylint: disable=E0611
"""
@brief      test log(time=5s)
"""
import unittest
import os
from pyquickhelper.pycode import ExtTestCase
from cpyquickhelper.fastdata.fast_dict import (
    FastDictStrInt64, FastDictInt64Int64, FastDictIntInt)
from cpyquickhelper.fastdata.fast_dict_cpy import cmap_int_int
from cpyquickhelper import __file__ as rootfile


class TestFastDict(ExtTestCase):

    def test_fast_dict_str(self):
        keys = [('A', 6, 7), ('ghhj', 7, 8)]
        d1 = {}
        d2 = FastDictStrInt64()
        for name, key, value in keys:
            self.assertEqual(len(d1), len(d2))
            self.assertEqual(d1.get((name, key), 0), 0)
            self.assertEqual(d2.get(name, key, 0), 0)
            d1[name, key] = value
            d2.insert(name, key, value)
            self.assertEqual(len(d1), len(d2))
            self.assertEqual(d1.get((name, key), 0), value)
            self.assertEqual(d2.get(name, key, 0), value)

    def test_fast_dict_int64(self):
        keys = [(2, 6, 7), (3, 7, 8)]
        d1 = {}
        d2 = FastDictInt64Int64()
        for name, key, value in keys:
            self.assertEqual(len(d1), len(d2))
            self.assertEqual(d1.get((name, key), 0), 0)
            self.assertEqual(d2.get(name, key, 0), 0)
            d1[name, key] = value
            d2.insert(name, key, value)
            self.assertEqual(len(d1), len(d2))
            self.assertEqual(d1.get((name, key), 0), value)
            self.assertEqual(d2.get(name, key, 0), value)

    def test_fast_dict_int(self):
        keys = [(2, 6, 7), (3, 7, 8)]
        d1 = {}
        d2 = FastDictIntInt()
        for name, key, value in keys:
            self.assertEqual(len(d1), len(d2))
            self.assertEqual(d1.get((name, key), 0), 0)
            self.assertEqual(d2.get(name, key, 0), 0)
            d1[name, key] = value
            d2.insert(name, key, value)
            self.assertEqual(len(d1), len(d2))
            self.assertEqual(d1.get((name, key), 0), value)
            self.assertEqual(d2.get(name, key, 0), value)

    def test_fast_dict_int_cpy(self):
        keys = [(2, 6, 7), (3, 7, 8)]
        d1 = {}
        d2 = cmap_int_int()
        for name, key, value in keys:
            self.assertEqual(len(d1), len(d2))
            self.assertEqual(d1.get((name, key), 0), 0)
            self.assertEqual(d2.get(name, key, 0), 0)
            d1[name, key] = value
            d2.insert(name, key, value)
            self.assertEqual(len(d1), len(d2))
            self.assertEqual(d1.get((name, key), 0), value)
            self.assertEqual(d2.get(name, key, 0), value)

    def test_profile_str(self):

        def insert1(d):
            for n, k, v in data:
                d[n, k] = v

        def insert11(d):
            for n, k, v in data:
                if k in d:
                    d[k][n] = v
                else:
                    d[k] = {n: v}

        def insert2(d):
            for n, k, v in data:
                d.insert(n, k, v)

        def g1(d, n, k, v):
            return d.get((n, k), 0)

        def get1(d):
            for n, k, v in data:
                g1(d, n, k, v)

        def g11(d, n, k, v):
            if k in d:
                if n in d[k]:
                    return d[k][n]
            return v

        def get11(d):
            for n, k, v in data:
                g11(d, n, k, v)

        def get2(d):
            for n, k, _ in data:
                d.get(n, k, 0)

        def fct1():
            d1 = {}
            insert1(d1)
            get1(d1)

        def fct11():
            d11 = {}
            insert11(d11)
            get11(d11)

        def fct2():
            d2 = FastDictStrInt64()
            insert2(d2)
            get2(d2)

        def fctN(N=4000):
            for _ in range(N):
                fct1()
                fct11()
                fct2()

        data = [("*" * (i // 2), i, i) for i in range(0, 100)]

        rootrem = os.path.normpath(os.path.abspath(
            os.path.join(os.path.dirname(rootfile), '..')))
        ps, res = self.profile(  # pylint: disable=W0632
            fctN, rootrem=rootrem)
        res = res.replace('\\', '/')
        if __name__ == "__main__":
            print(res)
        self.assertIn('test_fast_dict.py', res)
        self.assertNotEmpty(ps)

    def test_profile_int(self):

        def insert1(d):
            for n, k, v in data:
                d[n, k] = v

        def insert11(d):
            for n, k, v in data:
                if k in d:
                    d[k][n] = v
                else:
                    d[k] = {n: v}

        def insert2(d):
            for n, k, v in data:
                d.insert(n, k, v)

        def g1(d, n, k, v):
            return d.get((n, k), 0)

        def get1(d):
            for n, k, v in data:
                g1(d, n, k, v)

        def g11(d, n, k, v):
            if k in d:
                if n in d[k]:
                    return d[k][n]
            return v

        def get11(d):
            for n, k, v in data:
                g11(d, n, k, v)

        def get2(d):
            for n, k, _ in data:
                d.get(n, k, 0)

        def fct1():
            d1 = {}
            insert1(d1)
            get1(d1)

        def fct11():
            d11 = {}
            insert11(d11)
            get11(d11)

        def fct2():
            d2 = FastDictInt64Int64()
            insert2(d2)
            get2(d2)

        def insert3(d):
            for n, k, v in data:
                d.insert(n, k, v)

        def get3(d):
            for n, k, _ in data:
                d.get(n, k, 0)

        def fct3():
            d3 = cmap_int_int()
            insert3(d3)
            get3(d3)

        def insert4(d):
            for n, k, v in data:
                d.insert(n, k, v)

        def get4(d):
            for n, k, _ in data:
                d.get(n, k, 0)

        def fct4():
            d4 = FastDictIntInt()
            insert4(d4)
            get4(d4)

        def fctN(N=4000):
            for _ in range(N):
                fct1()
                fct11()
                fct2()
                fct3()
                fct4()

        data = [(i * 55, i, i) for i in range(0, 100)]

        rootrem = os.path.normpath(os.path.abspath(
            os.path.join(os.path.dirname(rootfile), '..')))
        ps, res = self.profile(  # pylint: disable=W0632
            fctN, rootrem=rootrem)
        res = res.replace('\\', '/')
        if __name__ == "__main__":
            print(res)
        self.assertIn('test_fast_dict.py', res)
        self.assertNotEmpty(ps)


if __name__ == "__main__":
    unittest.main()
