
cpyquickhelper.profiling
========================

.. contents::
    :local:

Capsules
++++++++

Not really related to profiling but maybe useful.

.. autosignature:: cpyquickhelper.profiling._event_profiler_c.dummy_capsule

.. autosignature:: cpyquickhelper.profiling._event_profiler_c.get_memory_content

C++ classes
+++++++++++

.. autosignature:: cpyquickhelper.profiling._event_profiler.CEventProfilerEvent

.. autosignature:: cpyquickhelper.profiling._event_profiler.CEventProfiler

Profiler
++++++++

.. autosignature:: cpyquickhelper.profiling.event_profiler.EventProfiler

.. autosignature:: cpyquickhelper.profiling.event_profiler.WithEventProfiler

.. _l-api-c-profiler:

C Profiler based on Python API
++++++++++++++++++++++++++++++

These functions partially implement an event profiler relying on Python C API.
The call to the function logging an event is much faster than a python call.

.. autosignature:: cpyquickhelper.profiling._event_profiler_c._profiling_start

.. autosignature:: cpyquickhelper.profiling._event_profiler_c._profiling_stop

.. autosignature:: cpyquickhelper.profiling._event_profiler_c._profiling_delete

.. autosignature:: cpyquickhelper.profiling._event_profiler_c._profiling_n_columns

.. autosignature:: cpyquickhelper.profiling._event_profiler_c._profiling_get_saved_maps

.. autosignature:: cpyquickhelper.profiling._event_profiler_c._profiling_log_event

.. autosignature:: cpyquickhelper.profiling._event_profiler_c._profiling_dump_and_clear

.. autosignature:: cpyquickhelper.profiling._event_profiler_c._profiling_register_pyinstance
