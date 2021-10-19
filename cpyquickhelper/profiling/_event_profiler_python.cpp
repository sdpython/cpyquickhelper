#include "_event_profiler.h"

#ifndef SKIP_PYTHON
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

namespace py = pybind11;


typedef struct MemoryAllocator {
    PyMemAllocatorEx old_allocator;
    PyMemAllocatorEx new_allocator;
    CEventProfiler* event_profiler;
    MemoryAllocator() { event_profiler = NULL; }
} _MemoryAllocator ;


static MemoryAllocator static_allocator;


void* profiled_malloc(void *ctx, size_t size) {
    void* ptr = static_allocator.old_allocator.malloc(ctx, size);
    static_allocator.event_profiler->LogEvent(
        0, 0, 1000, (int64_t)ptr, (int64_t)size);
    return ptr;
}


void* profiled_calloc(void *ctx, size_t nelem, size_t elsize) {
    void* ptr = static_allocator.old_allocator.calloc(ctx, nelem, elsize);
    static_allocator.event_profiler->LogEvent(
        0, 0, 1001, (int64_t)ptr, (int64_t)(elsize * nelem));
    return ptr;
}


void* profiled_realloc(void *ctx, void* old_ptr, size_t new_size) {
    void* ptr = static_allocator.old_allocator.realloc(ctx, old_ptr, new_size);
    static_allocator.event_profiler->LogEvent(
        0, 0, 1004, (int64_t)old_ptr, (int64_t)0);
    static_allocator.event_profiler->LogEvent(
        0, 0, 1002, (int64_t)ptr, (int64_t)new_size);
    return ptr;
}


void profiled_free(void *ctx, void *ptr) {
    static_allocator.event_profiler->LogEvent(0, 0, 1003, (int64_t)ptr, 0);
    static_allocator.old_allocator.free(ctx, ptr);
}


PYBIND11_MODULE(_event_profiler, m) {
	m.doc() = "Implements a profiler to log events.";

    py::class_<CEventProfilerEvent>(m, "CEventProfilerEvent")
        .def_readwrite("id_frame", &CEventProfilerEvent::id_frame, "frame id")
        .def_readwrite("id_arg", &CEventProfilerEvent::id_frame, "arg id")
        .def_readwrite("event", &CEventProfilerEvent::event, "event kind")
        .def_readwrite("time", &CEventProfilerEvent::time, "time")
        .def_readwrite("value1", &CEventProfilerEvent::value1, "first value")
        .def_readwrite("value2", &CEventProfilerEvent::value2, "second value")
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
        .def("__iter__", [](CEventProfiler &v) {
            return py::make_iterator(v.begin(), v.end());
        }, py::keep_alive<0, 1>()) /* Keep vector alive while iterator is used */
        .def("__getitem__", &CEventProfiler::operator[],
            "Returns the ith bytes of the container.")
        .def("log_event", &CEventProfiler::LogEvent, "Logs an event.")
        .def("n_columns", &CEventProfiler::n_columns,
            "Returns the number of stored int64 per event.")
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
        .def("start",
            [](CEventProfiler& self) {
                static_allocator.event_profiler = &self;
                PyMem_GetAllocator(PYMEM_DOMAIN_RAW, &static_allocator.old_allocator);
                static_allocator.new_allocator.ctx = static_allocator.old_allocator.ctx;
                static_allocator.new_allocator.malloc = profiled_malloc;
                static_allocator.new_allocator.calloc = profiled_calloc;
                static_allocator.new_allocator.realloc = profiled_realloc;
                static_allocator.new_allocator.free = profiled_free;
                PyMem_SetAllocator(PYMEM_DOMAIN_RAW, &static_allocator.new_allocator);
            }, "Starts the memory profiler.")
        .def("stop",
            [](CEventProfiler& self) {
                PyMem_SetAllocator(PYMEM_DOMAIN_RAW, &static_allocator.old_allocator);
                static_allocator.event_profiler = NULL;
            }, "Stops the memory profiler.")
        .def("c_log_event",
            [](CEventProfiler& self, py::object frame /*PyFrameObject*/,
                    const std::string &s_event, py::object arg /*PyObject*/) {
                bool add_frame, add_arg;
                int64_t i_event = self.get_event(s_event);
                bool res = self.cLogEvent((int64_t)frame.ptr(), (void*)frame.ptr(), 
                                          (int64_t)arg.ptr(), (void*) arg.ptr(),
                                          i_event, add_frame, add_arg);
                if (add_frame)
                    frame.inc_ref(); // Py_INCREF(frame);
                if (add_arg)
                    arg.inc_ref(); // Py_INCREF(add);
                if (res) {
                    // empty cache
                }
            }, "Logs an evant faster.")
        .def("delete", [] (CEventProfiler& self) {
            for(auto it: self.mem_frame()) {
                py::handle h((PyObject*)it.second);
                h.dec_ref();
            }
            for(auto it: self.mem_arg()) {
                py::handle h((PyObject*)it.second);
                h.dec_ref();
            }
            self.delete_pyobj();
        }, "Decrefs all the stored objects.")
		;
}

#endif
