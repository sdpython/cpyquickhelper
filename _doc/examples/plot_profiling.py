# -*- coding: utf-8 -*-
"""

.. _l-example-event-profling:

Event Profiling
===============

Many profilers propose to see how long a program stays
in a function (see :mod:`cProfile`, :epkg:`pyinstrument`,
:epkg:`palanteer`). This one logs events. It merges function calls
and memory allocations.

.. contents::
    :local:

The simple function to look at
++++++++++++++++++++++++++++++
"""

from time import sleep
from cpyquickhelper.profiling import EventProfiler


def f1(t):
    sleep(t)


def f2():
    f1(0.1)


def f3():
    li = [0 for i in range(0, 100)]
    f1(0.2)
    return li


def f4():
    f2()
    f3()

#########################################
# The profiling
# +++++++++++++
#
# It works the same way.


ev = EventProfiler()
ev.start()
f4()
ev.stop()

#########################################
# The report
# ++++++++++


def clean_name(name):
    return "/".join(name.replace("\\", "/").split('/')[-3:])


df = ev.retrieve_results(clean_file_name=clean_name)
df
