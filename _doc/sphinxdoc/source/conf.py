# -*- coding: utf-8 -*-
import sys
import os
import alabaster


sys.path.insert(0, os.path.abspath(os.path.join(os.path.split(__file__)[0])))
local_template = os.path.join(os.path.abspath(
    os.path.dirname(__file__)), "phdoc_templates")

from pyquickhelper.helpgen.default_conf import set_sphinx_variables
set_sphinx_variables(__file__, "cpyquickhelper", "Xavier Dupré", 2022,
                     "alabaster", alabaster.get_path(),
                     locals(), extlinks=dict(issue=(
                         'https://github.com/sdpython/cpyquickhelper/issues/%s',
                         'issue %s')),
                     title="cpyquickhelper", book=True)

blog_root = "http://www.xavierdupre.fr/app/cpyquickhelper/helpsphinx/"
html_css_files = ['my-styles.css']
html_logo = "phdoc_static/project_ico.png"
html_sidebars = {}

language = "en"
custom_preamble = """\n
\\newcommand{\\vecteur}[2]{\\pa{#1,\\dots,#2}}
\\newcommand{\\N}[0]{\\mathbb{N}}
\\newcommand{\\indicatrice}[1]{\\mathbf{1\\!\\!1}_{\\acc{#1}}}
\\usepackage[all]{xy}
\\newcommand{\\infegal}[0]{\\leqslant}
\\newcommand{\\supegal}[0]{\\geqslant}
\\newcommand{\\ensemble}[2]{\\acc{#1,\\dots,#2}}
\\newcommand{\\fleche}[1]{\\overrightarrow{ #1 }}
\\newcommand{\\intervalle}[2]{\\left\\{#1,\\cdots,#2\\right\\}}
\\newcommand{\\loinormale}[2]{{\\cal N}\\pa{#1,#2}}
\\newcommand{\\independant}[0]{\\;\\makebox[3ex]{\\makebox[0ex]{\\rule[-0.2ex]{3ex}{.1ex}}\\!\\!\\!\\!\\makebox[.5ex][l]{\\rule[-.2ex]{.1ex}{2ex}}\\makebox[.5ex][l]{\\rule[-.2ex]{.1ex}{2ex}}} \\,\\,}
\\newcommand{\\esp}{\\mathbb{E}}
\\newcommand{\\var}{\\mathbb{V}}
\\newcommand{\\pr}[1]{\\mathbb{P}\\pa{#1}}
\\newcommand{\\loi}[0]{{\\cal L}}
\\newcommand{\\vecteurno}[2]{#1,\\dots,#2}
\\newcommand{\\norm}[1]{\\left\\Vert#1\\right\\Vert}
\\newcommand{\\dans}[0]{\\rightarrow}
\\newcommand{\\partialfrac}[2]{\\frac{\\partial #1}{\\partial #2}}
\\newcommand{\\partialdfrac}[2]{\\dfrac{\\partial #1}{\\partial #2}}
\\newcommand{\\loimultinomiale}[1]{{\\cal M}\\pa{#1}}
\\newcommand{\\trace}[1]{tr\\pa{#1}}
\\newcommand{\\abs}[1]{\\left|#1\\right|}
"""
# \\usepackage{eepic}

imgmath_latex_preamble += custom_preamble
latex_elements['preamble'] += custom_preamble
mathdef_link_only = True

epkg_dictionary.update({
    'AVX': 'https://en.wikipedia.org/wiki/Advanced_Vector_Extensions',
    'BLAS': 'http://www.netlib.org/blas/explore-html',
    'C': 'https://en.wikipedia.org/wiki/C_(programming_language)',
    'C++': 'https://en.wikipedia.org/wiki/C%2B%2B',
    'cython': 'https://cython.org/',
    'DataFrame':
        'https://pandas.pydata.org/pandas-docs/stable/generated/pandas.DataFrame.html',
    'ExtensionArray':
        'https://pandas.pydata.org/pandas-docs/stable/generated/'
        'pandas.api.extensions.ExtensionArray.html',
    'JIT': 'https://en.wikipedia.org/wiki/Just-in-time_compilation',
    'LAPACK': 'http://www.netlib.org/lapack/explore-html',
    'llvmlite': 'https://llvmlite.readthedocs.io/en/latest/index.html',
    'nuitka': 'http://nuitka.net/',
    'numba': 'https://numba.pydata.org/',
    'palanteer': 'https://github.com/dfeneyrou/palanteer',
    'PandasArray':
        'https://pandas.pydata.org/docs/reference/api/'
        'pandas.arrays.PandasArray.html',
    'pybind11': 'https://github.com/pybind/pybind11',
    'pyinstrument': 'https://github.com/joerick/pyinstrument',
    'PyMem_SetAllocator':
        'https://docs.python.org/3/c-api/memory.html#c.PyMem_SetAllocator',
    'Pythran': 'https://pythran.readthedocs.io/en/latest/',
    'pythran': 'https://pythran.readthedocs.io/en/latest/',
    'Series':
        'https://pandas.pydata.org/pandas-docs/stable/'
        'generated/pandas.Series.html',
    'SSE': 'https://fr.wikipedia.org/wiki/Streaming_SIMD_Extensions',
})
