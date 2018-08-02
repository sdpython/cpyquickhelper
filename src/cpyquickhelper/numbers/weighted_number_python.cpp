#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include "weighted_number.h"

namespace py = pybind11;


PYBIND11_MODULE(weighted_number, m) {
	m.doc() = "Implements operation on weighted numbers";

	py::class_<WeightedDouble>(m, "WeightedDouble",
			R"pbdoc(
			Implements a weighted double used to speed up computation with aggregation.
			)pbdoc")
		.def(py::init<double, double>(), py::arg("value"), py::arg("weight") = 1.)
		.def("__str__", &WeightedDouble::__str__)
		.def("__repr__", &WeightedDouble::__repr__)
		.def(py::self + py::self)
		.def(py::self - py::self)
		.def(py::self * py::self)
		.def(py::self / py::self)
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
		R"pbdoc(
			Implements a weighted float used to speed up computation with aggregation.
			)pbdoc")
		.def(py::init<float, float>(), py::arg("value"), py::arg("weight") = 1.)
		.def("__str__", &WeightedFloat::__str__)
		.def("__repr__", &WeightedFloat::__repr__)
		.def(py::self + py::self)
		.def(py::self - py::self)
		.def(py::self * py::self)
		.def(py::self / py::self)
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
