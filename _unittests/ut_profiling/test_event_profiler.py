"""
@brief      test log(time=3s)
"""
import unittest
import inspect
from time import sleep, perf_counter
from pyquickhelper.pycode import ExtTestCase
from cpyquickhelper.profiling import (
    EventProfiler, WithEventProfiler)
from cpyquickhelper.profiling.event_profiler import EventProfilerDebug


class TestEventProfiler(ExtTestCase):

    def test_profiling_exc(self):
        ev = EventProfiler(impl='python')
        self.assertRaise(lambda: ev.stop(), RuntimeError)
        ev.start()
        self.assertRaise(lambda: ev.start(), RuntimeError)
        ev.stop()
        self.assertRaise(lambda: ev.stop(), RuntimeError)

    def test_profiling(self):

        def f1(t):
            sleep(t)

        def f2():
            f1(0.1)

        def f3():
            li = [0 for i in range(0, 10000)]
            f1(0.2)
            return li

        def f4():
            f2()
            f3()

        ev = EventProfiler(impl='python')
        ev.start()
        f4()
        ev.stop()
        res = ev.retrieve_raw_results()
        self.assertEqual(res.shape[1], ev.n_columns)
        df = ev.retrieve_results(False)
        self.assertEqual(df.shape, (res.shape[0], 10))
        expected = ['time', 'value1', 'value2', 'event',
                    'name', 'mod', 'lineno', 'from_name',
                    'from_mod', 'from_line']
        self.assertEqual(list(df.columns), expected)
        self.assertIn('sleep', set(df['name']))
        self.assertIn('time', set(df['mod']))

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

        ev = EventProfiler(20, impl='python')
        ev.start()
        f4()
        ev.stop()
        res = ev.retrieve_raw_results()
        self.assertGreater(res.shape[0], 10)
        self.assertEqual(res.shape[1], ev.n_columns)
        df = ev.retrieve_results(False)
        self.assertEqual(df.shape, (res.shape[0], 10))
        expected = ['time', 'value1', 'value2', 'event',
                    'name', 'mod', 'lineno', 'from_name',
                    'from_mod', 'from_line']
        self.assertEqual(list(df.columns), expected)

    def test_profiling_raise(self):

        def fraise():
            raise RuntimeError("issue")

        def catch_exc():
            try:
                fraise()
                return None
            except RuntimeError as e:
                return str(e)

        ev = EventProfiler(impl='python')
        ev.start()
        catch_exc()
        ev.stop()
        df = ev.retrieve_results(True)
        self.assertEqual(df.shape[1], 10)
        self.assertGreater(df.shape[0], 5)
        self.assertIn("catch_exc", set(df['name']))

    def test_with_sleep(self):

        def fsleep():
            sleep(0.1)

        prof = WithEventProfiler(impl='python')
        with prof:
            fsleep()
        df = prof.report
        self.assertGreater(df.shape[0], 1)
        self.assertEqual(df.shape[1], 10)

    def test_with_raise(self):

        def fraise():
            raise RuntimeError("TESTISSUE")

        try:
            prof = WithEventProfiler(impl='python')
            with prof:
                fraise()
        except RuntimeError as e:
            self.assertEqual(str(e), 'TESTISSUE')

    def test_debug(self):
        N = 100000
        ev = EventProfilerDebug(impl='python')
        ev.start()
        begin = perf_counter()
        for _ in range(N):
            ev.log_event(inspect.currentframe(), 'call', None)
            ev.log_event(inspect.currentframe(), 'return', None)
        end = perf_counter()
        ev.stop()
        duration = end - begin
        msg = "%1.6f microsecond" % (duration / N * 1e6)
        self.assertNotEmpty(msg)
        if __name__ == "__main__":
            print(msg)

    def test_debug_c(self):
        N = 100000
        ev = EventProfilerDebug(impl='pybind11', size=10000000)
        ev.start()
        begin = perf_counter()
        for _ in range(N):
            ev._buffer.c_log_event(  # pylint: disable=W0212
                inspect.currentframe(), 'call', None)
            ev._buffer.c_log_event(  # pylint: disable=W0212
                inspect.currentframe(), 'return', None)
        end = perf_counter()
        ev.stop()
        duration = end - begin
        msg = "%1.6f microsecond" % (duration / N * 1e6)
        self.assertNotEmpty(msg)
        if __name__ == "__main__":
            print(msg)

    def test_profiling_c(self):

        def f1(t):
            sleep(t)

        def f2():
            f1(0.1)

        def f3():
            li = [0 for i in range(0, 10000)]
            f1(0.2)
            return li

        def f4():
            f2()
            f3()

        ev = EventProfiler(impl='pybind11')
        ev.start()
        f4()
        ev.stop()
        res = ev.retrieve_raw_results()
        self.assertEqual(res.shape[1], ev.n_columns)
        df = ev.retrieve_results(False)
        self.assertEqual(df.shape, (res.shape[0], 10))
        expected = ['time', 'value1', 'value2', 'event',
                    'name', 'mod', 'lineno', 'from_name',
                    'from_mod', 'from_line']
        self.assertEqual(list(df.columns), expected)
        self.assertIn('sleep', set(df['name']))
        self.assertIn('time', set(df['mod']))


if __name__ == "__main__":
    unittest.main()
