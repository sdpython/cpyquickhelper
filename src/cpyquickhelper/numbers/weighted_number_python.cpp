#include "weighted_number.h"

#ifndef SKIP_PYTHON
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

namespace py = pybind11;


PYBIND11_MODULE(weighted_number, m) {
	m.doc() = "Implements operations on weighted numbers.";

	py::class_<WeightedDouble>(m, "WeightedDouble", 
    #if defined(__APPLE__)
    "Implements a weighted double used to speed up computation with aggregation."
    #else
    R"pbdoc(
Implements a weighted double used to speed up computation with aggregation.
It contains two attributes:

* value: unweighted value
* weight: weight associated to the value, it should be positive, but that's not enforced)pbdoc"
    #endif
        )
		.def(py::init<double, double>(), py::arg("value"), py::arg("weight") = 1.)
		.def("__str__", &WeightedDouble::__str__, "usual")
		.def("__repr__", &WeightedDouble::__repr__, "usual")
		.def_readwrite("value", &WeightedDouble::value, "unweighted numeric value (counter)")
		.def_readwrite("weight", &WeightedDouble::weight, "weight")
		.def(py::self + py::self, "operator +")
		.def(py::self - py::self, "operator -")
		.def(py::self * py::self, "operator *")
		.def(py::self / py::self, "operator /")
		.def(py::self += py::self)
		.def(py::self -= py::self)
		.def(py::self *= py::self)
		.def(py::self /= py::self)
		.def(py::self * double())
		.def(py::self / double())
		.def(py::self *= double())
		.def(py::self /= double())
		.def(py::self + double())
		.def(py::self - double())
		.def(py::self += double())
		.def(py::self -= double())
		//.def(double() * py::self)
		//.def(double() / py::self)
		.def(py::self == py::self)
		.def(py::self < py::self)
		.def(py::self <= py::self)
		.def(py::self > py::self)
		.def(py::self >= py::self)
		;

	py::class_<WeightedFloat>(m, "WeightedFloat", 
    #if defined(__APPLE__)
    "Implements a weighted float used to speed up computation with aggregation."
    #else
    R"pbdoc(
Implements a weighted float used to speed up computation with aggregation.
It contains two attributes:

* value: unweighted value
* weight: weight associated to the value, it should be positive, but that's not enforced)pbdoc"
#endif
        )
		.def(py::init<float, float>(), py::arg("value"), py::arg("weight") = 1.)
		.def("__str__", &WeightedFloat::__str__, "usual")
		.def("__repr__", &WeightedFloat::__repr__, "usual")
		.def_readwrite("value", &WeightedFloat::value, "unweighted numeric value (counter)")
		.def_readwrite("weight", &WeightedFloat::weight, "weight")
		.def(py::self + py::self, "operator +")
		.def(py::self - py::self, "operator -")
		.def(py::self * py::self, "operator *")
		.def(py::self / py::self, "operator /")
		.def(py::self += py::self)
		.def(py::self -= py::self)
		.def(py::self *= py::self)
		.def(py::self /= py::self)
		.def(py::self * float())
		.def(py::self / float())
		.def(py::self *= float())
		.def(py::self /= float())
		.def(py::self + float())
		.def(py::self - float())
		.def(py::self += float())
		.def(py::self -= float())
		//.def(double() * py::self)
		//.def(double() / py::self)
		.def(py::self == py::self)
		.def(py::self < py::self)
		.def(py::self <= py::self)
		.def(py::self > py::self)
		.def(py::self >= py::self)
		;
}

#endif
