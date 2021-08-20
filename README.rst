
.. image:: https://github.com/sdpython/cpyquickhelper/blob/master/_doc/sphinxdoc/source/phdoc_static/project_ico.png?raw=true
    :target: https://github.com/sdpython/cpyquickhelper/

.. _l-README:

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

.. image:: http://www.xavierdupre.fr/app/cpyquickhelper/helpsphinx/_images/nbcov.png
    :target: http://www.xavierdupre.fr/app/cpyquickhelper/helpsphinx/all_notebooks_coverage.html
    :alt: Notebook Coverage

.. image:: https://img.shields.io/github/repo-size/sdpython/cpyquickhelper
    :target: https://github.com/sdpython/cpyquickhelper/
    :alt: size

*cpyquickhelper* is a template to create a module with
C functions in different ways. It implements function
*measure_time*:

::

    from cpyquickhelper.numbers import measure_time
    from math import cos

    res = measure_time("cos(x)", context=dict(cos=cos, x=5.))
    print(res)

::

    {'average': 3.909366205334663e-06, 'deviation': 6.238702219064397e-07,
     'min_exec': 3.635883331298828e-06, 'max_exec': 5.776062607765198e-06,
     'repeat': 10, 'number': 50, 'context_size': 240}

On Windows, the following exception might happen:

::

    LINK : fatal error LNK1158: impossible d'exécuter 'rc.exe'

Which might be resolved with the following line before building it:

::

    set PATH=%PATH%;C:\Program Files (x86)\Windows Kits\10\bin\10.0.16299.0\x64

**Links:**

* `GitHub/cpyquickhelper <https://github.com/sdpython/cpyquickhelper/>`_
* `documentation <http://www.xavierdupre.fr/app/cpyquickhelper/helpsphinx/index.html>`_
* `Blog <http://www.xavierdupre.fr/app/cpyquickhelper/helpsphinx/blog/main_0000.html#ap-main-0>`_
