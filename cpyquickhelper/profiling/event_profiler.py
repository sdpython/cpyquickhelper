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
    :param filename: file to dump the results, None, to keep it in memory

    The profiler stores every event about function calls and returns,
    and memory allocation. It does not give the time spent in every function
    but a timeline with the series of events which occured.
    There are two pieces:

    * A C++ class which logs every event into a bug buffer,
      every event is described by five int64 (id_frame, event, timestamp,
      value1, value2).
    * A python class which wraps the first one.

    Function calls are caught by using :func:`sys.setprofile`.
    Memory calls are caught by using :epkg:`PyMem_SetAllocator`.
    Once the profiled code has ended, results are returned by
    @see meth retrieve_results. This function extends the information
    stored in the C++ class with file names, function names, line numbers,
    memory deallocated.
    """

    _event_mapping = {
        'call': 1,
        'return': 2,
        'c_call': 3,
        'c_return': 4,
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
        self._buffer.log_event(-1, 100, 0, 0)
        self._buffer.start()

    def stop(self):
        """
        Stops the profiling.
        """
        if not self._started:
            raise RuntimeError(
                "The profiler was not started. It must be done first.")
        self._buffer.stop()
        self._buffer.log_event(-1, 101, 0, 0)
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
        r = self._buffer.log_event(
            idf, EventProfiler._event_mapping[event], 0, 0)
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
        self._buffer.log_event(-1, 10, 0, 0)
        self._buffer.lock()
        size = len(self._buffer)
        if size == 0:
            self._buffer.unlock()
            self._buffer.log_event(-1, 11, 0, 0)
            return 0
        store = numpy.empty(
            (size, self._buffer.n_columns()), dtype=numpy.int64)
        self._buffer.dump(store, False)
        self._buffer.clear(False)
        self._buffer.unlock()

        self._events.append(store)
        self._buffer.log_event(-1, 11, store.shape[0], 0)
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
        # id_frame, event, time, value1, value2
        memo = {}
        for i in range(res.shape[0]):
            event = res[i, 1]
            if event < 1000:
                continue
            ptr = res[i, 3]
            if event <= 1002:
                memo[ptr] = res[i, 4]
            else:
                if ptr not in memo:
                    # The system is freeing a non-existing pointer.
                    res[i, 4] = numpy.nan
                else:
                    res[i, 4] = -memo[ptr]
                    del memo[ptr]
        return res

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
            res[:, 2:], columns=['time', 'value1', 'value2'])
        df['event'] = list(map(lambda v: rev_event_mapping[v], res[:, 1]))
        frames = list(map(lambda v: self._frames[v], res[:, 0]))
        df['name'] = list(frame.f_code.co_name for frame in frames)
        df['filename'] = list(clean_file_name(frame.f_code.co_filename)
                              for frame in frames)
        df['lineno'] = list(frame.f_code.co_firstlineno for frame in frames)
        df['from_name'] = list(
            frame.f_back.f_code.co_name for frame in frames)
        df['from_filename'] = list(clean_file_name(frame.f_back.f_code.co_filename)
                                   for frame in frames)
        df['from_line'] = list(
            frame.f_back.f_lineno for frame in frames)
        return df
