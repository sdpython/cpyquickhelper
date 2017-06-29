"""
@brief      test log(time=28s)
"""


import sys
import os
import unittest
import datetime


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
from src.cpyquickhelper.io.stdhelper import capture_output
from src.cpyquickhelper.io.stdchelper import cprint


class TestOutputCapture(unittest.TestCase):

    def test_output_capture(self):
        fLOG(
            __file__,
            self._testMethodName,
            OutputPrint=__name__ == "__main__")
            
        def callf():
            cprint("cout1")
            cprint("tout2")

        out, err = capture_output(callf)
        self.assertTrue(isinstance(out, bytes))
        self.assertEqual(out, b'c\x00o\x00u\x00t\x001\x00t\x00o\x00u\x00t\x002\x00')
        self.assertEqual(err, None)


if __name__ == "__main__":
    unittest.main()
