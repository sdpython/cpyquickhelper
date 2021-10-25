#include "_event_profiler.h"
#include "_event_profiler_allocator.h"

#ifndef SKIP_PYTHON
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <pybind11/functional.h>

namespace py = pybind11;


PYBIND11_MODULE(_event_profiler, m) {
	m.doc() = "Implements a profiler to log events.";

    py::class_<CEventProfilerEvent> (m, "CEventProfilerEvent", 
    #if defined(__APPLE__)
    "Description of an event logged by the the class @see cl CEventProfiler."
    #else
    R"pbdoc(
Description of an event logged by the the class @see cl CEventProfiler.)pbdoc"
    #endif
    )
        .def_readwrite("id_frame", &CEventProfilerEvent::id_frame, "frame id")
        .def_readwrite("id_arg", &CEventProfilerEvent::id_frame, "arg id")
        .def_readwrite("event", &CEventProfilerEvent::event, "event kind")
        .def_readwrite("time", &CEventProfilerEvent::time, "time")
        .def_readwrite("value1", &CEventProfilerEvent::value1, "first value")
        .def_readwrite("value2", &CEventProfilerEvent::value2, "second value")
        .def("__str__", &CEventProfilerEvent::to_string, "usual");

	py::class_<CEventProfiler> pyev (m, "CEventProfiler", 
    #if defined(__APPLE__)
    "C++ buffer for @see cl EventProfiler."
    #else
    R"pbdoc(
C++ buffer for @see cl EventProfiler.)pbdoc"
    #endif
        );
    
	pyev.def(py::init<int64_t>(), py::arg("size"));
	pyev.def("__str__", &CEventProfiler::__str__, "usual");
	pyev.def("__repr__", &CEventProfiler::__repr__, "usual");
    pyev.def("__len__", &CEventProfiler::size, "usual");

	pyev.def("register_pyinstance",
             [](CEventProfiler &self, py::object f) {
                f.inc_ref();
                self.register_pyinstance((void*)f.ptr());
             }, "Registers the python instance with holds this one.");

    pyev.def("__iter__", [](CEventProfiler &v) {
                return py::make_iterator(v.begin(), v.end());
             }, py::keep_alive<0, 1>()); /* Keep vector alive while iterator is used */

    pyev.def("__getitem__", &CEventProfiler::operator[],
             "Returns the ith bytes of the container.");
    
    pyev.def("log_event", &CEventProfiler::LogEvent, "Logs an event.");
    
    pyev.def("n_columns", &CEventProfiler::n_columns,
             "Returns the number of stored int64 per event.");
    
    pyev.def("lock", &CEventProfiler::lock, "Lock the profiler.");
    pyev.def("unlock", &CEventProfiler::unlock, "Unlock the profiler.");
    pyev.def("clear", &CEventProfiler::clear, "Empties the buffer.");
    
    pyev.def("dump_and_clear",
             [](CEventProfiler& self,
               py::array_t<int64_t, py::array::c_style | py::array::forcecast>& buffer,
               bool lock) -> int64_t {
                py::buffer_info b_buffer = buffer.request();
                int64_t* ptr = static_cast<int64_t*>(b_buffer.ptr);
                return self.dump_and_clear(ptr, buffer.size(), lock);
             }, "Copies the buffer into an array and clears it.");
    
    pyev.def("start",
             [](CEventProfiler& self) {
                MemoryAllocator& static_allocator = get_static_allocator();
                static_allocator.event_profiler = &self;
                PyMem_GetAllocator(PYMEM_DOMAIN_RAW, &static_allocator.old_allocator);
                static_allocator.new_allocator.ctx = static_allocator.old_allocator.ctx;
                static_allocator.new_allocator.malloc = profiled_malloc;
                static_allocator.new_allocator.calloc = profiled_calloc;
                static_allocator.new_allocator.realloc = profiled_realloc;
                static_allocator.new_allocator.free = profiled_free;
                PyMem_SetAllocator(PYMEM_DOMAIN_RAW, &static_allocator.new_allocator);
             }, "Starts the memory profiler.");
    
    pyev.def("stop",
             [](CEventProfiler& self) {
                MemoryAllocator& static_allocator = get_static_allocator();
                PyMem_SetAllocator(PYMEM_DOMAIN_RAW, &static_allocator.old_allocator);
                static_allocator.event_profiler = NULL;
             }, "Stops the memory profiler.");

    pyev.def("c_log_event",
             [](CEventProfiler& self, py::handle frame /*PyFrameObject*/,
                    const std::string &s_event, py::handle arg /*PyObject*/) {
                bool add_frame, add_arg;
                int64_t i_event = self.get_event(s_event);
                bool res = self.cLogEvent((void*)frame.ptr(), (void*)arg.ptr(),
                                          i_event, add_frame, add_arg);
                if (add_frame)
                    frame.inc_ref(); // Py_INCREF(frame);
                if (add_arg)
                    arg.inc_ref(); // Py_INCREF(add);
                if (!res) {
                    // empty cache
                    PyObject* obj = (PyObject*)self.get_pyinstance();
                    py::handle h(obj);
                    auto meth = h.attr("_empty_cache");
                    meth();
                }
             }, "Logs an evant faster.");

    pyev.def("delete",
             [](CEventProfiler& self) {
                for(auto it: self.mem_frame()) {
                    py::handle h((PyObject*)it.second);
                    h.dec_ref();
                }
                for(auto it: self.mem_arg()) {
                    py::handle h((PyObject*)it.second);
                    h.dec_ref();
                }
                self.delete_pyobj();
                PyObject* obj = (PyObject*)self.get_pyinstance();
                py::handle h(obj);
                h.dec_ref();
             }, "Decrefs all the stored objects.");

    pyev.def("get_saved_maps",
            [](CEventProfiler& self) -> py::tuple {
                py::dict frame, arg;
                auto cframe = self.mem_frame();
                auto carg = self.mem_arg();
                for(auto it: cframe) {
                    PyObject* obj = (PyObject*)it.second;
                    frame[py::int_(it.first)] = py::handle(obj);
                }
                for(auto it: carg) {
                    PyObject* obj = (PyObject*)it.second;
                    arg[py::int_(it.first)] = py::handle(obj);
                }
                return py::make_tuple(frame, arg);
            }, "Returns the mappings between ids and python objects.");
}

#endif
