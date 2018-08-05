"""
@brief      test log(time=0s)
"""

import sys
import os
import unittest


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

from src.cpyquickhelper.io.stdchelper import cprint
from src.cpyquickhelper.numbers import WeightedDouble
from src.cpyquickhelper.parallel.threader import kill_thread


class TestC(unittest.TestCase):

    def test_c1(self):
        print("\n**cprint")
        print(cprint)
        print(cprint.__doc__)

    def test_c2(self):
        print("\n**kill_thread")
        print(kill_thread)
        print(kill_thread.__doc__)

    def test_c3(self):
        print("\n**WeightedDouble")
        print(WeightedDouble)
        print(WeightedDouble.__doc__)


if __name__ == "__main__":
    unittest.main()
