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

    def __init__(self, size=1000000):
        self._started = False
        self._prof = None
        self._frames = {-1: inspect.currentframe()}
        self._args = {-1: None, 0: None, id(None): None}
        self._buffer = CEventProfiler(size)
        self._events = []

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
        self._buffer.stop()
        self._buffer.log_event(-1, -1, 101, 0, 0)
        self._restore_profiler()
        self._started = False

    def log_event(self, frame, event, arg):
        """
        Logs an event in the database.

        :param frame: (frame), see :mod:`inspect`
        :param event: (str)
            kind of event
        :param value1: (int)
            additional value to the event
        :param value2: (int)
            additional value to the event
        :param arg: None or...
        """
        idf = id(frame)
        if idf not in self._frames:
            self._frames[idf] = frame
        if type(arg) == type(open):
            ida = id(arg)
        else:
            ida = id(None)
        if ida not in self._args:
            self._args[ida] = arg
        r = self._buffer.log_event(
            idf, ida, EventProfiler._event_mapping[event], 0, 0)
        if not r:
            self._empty_cache()

    def _setup_profiler(self):
        """
        This relies on :func:`sys.setprofile` and :func:`sys.getprofile`.
        """
        self._prof = sys.getprofile()
        sys.setprofile(self.log_event)

    def _restore_profiler(self):
        """
        This relies on :func:`sys.setprofile` and :func:`sys.getprofile`.
        """
        sys.setprofile(self._prof)
        self._prof = None

    def _empty_cache(self):
        """
        Empties the cache.
        """
        self._buffer.log_event(-1, -1, 10, 0, 0)
        self._buffer.lock()
        size = len(self._buffer)
        if size == 0:
            self._buffer.unlock()
            self._buffer.log_event(-1, -1, 11, 0, 0)
            return 0
        store = numpy.empty(
            (size, self._buffer.n_columns()), dtype=numpy.int64)
        self._buffer.dump(store, False)
        self._buffer.clear(False)
        self._buffer.unlock()

        self._events.append(store)
        self._buffer.log_event(-1, -1, 11, store.shape[0], 0)
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
        return arg.__qualname__

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
        return arg.__module__

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
    :param clean_file_name: function uses to clean or shorten the file name
        saved in the report.
    """

    def __init__(self, size=1000000, clean_file_name=None):
        self.size = size
        self.clean_file_name = clean_file_name
        self.report_ = None
        self.prof_ = None

    def __enter__(self):
        self.prof_ = EventProfiler(size=self.size)
        self.prof_.start()

    def __exit__(self, typ, value, traceback):
        self.prof_.stop()
        self.report_ = self.prof_.retrieve_results(
            clean_file_name=self.clean_file_name)

    @property
    def report(self):
        """Returns the profiling report as a dataframe."""
        return self.report_
