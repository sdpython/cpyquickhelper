
.. |gitlogo| image:: _static/git_logo.png
             :height: 20

.. image:: https://github.com/sdpython/cpyquickhelper/blob/master/_doc/sphinxdoc/source/phdoc_static/project_ico.png?raw=true
    :target: https://github.com/sdpython/cpyquickhelper/

cpyquickhelper: python + C++ in different ways
==============================================

.. image:: https://travis-ci.org/sdpython/cpyquickhelper.svg?branch=master
    :target: https://travis-ci.org/sdpython/cpyquickhelper
    :alt: Build status

.. image:: https://ci.appveyor.com/api/projects/status/sia7wxgjv8e1fi5a?svg=true
    :target: https://ci.appveyor.com/project/sdpython/cpyquickhelper
    :alt: Build Status Windows

.. image:: https://circleci.com/gh/sdpython/cpyquickhelper/tree/master.svg?style=svg
    :target: https://circleci.com/gh/sdpython/cpyquickhelper/tree/master

.. image:: https://dev.azure.com/xavierdupre3/cpyquickhelper/_apis/build/status/sdpython.cpyquickhelper
    :target: https://dev.azure.com/xavierdupre3/cpyquickhelper/

.. image:: https://badge.fury.io/py/cpyquickhelper.svg
    :target: https://pypi.org/project/cpyquickhelper/

.. image:: https://img.shields.io/badge/license-MIT-blue.svg
    :alt: MIT License
    :target: http://opensource.org/licenses/MIT

.. image:: https://requires.io/github/sdpython/cpyquickhelper/requirements.svg?branch=master
     :target: https://requires.io/github/sdpython/cpyquickhelper/requirements/?branch=master
     :alt: Requirements Status

.. image:: https://codecov.io/github/sdpython/cpyquickhelper/coverage.svg?branch=master
    :target: https://codecov.io/github/sdpython/cpyquickhelper?branch=master

.. image:: http://img.shields.io/github/issues/sdpython/cpyquickhelper.png
    :alt: GitHub Issues
    :target: https://github.com/sdpython/cpyquickhelper/issues

.. image:: nbcov.png
    :target: http://www.xavierdupre.fr/app/cpyquickhelper/helpsphinx/all_notebooks_coverage.html
    :alt: Notebook Coverage

**Links:** `github <https://github.com/sdpython/cpyquickhelper/>`_,
`documentation <http://www.xavierdupre.fr/app/cpyquickhelper/helpsphinx/index.html>`_,
:ref:`l-README`,
:ref:`blog <ap-main-0>`,
:ref:`l-issues-todolist`

This library explores various technics to use
:epkg:`C` or :epkg:`C++` functions in :epkg:`Python`.

.. toctree::
    :maxdepth: 1

    api/index
    i_ex
    all_notebooks
    blog/blogindex
    i_index

Links on how to use :epkg:`C` in :epkg:`Python` extension:

* `Extending Python with C or C++ <https://docs.python.org/3/extending/extending.html>`_
* `Building C and C++ Extensions with distutils <https://docs.python.org/3/extending/building.html#building-c-and-c-extensions-with-distutils>`_
* `Building Extensions For Python 3.5 <http://stevedower.id.au/blog/building-for-python-3-5/>`_

The produced code might be quite verbose.
One possible option is :epkg:`pybind11` which
speeds up the writing of functions binding :epkg:`C` objects
with :epkg:`Python` objects. The interface also speeds up
the development of functions taking array and matrices
from :epkg:`numpy` as inputs or outputs.

+----------------------+---------------------+---------------------+--------------------+------------------------+------------------------------------------------+
| :ref:`l-modules`     |  :ref:`l-functions` | :ref:`l-classes`    | :ref:`l-methods`   | :ref:`l-staticmethods` | :ref:`l-properties`                            |
+----------------------+---------------------+---------------------+--------------------+------------------------+------------------------------------------------+
| :ref:`modindex`      |  :ref:`l-EX2`       | :ref:`search`       | :ref:`l-license`   | :ref:`l-changes`       | :ref:`l-README`                                |
+----------------------+---------------------+---------------------+--------------------+------------------------+------------------------------------------------+
| :ref:`genindex`      |  :ref:`l-FAQ2`      | :ref:`l-notebooks`  | :ref:`l-HISTORY`   | :ref:`l-statcode`      | `Unit Test Coverage <coverage/index.html>`_    |
+----------------------+---------------------+---------------------+--------------------+------------------------+------------------------------------------------+
