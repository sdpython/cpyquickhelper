"""
@file
@brief Measures speed.
"""
from timeit import Timer


def measure_time(stmt, context, repeat=10, number=50):
    """
    Measures a statement and returns the results as a dictionary.

    @param      stmt        string
    @param      context     variable to know in a dictionary
    @param      repeat      average over *repeat* experiment
    @param      number      number of execution in one row
    @return                 dictionary

    .. runpython::
        :showcode:

        from cpyquickhelper.numbers import measure_time
        from math import cos

        res = measure_time("cos(x)", context=dict(cos=cos, x=5.))
        print(res)
    """
    import numpy
    tim = Timer(stmt, globals=context)
    res = numpy.array(tim.repeat(repeat=repeat, number=number))
    mean = numpy.mean(res)
    dev = numpy.mean(res ** 2)
    dev = (dev - mean**2) ** 0.5
    return dict(average=mean, deviation=dev, min_exec=numpy.min(res),
                max_exec=numpy.max(res), repeat=repeat, number=number,
                size=context['values'].shape[0])


def _fcts():
    """
    Returns functions to measure.
    """
    import numpy
    from .cbenchmark import vector_dot_product  # pylint: disable=E0611
    from .cbenchmark import vector_dot_product16  # pylint: disable=E0611
    from .cbenchmark import vector_dot_product16_sse  # pylint: disable=E0611

    def simple_dot(values):
        return numpy.dot(values, values)

    def c11_dot(vect):
        return vector_dot_product(vect, vect)

    def c11_dot16(vect):
        return vector_dot_product16(vect, vect)

    def c11_dot16_sse(vect):
        return vector_dot_product16_sse(vect, vect)

    return [simple_dot, c11_dot, c11_dot16, c11_dot16_sse]


def check_speed(dims=[100000], repeat=10, number=50, fLOG=print):  # pylint: disable=W0102
    """
    Prints out some information about speed computation
    of this laptop. See :ref:`cbenchmarkbranchingrst` to compare.

    @param      n           sets of dimensions to try
    @param      repeat      average over *repeat* experiment
    @param      number      number of execution in one row
    @param      fLOG        logging function
    @return                 iterator on results
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
