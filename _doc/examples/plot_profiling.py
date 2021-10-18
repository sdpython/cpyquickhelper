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

import time
from cpyquickhelper.profiling import EventProfiler, WithEventProfiler


def f1(t):
    time.sleep(t)


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

#########################################
# Plotting
# ++++++++

df['cst'] = 1
df['nev'] = df['cst'].cumsum()
ax = df[['time', 'nev']].plot(
    x='time', y='nev', title="Number of events / time")
for i in range(1, df.shape[0]):
    x = (df.loc[i - 1, 'time'] + df.loc[i, 'time']) / 2
    y = df.loc[i, 'nev']
    ax.text(x, y, df.loc[i - 1, 'name'], rotation=90, fontsize=8)


##########################################
# If the code raises an exception
# +++++++++++++++++++++++++++++++
#
# The program crashes if the profiled code raises an exception
# because the memory profiler is not restored before python extracts
# the call stack. New allocations still get logged into an object with
# was deleted. The exception must be caught or another syntax can be
# used.

wev = WithEventProfiler(clean_file_name=clean_name)
with wev:
    f4()

wev.report
