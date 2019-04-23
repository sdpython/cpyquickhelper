"""
@brief      test log(time=0s)
"""
import os
import unittest
from pyquickhelper.loghelper import fLOG
from pyquickhelper.pycode import check_pep8, ExtTestCase


class TestCodeStyle(ExtTestCase):
    """Test style."""

    def test_style_src(self):
        thi = os.path.abspath(os.path.dirname(__file__))
        src_ = os.path.normpath(os.path.join(thi, "..", "..", "src"))
        check_pep8(src_, fLOG=fLOG,
                   pylint_ignore=('C0103', 'C1801', 'R0201', 'R1705', 'W0108', 'W0613',
                                  'W0212'),
                   skip=["No name 'kill_thread' in module 'src.cpyquickhelper.parallel.threader'",
                         "capture' in module 'src.cpyquickhelper.io.stdchelper'",
                         "No name 'WeightedDouble' in module 'src.cpyquickhelper.numbers.weighted_number'",
                         "No name 'WeightedFloat' in module 'src.cpyquickhelper.numbers.weighted_number'",
                         ])

    def test_style_test(self):
        thi = os.path.abspath(os.path.dirname(__file__))
        test = os.path.normpath(os.path.join(thi, "..", ))
        check_pep8(test, fLOG=fLOG, neg_pattern="temp_.*",
                   pylint_ignore=('C0103', 'C1801', 'R0201', 'R1705', 'W0108', 'W0613',
                                  'C0111', 'W0703'),
                   skip=["No name 'cprint' in module 'src.cpyquickhelper.io.stdchelper'",
                         "test_weighted_dataframe.py:29: E0611",
                         ])


if __name__ == "__main__":
    unittest.main()
