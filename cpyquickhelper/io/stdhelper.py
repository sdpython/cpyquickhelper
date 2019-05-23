"""
@file
@brief Python wrapper around C functions.
"""
from typing import Tuple, Union
from io import StringIO
from contextlib import redirect_stdout, redirect_stderr
from .stdchelper import begin_capture, end_capture, get_capture  # pylint: disable=E0611


def capture_output_c(function_to_call) -> Tuple[bytes, Union[bytes, None]]:
    """
    Capture the standard output and error for
    function *function_to_call*, it wraps C code which
    catches information from the command line.

    @param      function_to_call        function to call
    @return                             output, error

    This function must not be called in parallel with another
    call of the same function.

    .. warning:: *error* is always empty. Both streams are merged.
    """
    if not callable(function_to_call):
        raise TypeError("function_to_call must be callable.")
    begin_capture()
    function_to_call()
    end_capture()
    res = get_capture()
    if isinstance(res, bytes):
        return res, None
    elif isinstance(res, tuple):
        return res
    elif res is None:
        return None, None
    else:
        raise TypeError("Unexpected return type '{0}'.".format(type(res)))


def capture_output_py(function_to_call) -> Tuple[str, str]:
    """
    Capture the standard output and error for
    function *function_to_call* with function
    `redirect_stdout <https://docs.python.org/3/library/contextlib.html#contextlib.redirect_stdout>`_
    and function
    `redirect_stderr <https://docs.python.org/3/library/contextlib.html#contextlib.redirect_stderr>`_.

    @param      function_to_call        function to call
    @return                             output, error

    This function must not be called in parallel with another
    call of the same function.

    .. warning:: *error* is always empty. Both streams are merged.
    """
    if not callable(function_to_call):
        raise TypeError("function_to_call must be callable.")
    out, err = StringIO(), StringIO()
    with redirect_stdout(out):
        with redirect_stderr(err):
            function_to_call()
    return out.getvalue(), err.getvalue()


def capture_output(function_to_call, lang="py"):
    """
    Catch standard output and error for function
    *function_to_call*. If lang is *'py'*, calls
    @see fn capture_output_py or @see fn capture_output_c
    if lang is *'c'*.

    @param      function_to_call        function to call
    @return                             output, error
    """
    if lang == "py":
        return capture_output_py(function_to_call)
    elif lang == "c":
        return capture_output_c(function_to_call)
    else:
        raise ValueError("lang must be 'py' or 'c' not '{0}'".format(lang))
