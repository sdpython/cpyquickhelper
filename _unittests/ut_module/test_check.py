"""
@brief      test log(time=0s)
"""
import unittest
import io
from contextlib import redirect_stdout
from pyquickhelper.pycode import ExtTestCase
from cpyquickhelper import check, _setup_hook


class TestCheck(ExtTestCase):
    """Test style."""

    def test_check(self):
        check()

    def test_hook(self):
        buf = io.StringIO()
        with redirect_stdout(buf):
            _setup_hook(True)
        v = buf.getvalue()
        self.assertIn("Success: _setup_hook", v)


if __name__ == "__main__":
    unittest.main()
