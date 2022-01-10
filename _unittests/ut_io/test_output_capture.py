"""
@brief      test log(time=28s)
"""
import sys
import os
import unittest
from inspect import signature, isbuiltin, isfunction, _signature_fromstr, Signature
from pyquickhelper.pycode import ExtTestCase, skipif_appveyor
from pyquickhelper.helpgen import rst2html
import cpyquickhelper
from cpyquickhelper.io.stdhelper import capture_output
from cpyquickhelper.io.stdchelper import cprint  # pylint: disable=E0611


class TestOutputCapture(ExtTestCase):

    def test_output_capture(self):
        def callf():
            cprint("cout1")
            cprint("zout2")

        res, out, err = capture_output(callf, lang="c")
        self.assertEmpty(res)
        self.assertIsInstance(out, bytes)
        if sys.platform.startswith("win"):
            self.assertEqual(
                out, b'c\x00o\x00u\x00t\x001\x00z\x00o\x00u\x00t\x002\x00')
        else:
            self.assertEqual(
                out, b'cout1zout2')
        self.assertEqual(err, None)

    def test_output_capture_py(self):
        def callf():
            print("cout1")
            print("bout2")

        fout, out, err = capture_output(callf, lang="py")
        self.assertIsInstance(out, str)
        self.assertEqual(out, 'cout1\nbout2\n')
        self.assertEqual(err, '')
        self.assertEmpty(fout)

    def test_py_output_capture_c(self):
        def callf():
            print("cout1")
            print("aout2")

        res, out, err = capture_output(callf, lang="c")
        self.assertEmpty(res)
        if out is not None:
            self.assertIsInstance(out, bytes)
            if sys.platform.startswith("win"):
                self.assertEqual(err, None)
                if __name__ == "__main__":
                    self.assertEqual(out, b'cout1\r\naout2\r\n')
                else:
                    self.assertTrue(out.endswith(b'cout1\r\naout2\r\n'))
            else:
                self.assertEqual(err, None)
                if __name__ == "__main__":
                    self.assertEqual(out, b'cout1aout2')
                else:
                    if not out.endswith(b'cout1\naout2\n'):
                        raise AssertionError("###{0}###".format(out))

    def test_c_output_capture_py(self):
        def callf():
            cprint("cout1")
            cprint("fout2")

        fout, out, err = capture_output(callf, lang="py")
        self.assertIsInstance(out, str)
        self.assertEqual(out, '')
        self.assertEqual(err, '')
        self.assertEmpty(fout)

    def test_c_output_capture_py_error(self):
        callf = 4

        self.assertRaise(lambda: capture_output(callf, lang="py"), TypeError)

    def test_c_output_capture_py_tuple(self):
        def callf():
            cprint("cout1")
            cprint("gout2")
            return (4, 5)

        fout, out, err = capture_output(callf, lang="py")
        self.assertIsInstance(out, str)
        self.assertEqual(out, '')
        self.assertEqual(err, '')
        self.assertEqual(fout, (4, 5))

    def test_c_output_capture_py_error2(self):
        def callf():
            cprint("cout1")
            cprint("hout2")
            return (4, 5)

        self.assertRaise(lambda: capture_output(callf, lang="h"), ValueError)

    def test_doc(self):
        self.assertIn("Displays a string on the standard output",
                      cprint.__doc__)

    @skipif_appveyor("Miktex is not installed.")
    def test_signature(self):
        from pyquickhelper.sphinxext.import_object_helper import (
            import_any_object, import_object)
        self.assertRaise(lambda: signature(cprint), ValueError)
        self.assertTrue(isbuiltin(cprint))
        self.assertFalse(isfunction(cprint))
        sig = "cprint(*args)"
        res = _signature_fromstr(Signature, cprint, sig)
        docname = "cpyquickhelper.io.stdchelper.cprint"
        obj, _ = import_object(docname, "function", use_init=False)
        self.assertNotEmpty(obj)
        obj, _, kind = import_any_object(docname, use_init=False)
        self.assertNotEmpty(obj)
        self.assertEqual(kind, "function")
        self.assertNotEmpty(res)
        dirname = os.path.join(os.path.abspath(os.path.dirname(
            cpyquickhelper.__file__)), "..")
        excs = []
        for name in ['cpyquickhelper.io.stdchelper.cprint',
                     'cpyquickhelper.io.stdchelper.cprint']:
            newstring = ".. autosignature:: {0}\n    :debug:\n    :syspath: {1}".format(
                name, dirname)
            try:
                res = rst2html(newstring, writer="rst", layout="sphinx")
                break
            except Exception as e:
                excs.append((name, e))

        if len(excs) == 2:
            mes = [str(e) for e in excs]
            raise Exception(
                "Unable to run autosignature:\n{0}".format("\n----\n".join(mes)))

        if len(excs) == 1:
            res = str(excs[0]) + "\n---\n" + res
        self.assertIn(
            "cpyquickhelper.io.stdchelper.cprint", res)
        self.assertIn("Displays a string on the standard output", res)
        self.assertIn("Signature", res)
        self.assertIn("name='cprint'", res)


if __name__ == "__main__":
    unittest.main()
