#include "custom_container.h"

#ifndef SKIP_PYTHON
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

namespace py = pybind11;


class PyCContainer {
    public:
        PyCContainer(const std::string& bytes, bool copy);
        PyCContainer(py::array_t<int64_t, py::array::c_style | py::array::forcecast> array, bool copy);
        PyCContainer(py::array_t<double, py::array::c_style | py::array::forcecast> array, bool copy);
        PyCContainer(py::array_t<float, py::array::c_style | py::array::forcecast> array, bool copy);
        PyCContainer(PyCContainer* container, bool copy);

        ContainerType dtype() const { return _container.dtype() ; }
        size_t size() const { return _container.size() ; }
        void* data() const { return _container.data() ; }
        unsigned char operator [](size_t index) const { return _container[index]; }

    protected:
        CustomContainer _container;
};


PyCContainer::PyCContainer(const std::string& bytes, bool copy) :
    _container(bytes.size(), (void*)bytes.c_str(),
               ContainerType::CT_BYTE, copy, copy) {
    if (!copy)
        throw std::runtime_error(
            "The constructor cannot ensure the string remains alive "
            "while this object is used.");
}


PyCContainer::PyCContainer(py::array_t<int64_t, py::array::c_style | py::array::forcecast> array, bool copy) :
    _container((size_t)(array.size() * sizeof(int64_t)),
               (void*)array.data(), ContainerType::CT_INT64,
               copy, copy) {
    // If !copy and the underlying array is deleted, this could the program to crash.
}


PyCContainer::PyCContainer(py::array_t<double, py::array::c_style | py::array::forcecast> array, bool copy) :
    _container((size_t)(array.size() * sizeof(double)),
               (void*)array.data(), ContainerType::CT_DOUBLE,
               copy, copy) {
    // If !copy and the underlying array is deleted, this could the program to crash.
}


PyCContainer::PyCContainer(py::array_t<float, py::array::c_style | py::array::forcecast> array, bool copy) :
    _container((size_t)(array.size() * sizeof(float)),
               (void*)array.data(0), ContainerType::CT_FLOAT,
               copy, copy) {
    // If !copy and the underlying array is deleted, this could the program to crash.
}


PyCContainer::PyCContainer(PyCContainer* container, bool copy) :
    _container(container->size(), container->data(), container->dtype(),
               copy, copy) {
    // If !copy and the underlying array is deleted, this could the program to crash.
}


PYBIND11_MODULE(custom_container_python, m) {
	m.doc() = "Implements a custom container which may contain everything.";

    py::enum_<ContainerType>(m, "ContainerType", "Type enumeration, similar to dtype.")
        .value("VOID", ContainerType::CT_VOID, "untyped pointer")
        .value("BYTE", ContainerType::CT_BYTE, "byte pointer")
        .value("FLOAT", ContainerType::CT_FLOAT, "float pointer")
        .value("DOUBLE", ContainerType::CT_DOUBLE, "double pointer")
        .value("INT64", ContainerType::CT_INT64, "int64 pointer")
        .export_values();

	py::class_<PyCContainer> container (m, "PyCContainer", 
    #if defined(__APPLE__)
    "Custom container, may contain almost everything."
    #else
    R"pbdoc(Custom container, may contain almost everything.

:param value: value to copy or to hold or pointer onto
:param copy: if True, the constructor copies the data,
    if False, the pointer is just borrowed.

.. warning::
    If copy is False, the developer must know what it is doing as this pointer
    may be deleted if the object is not used by python anymore.
)pbdoc"
    #endif
        );

	container.def(py::init<std::string, bool>(), "Copies the bytes into the container.",
                  py::arg("value"), py::arg("copy")=true);

    container.def(py::init<py::array_t<int64_t, py::array::c_style | py::array::forcecast>, bool>(),
                 "Copies the integer into the container.",
                  py::arg("value"), py::arg("copy")=true);

	container.def(py::init<py::array_t<double, py::array::c_style | py::array::forcecast>, bool>(),
                  "Copies the doubles into the container.",
                  py::arg("value"), py::arg("copy")=true);

    container.def(py::init<py::array_t<float, py::array::c_style | py::array::forcecast>, bool>(),
                  "Copies the floats into the container.",
                  py::arg("value"), py::arg("copy")=true);

    container.def(py::init<PyCContainer*, bool>(), "Copies or borrow the container into another one.",
                  py::arg("value"), py::arg("copy")=true);

    container.def_property_readonly("dtype", &PyCContainer::dtype, "Returns the type of every elements of the container.");
    container.def_property_readonly("size", &PyCContainer::size, "Returns the size in bytes of the container.");
    container.def("__getitem__", &PyCContainer::operator[], "Returns the ith bytes of the container.");
}

#endif
