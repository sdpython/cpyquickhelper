"""
@brief      test log(time=3s)
"""


import sys
import os
import unittest
from time import sleep


try:
    import pyquickhelper as skip_
except ImportError:
    path = os.path.normpath(
        os.path.abspath(
            os.path.join(
                os.path.split(__file__)[0],
                "..",
                "..",
                "..",
                "pyquickhelper",
                "src")))
    if path not in sys.path:
        sys.path.append(path)
    import pyquickhelper as skip_

try:
    import src
except ImportError:
    path = os.path.normpath(
        os.path.abspath(
            os.path.join(
                os.path.split(__file__)[0],
                "..",
                "..")))
    if path not in sys.path:
        sys.path.append(path)
    import src

from pyquickhelper.loghelper import fLOG
from src.cpyquickhelper.parallel import KThread


class TestParallel(unittest.TestCase):

    def test_cancel_thread(self):
        fLOG(
            __file__,
            self._testMethodName,
            OutputPrint=__name__ == "__main__")

        def sleep_forever():
            sleep(3)

        th = KThread(target=sleep_forever)
        th.start()
        sleep(0.1)
        th.kill()

        # It does not work on Windows...


if __name__ == "__main__":
    unittest.main()
