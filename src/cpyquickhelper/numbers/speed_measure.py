"""
@file
@brief Measures speed.
"""
import sys
from timeit import Timer


def measure_time(stmt, context, repeat=10, number=50, div_by_number=False):
    """
    Measures a statement and returns the results as a dictionary.

    @param      stmt            string
    @param      context         variable to know in a dictionary
    @param      repeat          average over *repeat* experiment
    @param      number          number of executions in one row
    @param      div_by_number   divide by the number of executions
    @return                     dictionary

    .. runpython::
        :showcode:

        from cpyquickhelper.numbers import measure_time
        from math import cos

        res = measure_time("cos(x)", context=dict(cos=cos, x=5.))
        print(res)

    See `Timer.repeat <https://docs.python.org/3/library/timeit.html?timeit.Timer.repeat>`_
    for a better understanding of parameter *repeat* and *number*.
    The function returns a duration corresponding to
    *number* times the execution of the main statement.
    """
    import numpy
    tim = Timer(stmt, globals=context)
    res = numpy.array(tim.repeat(repeat=repeat, number=number))
    if div_by_number:
        res /= number
    mean = numpy.mean(res)
    dev = numpy.mean(res ** 2)
    dev = (dev - mean**2) ** 0.5
    mes = dict(average=mean, deviation=dev, min_exec=numpy.min(res),
               max_exec=numpy.max(res), repeat=repeat, number=number)
    if 'values' in context:
        if hasattr(context['values'], 'shape'):
            mes['size'] = context['values'].shape[0]
        else:
            mes['size'] = len(context['values'])
    else:
        mes['context_size'] = sys.getsizeof(context)
    return mes


def _fcts():
    """
    Returns functions to measure.
    """
    import numpy
    from .cbenchmark import vector_dot_product  # pylint: disable=E0611
    from .cbenchmark import vector_dot_product16  # pylint: disable=E0611
    from .cbenchmark import vector_dot_product16_nofcall  # pylint: disable=E0611
    from .cbenchmark import vector_dot_product16_sse  # pylint: disable=E0611

    def simple_dot(values):
        return numpy.dot(values, values)

    def c11_dot(vect):
        return vector_dot_product(vect, vect)

    def c11_dot16(vect):
        return vector_dot_product16(vect, vect)

    def c11_dot16_nofcall(vect):
        return vector_dot_product16_nofcall(vect, vect)

    def c11_dot16_sse(vect):
        return vector_dot_product16_sse(vect, vect)

    return [simple_dot, c11_dot, c11_dot16, c11_dot16_nofcall, c11_dot16_sse]


def check_speed(dims=[100000], repeat=10, number=50, fLOG=print):  # pylint: disable=W0102
    """
    Prints out some information about speed computation
    of this laptop. See :ref:`cbenchmarkbranchingrst` to compare.

    @param      n           sets of dimensions to try
    @param      repeat      average over *repeat* experiment
    @param      number      number of execution in one row
    @param      fLOG        logging function
    @return                 iterator on results

    :epkg:`numpy` is multithreaded. For an accurate comparison,
    this needs to be disabled. This can be done by setting environment variable
    ``MKL_NUM_THREADS=1`` or by running:

    ::

        import mkl
        mkl.set_num_threads(1)
    """
    import numpy
    fcts = _fcts()
    mx = max(dims)
    vect = numpy.ones((mx,))
    for i in range(0, vect.shape[0]):
        vect[i] = i
    for i in dims:
        values = vect[:i].copy()
        for fct in fcts:
            ct = {fct.__name__: fct}
            ct['values'] = values
            t = measure_time("{0}(values)".format(fct.__name__),
                             repeat=repeat, number=number, context=ct)
            t['name'] = fct.__name__
            if fLOG:
                fLOG(t)
            yield t
