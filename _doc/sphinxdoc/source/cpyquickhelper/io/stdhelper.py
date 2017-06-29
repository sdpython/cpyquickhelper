"""


Python wrapper around C functions.


:githublink:`%|py|5`
"""

from .stdchelper import begin_capture, end_capture, get_capture


def capture_output(function_to_call):
    """
    
        Capture the standard output and error when
        function *function_to_call* wraps C code whichs
        throws information from the command line.
    
        :param      function_to_call:        function to call
        :return:                             output, error
    
        This function must not be called in parallel with another
        call of the same function.
    
        .. warning:: *error* is always empty. Both streams are merged.
        

        :githublink:`%|py|22`
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
    else:
        raise TypeError("Unexpected return type '{0}'.".format(type(res)))
