
.. _l-README:

README
======

.. image:: https://travis-ci.org/sdpython/cpyquickhelper.svg?branch=master
    :target: https://travis-ci.org/sdpython/cpyquickhelper
    :alt: Build status

.. image:: https://ci.appveyor.com/api/projects/status/sia7wxgjv8e1fi5a?svg=true
    :target: https://ci.appveyor.com/project/sdpython/cpyquickhelper
    :alt: Build Status Windows

.. image:: https://circleci.com/gh/sdpython/cpyquickhelper/tree/master.svg?style=svg
    :target: https://circleci.com/gh/sdpython/cpyquickhelper/tree/master

.. image:: https://dev.azure.com/xavierdupre3/pyquickhelper/_apis/build/status/sdpython.pyquickhelper
    :target: https://dev.azure.com/xavierdupre3/pyquickhelper/

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

.. image:: https://badge.waffle.io/sdpython/cpyquickhelper.png?label=to%20do&title=to%20do
    :alt: Waffle
    :target: https://waffle.io/cpyquickhelper/cpyquickhelper

.. image:: http://www.xavierdupre.fr/app/cpyquickhelper/helpsphinx/_images/nbcov.png
    :target: http://www.xavierdupre.fr/app/cpyquickhelper/helpsphinx/all_notebooks_coverage.html
    :alt: Notebook Coverage

*cpyquickhelper* is a template to create a module with
C functions.

On Windows, the following exception might happen:

::

    LINK : fatal error LNK1158: impossible d'ex�cuter 'rc.exe'

Which might be resolved with the following line before building it:

::

    set PATH=%PATH%;C:\Program Files (x86)\Windows Kits\10\bin\10.0.16299.0\x64

**Links:**

* `GitHub/cpyquickhelper <https://github.com/sdpython/cpyquickhelper/>`_
* `documentation <http://www.xavierdupre.fr/app/cpyquickhelper/helpsphinx/index.html>`_
* `Blog <http://www.xavierdupre.fr/app/cpyquickhelper/helpsphinx/blog/main_0000.html#ap-main-0>`_
