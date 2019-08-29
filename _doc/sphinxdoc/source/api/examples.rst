
cpyquickhelper.examples
=======================

There is no particular purpose to this examples
except testing some assumptions. The first class
introduces a C container which holds a void pointer.

.. autosignature:: cpyquickhelper.examples.custom_container_python.ContainerType

.. autosignature:: cpyquickhelper.examples.custom_container_python.PyCContainer

The following example is a way to get the sequence of bytes
for a sequence of doubles.

.. runpython::
    :showcode:

    import numpy
    from cpyquickhelper.examples.custom_container_python import PyCContainer

    arr = numpy.array([0, 2, 8, 8e-2, 1], dtype=numpy.float32)
    cc = PyCContainer(arr)
    print(cc.dtype)
    print(cc.size)
    content = [cc[i] for i in range(cc.size)]
    print(content)
