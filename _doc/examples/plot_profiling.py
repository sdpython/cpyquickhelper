# -*- coding: utf-8 -*-
"""

.. _l-example-event-profling:

EventProfiler
=============

.. index:: event, profiling, profiler

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
import inspect
from cpyquickhelper.profiling import (
    EventProfiler, WithEventProfiler)
from cpyquickhelper.profiling.event_profiler import EventProfilerDebug
from cpyquickhelper.profiling._event_profiler_c import (
    _profiling_start, _profiling_stop,
    _profiling_log_event, _profiling_delete)


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

##########################################
# Profiling cost
# ++++++++++++++
#
# Logging an events takes a few microseconds so
# A class not profiling but still logging is available
# to measure the cost of the profiling.


def measure_implementation(impl):
    N = 100000

    if impl == 'python':
        ev = EventProfilerDebug(impl=impl)
        ev.start()
        begin = time.perf_counter()
        for _ in range(N):
            ev.log_event(inspect.currentframe(), 'call', None)
            ev.log_event(inspect.currentframe(), 'return', None)
        end = time.perf_counter()
        ev.stop()
    elif impl == 'c':
        _profiling_start(10000000, True)
        begin = time.perf_counter()
        for _ in range(N):
            _profiling_log_event(inspect.currentframe(), 'call', None)
            _profiling_log_event(inspect.currentframe(), 'return', None)
        end = time.perf_counter()
        _profiling_delete()
        _profiling_stop()
    else:
        ev = EventProfilerDebug(impl=impl)
        ev.start()
        begin = time.perf_counter()
        for _ in range(N):
            ev._buffer.c_log_event(
                inspect.currentframe(), 'call', None)
            ev._buffer.c_log_event(
                inspect.currentframe(), 'return', None)
        end = time.perf_counter()
        ev.stop()

    duration = end - begin
    msg = "%s: %1.6f microsecond" % (impl, duration / N * 1e6)
    return msg


print(measure_implementation('python'))
print(measure_implementation('pybind11'))
print(measure_implementation('c'))


###########################################
# The fastest implementation uses Python C API and can be enabled
# by using `EventProfiler(impl='c')`. This implementation
# relies on function from `l-api-c-profiler`.
#
# EventProfiler with C implementation
# +++++++++++++++++++++++++++++++++++

wev = WithEventProfiler(clean_file_name=clean_name, impl='c')
with wev:
    f4()

wev.report
