
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

.. image:: https://pepy.tech/badge/cpyquickhelper/month
    :target: https://pepy.tech/project/cpyquickhelper/month
    :alt: Downloads

.. image:: https://img.shields.io/github/repo-size/sdpython/cpyquickhelper
    :target: https://github.com/sdpython/cpyquickhelper/
    :alt: size

.. image:: https://img.shields.io/github/forks/sdpython/cpyquickhelper.svg
    :target: https://github.com/sdpython/cpyquickhelper/
    :alt: Forks

.. image:: https://img.shields.io/github/stars/sdpython/cpyquickhelper.svg
    :target: https://github.com/sdpython/cpyquickhelper/
    :alt: Stars

*cpyquickhelper* is a template to create a module with
C functions in different ways. It implements function
*measure_time*:

::

    from cpyquickhelper.numbers import measure_time
    from math import cos

    res = measure_time(lambda: cos(0.5))
    print(res)

::

    {'average': 3.909366205334663e-06, 'deviation': 6.238702219064397e-07,
     'min_exec': 3.635883331298828e-06, 'max_exec': 5.776062607765198e-06,
     'repeat': 10, 'number': 50, 'context_size': 240}

It also implements an event profiler: it logs the timestamp
for every event such as functions call or returns, memory allocations.

::

    import io
    import numpy
    from cpyquickhelper.profiling import EventProfiler

    def custom_array(N):
        a = numpy.zeros((N, N))
        a[:, 0] = 1
        a[0, :] = 1
        return a

    ev = EventProfiler(impl='c')
    ev.start()

    custom_array(3)

    ev.stop()

    df = ev.retrieve_results()  # DataFrame
    st = io.StringIO()
    df.to_csv(st, index=False)
    print(st.getvalue().replace("\r", ""))

::

    time,value1,value2,event,name,mod,lineno,from_name,from_mod,from_line
    822467345556400,0,0,return,_setup_profiler,cpyquickhelper/profiling/event_profiler.py,153,start,cpyquickhelper/profiling/event_profiler.py,126
    822467345566700,0,0,c_call,_profiling_register_pyinstance,cpyquickhelper.profiling._event_profiler_c,109,_profiling_register_pyinstance,k.py,19
    822467345569000,0,0,c_return,_profiling_register_pyinstance,cpyquickhelper.profiling._event_profiler_c,109,_profiling_register_pyinstance,k.py,19
    822467345569700,0,0,return,start,cpyquickhelper/profiling/event_profiler.py,109,<module>,k.py,19
    822467345575100,0,0,call,custom_array,k.py,5,<module>,k.py,19
    822467345579600,0,0,c_call,zeros,numpy,5,zeros,k.py,19
    822467345584300,2698130437280,32,malloc,start,cpyquickhelper/profiling/event_profiler.py,109,<module>,k.py,19
    822467345590500,0,0,c_return,zeros,numpy,5,zeros,k.py,19
    822467345598200,0,0,free,start,cpyquickhelper/profiling/event_profiler.py,109,<module>,k.py,19
    822467345600400,0,0,free,start,cpyquickhelper/profiling/event_profiler.py,109,<module>,k.py,19
    822467345600900,0,0,return,ndarray,numpy,5,ndarray,k.py,19
    822467345603200,0,0,call,stop,cpyquickhelper/profiling/event_profiler.py,128,<module>,k.py,19
    822467345604900,0,0,call,_restore_profiler,cpyquickhelper/profiling/event_profiler.py,168,stop,cpyquickhelper/profiling/event_profiler.py,151
    822467345605600,0,0,c_call,setprofile,sys,168,setprofile,cpyquickhelper/profiling/event_profiler.py,151

**Links:**

* `GitHub/cpyquickhelper <https://github.com/sdpython/cpyquickhelper/>`_
* `documentation <http://www.xavierdupre.fr/app/cpyquickhelper/helpsphinx/index.html>`_
* `Blog <http://www.xavierdupre.fr/app/cpyquickhelper/helpsphinx/blog/main_0000.html#ap-main-0>`_
