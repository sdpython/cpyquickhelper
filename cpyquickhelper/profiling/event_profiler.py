"""
@file
@brief Profiling class.
"""
import sys
import inspect
import numpy
import pandas
from ._event_profiler import CEventProfiler  # pylint: disable=E0611


class EventProfiler:
    """
    This profiler profiles both memory and function calls.
    It stores events and produces a timeline.
    See :ref:`l-example-event-profling` for an example
    of use.

    :param size: size of the buffer to store events
    :param impl: different implementation of the same
        function (`'python'`, `'pybind11'`)

    The profiler stores every event about function calls and returns,
    and memory allocation. It does not give the time spent in every function
    but a timeline with the series of events which occured.
    There are two pieces:

    * A C++ class which logs every event into a bug buffer,
      every event is described by five int64 (id_frame, id_arg, event,
      timestamp, value1, value2).
    * A python class which wraps the first one.

    Function calls are caught by using :func:`sys.setprofile`.
    Memory calls are caught by using :epkg:`PyMem_SetAllocator`.
    Once the profiled code has ended, results are returned by
    @see meth retrieve_results. This function extends the information
    stored in the C++ class with file names, function names, line numbers,
    memory deallocated.

    .. note::
        The program crashes if the profiled code raises an exception.
        The default memory allocator is not restored and any allocation
        is logged into a buffer which was deleted. The exception must be
        caught or class @see cl WithEventProfiler must be used.

    About parameter *impl*, the question is related to the implementation
    of method :meth:`log_event
    <cpyquickhelper.profiling.event_profiler.EventProfiler.log_event>`.
    A call to the python implementation takes 2-3 microseconds, a call
    to the :epkg:`pybind11` implementation takes 1-2 microseconds.
    """

    _event_mapping = {
        'call': 1,
        'return': 2,
        'c_call': 3,
        'c_return': 4,
        'exception': 5,
        'c_exception': 6,
        'profiler_call': 10,
        'profiler_return': 11,
        'profiler_start': 100,
        'profiler_sort': 101,
        'malloc': 1000,
        'calloc': 1001,
        'realloc': 1002,
        'free': 1003,
        'realloc_free': 1004,
    }

    def __init__(self, size=1000000, impl='python'):
        self._started = False
        self._prof = None
        self._frames = {-1: inspect.currentframe()}
        self._args = {-1: None, 0: None, id(None): None}
        self._buffer = CEventProfiler(size)
        self._events = []
        self._impl = impl
        self._copy_cache = False
        self._cache_copy = numpy.empty((size, self._buffer.n_columns()),
                                       dtype=numpy.int64)
        if impl == 'pybind11':
            # self._buffer.register_empty_cache(self._empty_cache)
            self._buffer.register_pyinstance(self)

    @property
    def n_columns(self):
        """
        Returns the number of stored informations by the profiler
        in memory. This corresponds to the number of columns returned
        by @see meth retrieve_raw_results.
        """
        return self._buffer.n_columns()

    def start(self):
        """
        Starts the profiling.
        """
        if self._started:
            raise RuntimeError(
                "The profiler was already started. It cannot be done again.")
        self._frames[0] = inspect.currentframe()
        self._started = True
        self._copy_cache = False
        self._setup_profiler()
        self._buffer.log_event(-1, -1, 100, 0, 0)
        self._buffer.start()

    def stop(self):
        """
        Stops the profiling.
        """
        if not self._started:
            raise RuntimeError(
                "The profiler was not started. It must be done first.")
        self._restore_profiler()
        self._buffer.stop()
        self._buffer.log_event(-1, -1, 101, 0, 0)
        if self._impl == 'pybind11':
            map_frame, map_arg = self._buffer.get_saved_maps()
            self._frames.update(map_frame)
            self._args.update(map_arg)
        self._buffer.delete()
        self._started = False

    def _setup_profiler(self):
        """
        This relies on :func:`sys.setprofile` and :func:`sys.getprofile`.
        """
        self._prof = sys.getprofile()
        if self._impl == 'python':
            sys.setprofile(self.log_event)
        elif self._impl == 'pybind11':
            sys.setprofile(self._buffer.c_log_event)
        else:
            raise ValueError(
                "Unexpected value for impl=%r." % self._impl)

    def _restore_profiler(self):
        """
        This relies on :func:`sys.setprofile` and :func:`sys.getprofile`.
        """
        sys.setprofile(self._prof)
        self._prof = None

    def log_event(self, frame, event, arg):
        """
        Logs an event in the database.

        :param frame: (frame), see :mod:`inspect`
        :param event: (str)
            kind of event
        :param arg: None or...
        """
        idf = id(frame)
        if idf not in self._frames:
            self._frames[idf] = frame
        if type(arg) == type(open):  # pylint: disable=C0123
            ida = id(arg)
        else:
            ida = id(None)
        if ida not in self._args:
            self._args[ida] = arg
        r = self._buffer.log_event(
            idf, ida, EventProfiler._event_mapping[event], 0, 0)
        if not r:
            self._empty_cache()

    def _empty_cache(self):
        """
        Empties the cache. This function logs a couple of
        events. The cache must contains enough place to
        log them.
        """
        if self._copy_cache:
            raise RuntimeError(
                "Profiling cache being copied. Increase the size of the cache.")
        self._copy_cache = True
        size = self._buffer.dump_and_clear(self._cache_copy, True)
        # We hope here this function will not be called by another
        # thread. That would another thread was able to fill
        # the cache while it is being copied.
        self._events.append(self._cache_copy[:size].copy())
        self._copy_cache = False
        return size

    def retrieve_raw_results(self, empty_cache=True):
        """
        Retrieves the raw results. Difficult to interpret.

        :param empty_cache: retrives the data from the C++ container and
            stores it in a numpy array before
        :return: numpy array
        """
        if empty_cache:
            self._empty_cache()
        res = numpy.vstack(self._events)
        # Filling information about the size of freed buffer.
        # id_frame, id_arg, event, time, value1, value2
        memo = {}
        for i in range(res.shape[0]):
            event = res[i, 2]
            if event < 1000:
                continue
            ptr = res[i, 4]
            if event <= 1002:
                memo[ptr] = res[i, 5]
            else:
                if ptr not in memo:
                    # The system is freeing a non-existing pointer.
                    res[i, 5] = 0
                else:
                    res[i, 5] = -memo[ptr]
                    del memo[ptr]
        return res

    def _choose(self, frame, arg, f_back=False):
        """
        Returns a string which represents the called function.
        When the function is built-in, frame contains the function
        calling the built-in, *arg* is then a pointer on this
        function. This method returns its name.
        """
        if arg is None:
            if f_back:
                if frame.f_back is None:
                    name = ''
                else:
                    name = frame.f_back.f_code.co_name
            else:
                name = frame.f_code.co_name
            return name
        try:
            return arg.__qualname__
        except AttributeError:
            return arg.__class__.__name__

    def _choose_mod(self, frame, arg, clean_name, f_back=False):
        """
        Returns a string which represents the module of the called function.
        When the function is built-in, frame contains the function
        calling the built-in, *arg* is then a pointer on this
        function. This method returns its module.
        """
        if arg is None:
            if f_back:
                if frame.f_back is None:
                    name = ''
                else:
                    name = frame.f_back.f_code.co_filename
            else:
                name = frame.f_code.co_filename
            return clean_name(name)
        try:
            return arg.__module__
        except AttributeError:
            return arg.__class__.__module__

    def retrieve_results(self, empty_cache=True, clean_file_name=None):
        """
        Retrieves the raw results. Difficult to interpret.

        :param empty_cache: retrives the data from the C++ container and
            stores it in a numpy array before
        :param clean_file_name: function which cleans the file name
        :return: numpy array
        """
        def _identity_(x):
            return x

        if clean_file_name is None:
            clean_file_name = _identity_
        rev_event_mapping = {
            v: k for k, v in EventProfiler._event_mapping.items()}
        res = self.retrieve_raw_results(empty_cache=empty_cache)
        df = pandas.DataFrame(
            res[:, 3:], columns=['time', 'value1', 'value2'])
        df['event'] = list(map(lambda v: rev_event_mapping[v], res[:, 2]))
        frames = list(map(lambda v: self._frames[v], res[:, 0]))
        args = list(map(lambda v: self._args[v], res[:, 1]))

        df['name'] = list(self._choose(frame, arg)
                          for frame, arg in zip(frames, args))
        df['mod'] = list(self._choose_mod(frame, arg, clean_file_name)
                         for frame, arg in zip(frames, args))
        df['lineno'] = list(frame.f_code.co_firstlineno for frame in frames)
        df['from_name'] = list(
            self._choose(frame, arg, True) for frame, arg in zip(frames, args))
        df['from_mod'] = list(
            self._choose_mod(frame, None, clean_file_name, f_back=True)
            for frame in frames)
        df['from_line'] = list(
            (-1 if frame.f_back is None else frame.f_back.f_lineno)
            for frame in frames)
        return df


class WithEventProfiler:
    """
    Enables profiling with the following syntax:

    ::

        prof = WithEventProfiler()
        with prof:
            # code to profile
            # may raise an exception

    The class restores the memory allocator and stops logging any event
    even if an exception was raised.

    :param size: size of the buffer to store events
    :param impl: different implementation of the same
        function (`'python'`, `'pybind11'`)
    :param clean_file_name: function uses to clean or shorten the file name
        saved in the report.
    """

    def __init__(self, size=1000000, impl='python', clean_file_name=None):
        self.size = size
        self.clean_file_name = clean_file_name
        self.report_ = None
        self.prof_ = None
        self.impl = impl

    def __enter__(self):
        self.prof_ = EventProfiler(size=self.size, impl=self.impl)
        self.prof_.start()

    def __exit__(self, typ, value, traceback):
        self.prof_.stop()
        self.report_ = self.prof_.retrieve_results(
            clean_file_name=self.clean_file_name)

    @property
    def report(self):
        """Returns the profiling report as a dataframe."""
        return self.report_


class EventProfilerDebug(EventProfiler):
    """
    One class to measure time wasted by profiling.
    """

    def start(self):
        """
        Starts the profiling without enabling it.
        """
        if self._started:
            raise RuntimeError(
                "The profiler was already started. It cannot be done again.")
        self._frames[0] = inspect.currentframe()
        self._started = True
        self._copy_cache = False
        self._buffer.log_event(-1, -1, 100, 0, 0)

    def stop(self):
        """
        Stops the unstarted profiling.
        """
        if not self._started:
            raise RuntimeError(
                "The profiler was not started. It must be done first.")
        self._buffer.log_event(-1, -1, 101, 0, 0)
        self._started = False
