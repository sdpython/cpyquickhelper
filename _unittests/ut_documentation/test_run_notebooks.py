# -*- coding: utf-8 -*-
"""
@brief      test log(time=33s)
"""

import os
import unittest
from pyquickhelper.loghelper import fLOG
from pyquickhelper.pycode import ExtTestCase
from pyquickhelper.ipythonhelper import test_notebook_execution_coverage

import cpyquickhelper


class TestRunNotebooksPython(ExtTestCase):

    def test_run_notebooks(self):
        fLOG(
            __file__,
            self._testMethodName,
            OutputPrint=__name__ == "__main__")

        import jyquickhelper as jyq
        self.assertNotEmpty(jyq)
        self.assertNotEmpty(cpyquickhelper)

        folder = os.path.join(os.path.dirname(__file__),
                              "..", "..", "_doc", "notebooks")
        test_notebook_execution_coverage(__file__, "", folder,
                                         'cpyquickhelper', fLOG=fLOG)


if __name__ == "__main__":
    unittest.main()
