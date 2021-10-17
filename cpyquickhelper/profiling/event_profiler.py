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

    :param size: size of the buffer to store events
    :param filename: file to dump the results, None, to keep it in memory
    """

    _event_mapping = {
        'call': 1,
        'return': 2,
        'c_call': 3,
        'c_return': 4,
        'profile_call': 10,
        'profiler_return': 11,
    }

    def __init__(self, size=1000000):
        self._started = False
        self._prof = None
        self._frames = {-1: inspect.currentframe()}
        self._buffer = CEventProfiler(size)
        self._events = []

    def start(self):
        """
        Starts the profiling.
        """
        if self._started:
            raise RuntimeError(
                "The profiler was already started. It cannot be done again.")
        self._started = True
        self._setup_profiler()

    def stop(self):
        """
        Stops the profiling.
        """
        if not self._started:
            raise RuntimeError(
                "The profiler was not started. It must be done first.")
        self._restore_profiler()
        self._started = False

    def log_event(self, frame, event, arg):
        """
        Logs an event in the database.

        :param frame: (frame), see :mod:`inspect`
        :param event: (str)
            kind of event
        :param arg: None or...

        A frame contains the following fields:

        * f_back
        * f_builtins
        * f_code
        * f_globals
        * f_lasti
        * f_lineno
        * f_locals
        * f_trace
        * f_trace_lines
        * f_trace_opcodes
        """
        idf = id(frame)
        if idf not in self._frames:
            self._frames[idf] = frame
        r = self._buffer.log_event(idf, EventProfiler._event_mapping[event])
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
        self._buffer.log_event(-1, 10)
        self._buffer.lock()
        size = len(self._buffer)
        if size == 0:
            self._buffer.unlock()
            self._buffer.log_event(-1, 11)
            return 0
        store = numpy.empty((size, 3), dtype=numpy.int64)
        store[:, :] = -2
        self._buffer.dump(store, False)
        self._buffer.clear(False)
        self._buffer.unlock()

        self._events.append(store)
        self._buffer.log_event(-1, 11)
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
        return numpy.vstack(self._events)

    def retrieve_results(self, empty_cache=True):
        """
        Retrieves the raw results. Difficult to interpret.

        :param empty_cache: retrives the data from the C++ container and
            stores it in a numpy array before
        :return: numpy array
        """
        rev_event_mapping = {
            v: k for k, v in EventProfiler._event_mapping.items()}
        res = self.retrieve_raw_results(empty_cache=empty_cache)
        df = pandas.DataFrame(dict(time=res[:, 1]))
        df['kind'] = list(map(lambda v: rev_event_mapping[v], res[:, 2]))
        frames = list(map(lambda v: self._frames[v], res[:, 0]))
        df['name'] = list(frame.f_code.co_name for frame in frames)
        df['filename'] = list(frame.f_code.co_filename for frame in frames)
        df['lineno'] = list(frame.f_code.co_firstlineno for frame in frames)
        df['from_name'] = list(
            frame.f_back.f_code.co_name for frame in frames)
        df['from_line'] = list(
            frame.f_back.f_lineno for frame in frames)
        return df
