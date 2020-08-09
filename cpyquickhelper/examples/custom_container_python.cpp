#include "custom_container.h"

#ifndef SKIP_PYTHON
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

namespace py = pybind11;


class PyCContainer
{
    public:
        PyCContainer(const std::string& bytes);
        PyCContainer(py::array_t<double> array);
        PyCContainer(py::array_t<float> array);
    
        ContainerType dtype() { return _container.dtype() ; }
        size_t size() { return _container.size() ; }
        unsigned char operator [](size_t index) { return _container[index]; }
    private:
        CustomContainer _container;
};


PyCContainer::PyCContainer(const std::string& bytes) :
    _container(bytes.size(), (void*)bytes.c_str(),
               ContainerType::BYTE, true, true) {
}


PyCContainer::PyCContainer(py::array_t<double> array) :
    _container((size_t)(array.size() * sizeof(double)),
               (void*)array.data(), ContainerType::DOUBLE,
               true, true) {
}


PyCContainer::PyCContainer(py::array_t<float> array) :
    _container((size_t)(array.size() * sizeof(float)),
               (void*)array.data(0), ContainerType::FLOAT,
               true, true) {
}


PYBIND11_MODULE(custom_container_python, m) {
	m.doc() = "Implements a custom container which may contain everything.";
    
    py::enum_<ContainerType>(m, "ContainerType")
        .value("VOID", ContainerType::VOID, "untyped pointer")
        .value("BYTE", ContainerType::BYTE, "byte pointer")
        .value("FLOAT", ContainerType::FLOAT, "float pointer")
        .value("DOUBLE", ContainerType::DOUBLE, "double pointer")
        .export_values();

	py::class_<PyCContainer>(m, "PyCContainer", 
    #if defined(__APPLE__)
    "Custom container, may contain almost everything."
    #else
    R"pbdoc(Custom container, may contain almost everything.)pbdoc"
    #endif
        )
		.def(py::init<std::string>(), "Copies the bytes into the container.")
		.def(py::init<py::array_t<double>>(), "Copies the doubles into the container.")
		.def(py::init<py::array_t<float>>(), "Copies the floats into the container.")
        .def_property_readonly("dtype", &PyCContainer::dtype, "Returns the type of every elements of the container.")
        .def_property_readonly("size", &PyCContainer::size, "Returns the size in bytes of the container.")
        .def("__getitem__", &PyCContainer::operator[], "Returns the ith bytes of the container.")
		;
}

#endif
