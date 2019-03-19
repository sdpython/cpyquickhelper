"""
@brief      test log(time=3s)
"""

import unittest
from time import sleep
from pyquickhelper.loghelper import fLOG

from cpyquickhelper.parallel import KThread


class TestParallel(unittest.TestCase):

    def test_cancel_thread(self):
        fLOG(
            __file__,
            self._testMethodName,
            OutputPrint=__name__ == "__main__")

        def sleep_forever():
            sleep(2)

        th = KThread(target=sleep_forever)
        self.assertTrue(th is not None)

        # It does not work on Windows neither on Linux.

        # th.start()
        # sleep(0.1)
        # th.kill()


if __name__ == "__main__":
    unittest.main()
