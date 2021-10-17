#include "_event_profiler.h"

#ifndef SKIP_PYTHON
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

namespace py = pybind11;


PYBIND11_MODULE(_event_profiler, m) {
	m.doc() = "Implements operations on weighted numbers.";

    py::class_<CEventProfilerEvent>(m, "CEventProfilerEvent")
        .def_readwrite("id_frame", &CEventProfilerEvent::id_frame, "frame id")
        .def_readwrite("time", &CEventProfilerEvent::time, "time")
        .def_readwrite("event", &CEventProfilerEvent::event, "event kind")
        .def("__str__", &CEventProfilerEvent::to_string, "usual");

	py::class_<CEventProfiler>(m, "CEventProfiler", 
    #if defined(__APPLE__)
    "C++ buffer for the EventProfiler."
    #else
    R"pbdoc(
C++ buffer for the CEventProfiler.)pbdoc"
    #endif
        )
		.def(py::init<int64_t>(), py::arg("size"))
		.def("__str__", &CEventProfiler::__str__, "usual")
		.def("__repr__", &CEventProfiler::__repr__, "usual")
        .def("__len__", &CEventProfiler::size, "usual")
        .def("__len__", &CEventProfiler::size, "usual")
        .def("__iter__", [](CEventProfiler &v) {
            return py::make_iterator(v.begin(), v.end());
        }, py::keep_alive<0, 1>()) /* Keep vector alive while iterator is used */
        .def("__getitem__", &CEventProfiler::operator[],
            "Returns the ith bytes of the container.")
        .def("log_event", &CEventProfiler::LogEvent, "Logs an event.")
        .def("lock", &CEventProfiler::lock, "Lock the profiler.")
        .def("unlock", &CEventProfiler::unlock, "Unlock the profiler.")
        .def("clear", &CEventProfiler::clear, "Empties the buffer.")
        .def("dump",
            [](CEventProfiler& self,
               py::array_t<int64_t, py::array::c_style | py::array::forcecast>& buffer,
               bool lock) -> int64_t {
            py::buffer_info b_buffer = buffer.request();
            int64_t* ptr = static_cast<int64_t*>(b_buffer.ptr);
            return self.dump(ptr, buffer.size(), lock);
        }, "Copies the buffer into an array.")
		;
}

#endif
