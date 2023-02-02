
.. |gitlogo| image:: _static/git_logo.png
             :height: 20

.. image:: https://github.com/sdpython/cpyquickhelper/blob/master/_doc/sphinxdoc/source/_static/project_ico.png?raw=true
    :target: https://github.com/sdpython/cpyquickhelper/

cpyquickhelper: python + C++ in different ways
==============================================

.. image:: https://travis-ci.com/sdpython/cpyquickhelper.svg?branch=master
    :target: https://app.travis-ci.com/github/sdpython/cpyquickhelper
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

.. image:: https://codecov.io/github/sdpython/cpyquickhelper/coverage.svg?branch=master
    :target: https://codecov.io/github/sdpython/cpyquickhelper?branch=master

.. image:: http://img.shields.io/github/issues/sdpython/cpyquickhelper.png
    :alt: GitHub Issues
    :target: https://github.com/sdpython/cpyquickhelper/issues

.. image:: nbcov.png
    :target: http://www.xavierdupre.fr/app/cpyquickhelper/helpsphinx/all_notebooks_coverage.html
    :alt: Notebook Coverage

.. image:: https://pepy.tech/badge/cpyquickhelper
    :target: https://pypi.org/project/cpyquickhelper/
    :alt: Downloads

.. image:: https://img.shields.io/github/forks/sdpython/cpyquickhelper.svg
    :target: https://github.com/sdpython/cpyquickhelper/
    :alt: Forks

.. image:: https://img.shields.io/github/stars/sdpython/cpyquickhelper.svg
    :target: https://github.com/sdpython/cpyquickhelper/
    :alt: Stars

.. image:: https://img.shields.io/github/repo-size/sdpython/cpyquickhelper
    :target: https://github.com/sdpython/cpyquickhelper/
    :alt: size

**Links:** `github <https://github.com/sdpython/cpyquickhelper/>`_,
`documentation <http://www.xavierdupre.fr/app/cpyquickhelper/helpsphinx/index.html>`_,
:ref:`l-README`,
:ref:`blog <ap-main-0>`

This library explores various technics to use
:epkg:`C` or :epkg:`C++` functions in :epkg:`Python`. The code
shows examples on how to link with :epkg:`pybind11` or :epkg:`cython`,
how to call :epkg:`BLAS` or :epkg:`LAPACK` function from *cython* code
available through :epkg:`scipy`.
It also implement an event profiler based on the direct Python API
(see :ref:`l-example-event-profling`).

.. toctree::
    :maxdepth: 1

    api/index
    i_ex
    gyexamples/index
    all_notebooks
    blog/blogindex
    i_index

The following links mixing :epkg:`C` and :epkg:`Python` may be useful:
`Extending Python with C or C++
<https://docs.python.org/3/extending/extending.html>`_,
`Building C and C++ Extensions with distutils
<https://docs.python.org/3/extending/building.html#building-c-and-c-extensions-with-distutils>`_.

Interesting features:
:func:`edit_distance_string <cpyquickhelper.algorithms.edit_distance.edit_distance_string>`,
:class:`PyCContainer <cpyquickhelper.examples.custom_container_python.PyCContainer>`,
:func:`get_simd_available_option <cpyquickhelper.numbers.cbenchmark.get_simd_available_option>`,
:func:`check_speed <cpyquickhelper.numbers.speed_measure.check_speed>`,
:func:`measure_time <cpyquickhelper.numbers.speed_measure.measure_time>`,
:class:`EventProfiler <cpyquickhelper.profiling.event_profiler.EventProfiler>`.

+----------------------+---------------------+---------------------+--------------------+------------------------+------------------------------------------------+
| :ref:`l-modules`     |  :ref:`l-functions` | :ref:`l-classes`    | :ref:`l-methods`   | :ref:`l-staticmethods` | :ref:`l-properties`                            |
+----------------------+---------------------+---------------------+--------------------+------------------------+------------------------------------------------+
| :ref:`modindex`      |  :ref:`l-EX2`       | :ref:`search`       | :ref:`l-license`   | :ref:`l-changes`       | :ref:`l-README`                                |
+----------------------+---------------------+---------------------+--------------------+------------------------+------------------------------------------------+
| :ref:`genindex`      |  :ref:`l-FAQ2`      | :ref:`l-notebooks`  | :ref:`l-HISTORY`   | :ref:`l-statcode`      | `Unit Test Coverage <coverage/index.html>`_    |
+----------------------+---------------------+---------------------+--------------------+------------------------+------------------------------------------------+
