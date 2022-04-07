#include "vector_container.h"

#ifndef SKIP_PYTHON
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

namespace py = pybind11;


class OneTensorFloat: public OneTensor<float> {
    public:
        int version() const { return 1; }
};


class OneTensorFloat2: public OneTensor<float> {
    public:
        int version() const { return 2; }
};


class RandomTensorVectorFloat: public RandomTensorVector<OneTensorFloat> {
    public:
        RandomTensorVectorFloat(int64_t n_vectors, int64_t n_dims):
            RandomTensorVector<OneTensorFloat>(n_vectors, n_dims) {}
};


class RandomTensorVectorFloat2: public RandomTensorVector<OneTensorFloat2> {
    public:
        RandomTensorVectorFloat2(int64_t n_vectors, int64_t n_dims):
            RandomTensorVector<OneTensorFloat2>(n_vectors, n_dims) {}
};

template <typename PT>
class ConstReferencePointer {
    protected:
        const PT* ptr_;
    public:
        ConstReferencePointer(): ptr_(NULL) { }
        ConstReferencePointer(const ConstReferencePointer<PT>& c): ptr_(c.ptr_) {}
        ConstReferencePointer(const PT* ptr): ptr_(ptr) {}
};


class TensorVectorConstReferencePointer: public ConstReferencePointer<std::vector<OneTensorFloat>> {
    public:
        TensorVectorConstReferencePointer(): ConstReferencePointer<std::vector<OneTensorFloat>>() { }
        TensorVectorConstReferencePointer(const TensorVectorConstReferencePointer& c): ConstReferencePointer<std::vector<OneTensorFloat>>(c) {}
        TensorVectorConstReferencePointer(const std::vector<OneTensorFloat>* ptr): ConstReferencePointer<std::vector<OneTensorFloat>>(ptr) {}
};


class TensorVectorConstReferencePointer2: public ConstReferencePointer<std::vector<OneTensorFloat2>> {
    public:
        TensorVectorConstReferencePointer2(): ConstReferencePointer<std::vector<OneTensorFloat2>>() { }
        TensorVectorConstReferencePointer2(const TensorVectorConstReferencePointer2& c): ConstReferencePointer<std::vector<OneTensorFloat2>>(c) {}
        TensorVectorConstReferencePointer2(const std::vector<OneTensorFloat2>* ptr): ConstReferencePointer<std::vector<OneTensorFloat2>>(ptr) {}
};


PYBIND11_MAKE_OPAQUE(std::vector<OneTensorFloat>);


PYBIND11_MODULE(vector_container_python, m) {
	m.doc() = "Looks into two ways to export an array of objects (vectors here).";

    // Reference
    
    py::class_<TensorVectorConstReferencePointer>(m, "TensorVectorConstReferencePointer",
                                                  "Reference on `std::vector<OneTensorFloat>`.");
    py::class_<TensorVectorConstReferencePointer2>(m, "TensorVectorConstReferencePointer2",
                                                   "Reference on `std::vector<OneTensorFloat2>`.");

    // OneTensor

	py::class_<OneTensorFloat> one_tensor (m, "OneTensor", 
    #if defined(__APPLE__)
    "Definition of a simple tensor."
    #else
    R"pbdoc(Definition of a simple tensor. `PYBIND11_MAKE_OPAQUE(std::vector<OneTensorFloat>)` is defined.)pbdoc"
    #endif
    );

    one_tensor.def(py::init<>(), "Creates an empty tensor.");

    one_tensor.def("set_content", [](OneTensorFloat& p, py::array_t<float, py::array::c_style | py::array::forcecast> v) {
            std::function<size_t(size_t)> f = [&v](size_t i) -> size_t { return (size_t)v.shape(i); };
            p.set_content(v.ndim(), &f, v.data());
        }, R"pbdoc(Fills the tensor.

:param v: numpy array
        )pbdoc", py::arg("content"));

    one_tensor.def_property_readonly("element_type", &OneTensorFloat::element_type,
                                     "Returns the type of every element of the tensor.");
    one_tensor.def_property_readonly("version", &OneTensorFloat::version, "Returns the version.");
    one_tensor.def_property_readonly("shape", &OneTensorFloat::shape, "Returns the tensor shape.");
    one_tensor.def("__getitem__", [](const OneTensorFloat& tensor, int64_t index) { return tensor.data()[index]; },
                   "Returns the ith element of the container.");
    one_tensor.def("__len__", [](const OneTensorFloat& tensor) { return tensor.size(); },
                   "Returns the length of the tensor.");

    // OneTensor2

	py::class_<OneTensorFloat2> one_tensor2 (m, "OneTensor2", 
    #if defined(__APPLE__)
    "Definition of a simple tensor."
    #else
    R"pbdoc(Definition of a simple tensor, `PYBIND11_MAKE_OPAQUE(std::vector<OneTensorFloat>)` is node defined.)pbdoc"
    #endif
        );

	one_tensor2.def(py::init<>(), "Creates an empty tensor.");

    one_tensor2.def("set_content", [](OneTensorFloat2& p, py::array_t<float, py::array::c_style | py::array::forcecast> v) {
            std::function<size_t(size_t)> f = [&v](size_t i) -> size_t { return (size_t)v.shape(i); };
            p.set_content(v.ndim(), &f, v.data());
        }, R"pbdoc(Fills the tensor.

:param v: numpy array
        )pbdoc", py::arg("content"));

    one_tensor2.def_property_readonly("element_type", &OneTensorFloat2::element_type,
                                      "Returns the type of every element of the tensor.");
    one_tensor2.def_property_readonly("version", &OneTensorFloat2::version, "Returns the version.");
    one_tensor2.def_property_readonly("shape", &OneTensorFloat2::shape, "Returns the tensor shape.");
    one_tensor2.def("__getitem__", [](const OneTensorFloat2& tensor, int64_t index) { return tensor.data()[index]; },
                    "Returns the ith element of the container.");
    one_tensor2.def("__len__", [](const OneTensorFloat2& tensor) { return tensor.size(); },
                    "Returns the length of the tensor.");

    // vector of vectors

    py::class_<std::vector<OneTensorFloat>>(m, "OneTensorVector")
        .def(py::init<>())
        .def("push_back", [](std::vector<OneTensorFloat>& v, const OneTensorFloat& tensor) { v.push_back(tensor); })
        .def("reserve", [](std::vector<OneTensorFloat>& v, const size_t len) { v.reserve(len); })
        .def("__len__", [](const std::vector<OneTensorFloat>& v) { return v.size(); })
        .def("__iter__", [](const std::vector<OneTensorFloat>& v) {
            return py::make_iterator(v.cbegin(), v.cend());
        }, py::keep_alive<0, 1>())
        .def("__getitem__", [](const std::vector<OneTensorFloat>& v, const size_t idx) { return v.at(idx); });

    // container

    py::class_<RandomTensorVectorFloat> tv(m, "RandomTensorVectorFloat", "Random Tensors");
    tv.def(
        py::init<int64_t, int64_t>(), "Creates *n_vectors* random tensors of size *n_dims*.",
        py::arg("n_vectors"), py::arg("n_dims"));
    tv.def("get_tensor_vector", [](const RandomTensorVectorFloat& v) { return v.get(); });
    tv.def("get_tensor_vector_ref", [](const RandomTensorVectorFloat& v) {
        return TensorVectorConstReferencePointer(v.getp()); });

    py::class_<RandomTensorVectorFloat2> tv2(m, "RandomTensorVectorFloat2", "Random Tensors");
    tv2.def(
        py::init<int64_t, int64_t>(), "Creates *n_vectors* random tensors of size *n_dims*.",
        py::arg("n_vectors"), py::arg("n_dims"));
    tv2.def("get_tensor_vector", [](const RandomTensorVectorFloat2& v) { return v.get(); });
    tv2.def("get_tensor_vector_ref", [](const RandomTensorVectorFloat2& v) {
        return TensorVectorConstReferencePointer2(v.getp()); });
}

#endif
