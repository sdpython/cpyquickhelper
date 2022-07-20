"""
@file
@brief Python wrapper around C functions.
"""
from typing import Tuple
from io import StringIO
from contextlib import redirect_stdout, redirect_stderr
from .stdchelper import begin_capture, end_capture, get_capture  # pylint: disable=E0611


def capture_output_c(function_to_call) -> Tuple:
    """
    Capture the standard output and error for
    function *function_to_call*, it wraps C code which
    catches information from the command line.

    :param function_to_call: function to call
    :return: output, error

    This function must not be called in parallel with another
    call of the same function.

    .. warning:: *error* is always empty. Both streams are merged.
    """
    if not callable(function_to_call):  # pragma no cover
        raise TypeError("function_to_call must be callable.")
    begin_capture()
    fout = function_to_call()
    end_capture()
    res = get_capture()
    if res is None:  # pragma: no cover
        return fout, None, None
    if isinstance(res, bytes):  # pragma: no cover
        return fout, res, None
    if isinstance(res, tuple):  # pragma: no cover
        return (fout, ) + res
    raise TypeError(  # pragma no cover
        f"Unexpected return type '{type(res)}'.")


def capture_output_py(function_to_call) -> Tuple[str, str]:
    """
    Capture the standard output and error for
    function *function_to_call* with function
    `redirect_stdout <https://docs.python.org/3/library/contextlib.html#contextlib.redirect_stdout>`_
    and function
    `redirect_stderr <https://docs.python.org/3/library/contextlib.html#contextlib.redirect_stderr>`_.

    :param function_to_call: function to call
    :return: output, error

    This function must not be called in parallel with another
    call of the same function.

    .. warning:: *error* is always empty. Both streams are merged.
    """
    if not callable(function_to_call):
        raise TypeError("function_to_call must be callable.")
    out, err = StringIO(), StringIO()
    with redirect_stdout(out):
        with redirect_stderr(err):
            fout = function_to_call()
    return fout, out.getvalue(), err.getvalue()


def capture_output(function_to_call, lang="py"):
    """
    Catch standard output and error for function
    *function_to_call*. If lang is *'py'*, calls
    @see fn capture_output_py or @see fn capture_output_c
    if lang is *'c'*.

    :param function_to_call: function to call
    :return: output, error
    """
    if lang == "py":
        return capture_output_py(function_to_call)
    if lang == "c":
        return capture_output_c(function_to_call)
    raise ValueError(f"lang must be 'py' or 'c' not '{lang}'")
