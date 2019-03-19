
.. blogpost::
    :title: Mixing pybind11 modules
    :keywords: pybind11, C++
    :date: 2019-03-19
    :categories: pybind11

    My scenario was the following: expose a C++ class in
    one C++ module exported with :epkg:`pybind11`, then
    create a function in a another C++ module returning
    a result of this class. The solution for that is
    described in the following issue
    `Return type that is implemented in another library with another set of python bindings
    <https://github.com/pybind/pybind11/issues/616>`_, which
    links to this part of the documentation
    `Custom type casters <https://pybind11.readthedocs.io/en/stable/advanced/cast/custom.html>`_.
    and implemented in this module somewhere in
    `cbenchmark_dot.cpp <https://github.com/sdpython/cpyquickhelper/blob/master/src/cpyquickhelper/numbers/cbenchmark_dot.cpp#L608>`_.
