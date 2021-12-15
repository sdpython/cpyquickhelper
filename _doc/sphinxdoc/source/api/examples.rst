
cpyquickhelper.examples
=======================

.. contents::
    :local:

C Container
+++++++++++

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

DLPack
++++++

:epkg:`DLPack` is an initiative to facilitate the exchange of data
accross libraries, mostly libraries of machine learning. The following
classes implements the class and the exchange protocol.

.. autosignature:: cpyquickhelper.examples.dlpack_container_python.DLDevice

.. autosignature:: cpyquickhelper.examples.dlpack_container_python.DLDeviceType

.. autosignature:: cpyquickhelper.examples.dlpack_container_python.DLDataType

.. autosignature:: cpyquickhelper.examples.dlpack_container_python.DLDataTypeCode

.. autosignature:: cpyquickhelper.examples.dlpack_container_python.DLPackContainer
