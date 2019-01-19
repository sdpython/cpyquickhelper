
cpyquickhelper.numbers
======================

This class uses :epkg:`pybind11` to created a :epkg:`Python`
from a :epkg:`C++` class. This is heavily inspired from
the example `python_example <https://github.com/pybind/python_example>`_.
The first classes are an example which exposes two C++
classes.

.. autosignature:: cpyquickhelper.numbers.weighted_number.WeightedDouble

.. autosignature:: cpyquickhelper.numbers.weighted_number.WeightedFloat

The second example exposes a function doing a benchmark comparing
the execution time of a couple of C++ function. The difficulty is
the measure cannot happen in Python as the C++ execution time
is not significant compare to the time spent in Python.
Results are stored in a C++ classes exposes in Python.

.. autosignature:: cpyquickhelper.numbers.cbenchmark.ExecutionStat

The function to be tested can be found in
`cbenchmark.cpp <https://github.com/sdpython/cpyquickhelper/blob/master/src/cpyquickhelper/numbers/cbenchmark.cpp>`_ and
`repeat_fct.h <https://github.com/sdpython/cpyquickhelper/blob/master/src/cpyquickhelper/numbers/repeat_fct.h>`_.
It all began with the blog post
`Why is it faster to process a sorted array than an unsorted array? <https://stackoverflow.com/questions/11227809/why-is-it-faster-to-process-a-sorted-array-than-an-unsorted-array/11227902#11227902>`_.
It plays with a function for which the third line
is implemented in different ways.

::

    int nb = 0;
    for(auto it = values.begin(); it != values.end(); ++it)
        if (*it >= th) nb++; // this line changes
        if (*it >= th) nb++; // and is repeated 10 times inside the loop.
        // ... 10 times
    return nb;

And it is replaced by the following scenarios:

.. autosignature:: cpyquickhelper.numbers.cbenchmark.measure_scenario_A

.. autosignature:: cpyquickhelper.numbers.cbenchmark.measure_scenario_B

.. autosignature:: cpyquickhelper.numbers.cbenchmark.measure_scenario_C

.. autosignature:: cpyquickhelper.numbers.cbenchmark.measure_scenario_D

.

.. autosignature:: cpyquickhelper.numbers.cbenchmark.measure_scenario_E

.. autosignature:: cpyquickhelper.numbers.cbenchmark.measure_scenario_F

.. autosignature:: cpyquickhelper.numbers.cbenchmark.measure_scenario_G

.. autosignature:: cpyquickhelper.numbers.cbenchmark.measure_scenario_H

.

.. autosignature:: cpyquickhelper.numbers.cbenchmark.measure_scenario_I

.. autosignature:: cpyquickhelper.numbers.cbenchmark.measure_scenario_J

The last implemented is taken from
`Checking whether a number is positive or negative using bitwise operators <https://stackoverflow.com/questions/3779202/checking-whether-a-number-is-positive-or-negative-using-bitwise-operators>`_.

.. autosignature:: cpyquickhelper.numbers.cbenchmark.measure_scenario_I

The other function implements different *dot* products between two
vectors:

.. autosignature:: cpyquickhelper.numbers.cbenchmark.vector_dot_product

The second function does the same dot product but while computing
the dot product, if the remaining size is more than 16,
it calls a function which does the 16 product in one sequence.

.. autosignature:: cpyquickhelper.numbers.cbenchmark.vector_dot_product16

The following use :epkg:`SSE` instructions.
See documentation on `Intel website <https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=4895,152,3895,3886,3877,5554,5559,5554,152,127,3895,127&text=_mm_add_ps>`_.

.. autosignature:: cpyquickhelper.numbers.cbenchmark.vector_dot_product16_sse

The next one is using AVX instruction with 512 bits.

.. autosignature:: cpyquickhelper.numbers.vector_dot_product16_avx512

The last function is used to measure the time spent in the python
binding, it is the same signature as the dot product but does nothing.

.. autosignature:: cpyquickhelper.numbers.cbenchmark.empty_vector_dot_product
