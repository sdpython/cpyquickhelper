
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

.. autosignature:: cpyquickhelper.numbers.cbenchmark.measure_scenario_E

.. autosignature:: cpyquickhelper.numbers.cbenchmark.measure_scenario_F

.. autosignature:: cpyquickhelper.numbers.cbenchmark.measure_scenario_G

.. autosignature:: cpyquickhelper.numbers.cbenchmark.measure_scenario_H

.. autosignature:: cpyquickhelper.numbers.cbenchmark.measure_scenario_I

.. autosignature:: cpyquickhelper.numbers.cbenchmark.measure_scenario_J

The last implemented is taken from
`Checking whether a number is positive or negative using bitwise operators <https://stackoverflow.com/questions/3779202/checking-whether-a-number-is-positive-or-negative-using-bitwise-operators>`_.

.. autosignature:: cpyquickhelper.numbers.cbenchmark.measure_scenario_I
