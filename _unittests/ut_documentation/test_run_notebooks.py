# -*- coding: utf-8 -*-
"""
@brief      test log(time=33s)
"""

import os
import unittest
import platform
from pyquickhelper.loghelper import fLOG
from pyquickhelper.pycode import ExtTestCase
from pyquickhelper.ipythonhelper import test_notebook_execution_coverage
import cpyquickhelper


class TestRunNotebooksPython(ExtTestCase):

    @unittest.skipIf(platform.system().lower() == "darwin",
                     reason="no openmp")
    def test_run_notebooks_branching(self):
        fLOG(
            __file__,
            self._testMethodName,
            OutputPrint=__name__ == "__main__")

        import jyquickhelper as jyq
        self.assertNotEmpty(jyq)
        self.assertNotEmpty(cpyquickhelper)

        folder = os.path.join(os.path.dirname(__file__),
                              "..", "..", "_doc", "notebooks")
        test_notebook_execution_coverage(
            __file__, "branching", folder, 'cpyquickhelper', fLOG=fLOG)

    def test_run_notebooks_nobranching(self):
        fLOG(
            __file__,
            self._testMethodName,
            OutputPrint=__name__ == "__main__")

        import jyquickhelper as jyq
        self.assertNotEmpty(jyq)
        self.assertNotEmpty(cpyquickhelper)

        folder = os.path.join(os.path.dirname(__file__),
                              "..", "..", "_doc", "notebooks")
        test_notebook_execution_coverage(
            __file__, "", folder, 'cpyquickhelper', fLOG=fLOG,
            filter_name=lambda name: "branching" not in name)


if __name__ == "__main__":
    unittest.main()
