"""
@brief      test log(time=28s)
"""


import sys
import os
import unittest


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
from pyquickhelper.pycode import ExtTestCase
from src.cpyquickhelper.io.stdhelper import capture_output
from src.cpyquickhelper.io.stdchelper import cprint


class TestOutputCapture(ExtTestCase):

    def test_output_capture(self):
        fLOG(
            __file__,
            self._testMethodName,
            OutputPrint=__name__ == "__main__")

        def callf():
            cprint("cout1")
            cprint("tout2")

        out, err = capture_output(callf, lang="c")
        self.assertTrue(isinstance(out, bytes))
        if sys.platform.startswith("win"):
            self.assertEqual(
                out, b'c\x00o\x00u\x00t\x001\x00t\x00o\x00u\x00t\x002\x00')
        else:
            self.assertEqual(
                out, b'cout1tout2')
        self.assertEqual(err, None)

    def test_output_capture_py(self):
        fLOG(
            __file__,
            self._testMethodName,
            OutputPrint=__name__ == "__main__")

        def callf():
            print("cout1")
            print("tout2")

        out, err = capture_output(callf, lang="py")
        self.assertTrue(isinstance(out, str))
        self.assertEqual(out, 'cout1\ntout2\n')
        self.assertEqual(err, '')

    def test_py_output_capture_c(self):
        fLOG(
            __file__,
            self._testMethodName,
            OutputPrint=__name__ == "__main__")

        def callf():
            print("cout1")
            print("tout2")

        out, err = capture_output(callf, lang="c")
        self.assertTrue(isinstance(out, bytes))
        if sys.platform.startswith("win"):
            self.assertEqual(err, None)
            if __name__ == "__main__":
                self.assertEqual(out, b'cout1\r\ntout2\r\n')
            else:
                self.assertTrue(out.endswith(b'cout1\r\ntout2\r\n'))
        else:
            self.assertEqual(err, None)
            if __name__ == "__main__":
                self.assertEqual(out, b'cout1tout2')
            else:
                if not out.endswith(b'cout1\ntout2\n'):
                    raise Exception("###{0}###".format(out))

    def test_c_output_capture_py(self):
        fLOG(
            __file__,
            self._testMethodName,
            OutputPrint=__name__ == "__main__")

        def callf():
            cprint("cout1")
            cprint("tout2")

        out, err = capture_output(callf, lang="py")
        self.assertTrue(isinstance(out, str))
        self.assertEqual(out, '')
        self.assertEqual(err, '')

    def test_c_output_capture_py_error(self):
        fLOG(
            __file__,
            self._testMethodName,
            OutputPrint=__name__ == "__main__")

        callf = 4

        self.assertRaise(lambda: capture_output(callf, lang="py"), TypeError)

    def test_c_output_capture_py_tuple(self):
        fLOG(
            __file__,
            self._testMethodName,
            OutputPrint=__name__ == "__main__")

        def callf():
            cprint("cout1")
            cprint("tout2")
            return (4, 5)

        out, err = capture_output(callf, lang="py")
        self.assertTrue(isinstance(out, str))
        self.assertEqual(out, '')
        self.assertEqual(err, '')

    def test_c_output_capture_py_error2(self):
        fLOG(
            __file__,
            self._testMethodName,
            OutputPrint=__name__ == "__main__")

        def callf():
            cprint("cout1")
            cprint("tout2")
            return (4, 5)

        self.assertRaise(lambda: capture_output(callf, lang="h"), ValueError)


if __name__ == "__main__":
    unittest.main()
