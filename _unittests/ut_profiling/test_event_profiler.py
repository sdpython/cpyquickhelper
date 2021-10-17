"""
@brief      test log(time=3s)
"""

import unittest
from time import sleep
from pyquickhelper.pycode import ExtTestCase
from cpyquickhelper.profiling import EventProfiler


class TestEventProfiler(ExtTestCase):

    def test_profiling(self):

        def f1(t):
            sleep(t)

        def f2():
            f1(0.1)

        def f3():
            f1(0.2)

        def f4():
            f2()
            f3()

        ev = EventProfiler()
        ev.start()
        f4()
        ev.stop()
        res = ev.retrieve_raw_results()
        self.assertEqual(res.shape[1], 3)
        df = ev.retrieve_results(False)
        self.assertEqual(df.shape, (res.shape[0], 7))
        expected = ['time', 'kind', 'name', 'filename',
                    'lineno', 'from_name', 'from_line']
        self.assertEqual(list(df.columns), expected)

    def test_profiling_20(self):

        def f1(t):
            sleep(t)

        def f2():
            f1(0.1)

        def f3():
            f1(0.2)

        def f4():
            f2()
            f3()

        ev = EventProfiler(20)
        ev.start()
        f4()
        ev.stop()
        res = ev.retrieve_raw_results()
        self.assertGreater(res.shape[0], 10)
        self.assertEqual(res.shape[1], 3)
        df = ev.retrieve_results(False)
        self.assertEqual(df.shape, (res.shape[0], 7))
        expected = ['time', 'kind', 'name', 'filename',
                    'lineno', 'from_name', 'from_line']
        self.assertEqual(list(df.columns), expected)


if __name__ == "__main__":
    unittest.main()
