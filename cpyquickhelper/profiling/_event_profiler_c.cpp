#include "_event_profiler.h"
#include "_event_profiler_allocator.h"


static PyObject* _profiling_stop(PyObject* Py_UNUSED(self), PyObject* args) {
    Py_RETURN_NONE;
}


static PyObject* _profiling_start(PyObject* Py_UNUSED(self), PyObject* args) {
    Py_RETURN_NONE;
}


static PyObject* _profile_event(PyObject* self, PyObject* args) {
    PyFrameObject* frame;
    PyObject* event;
    PyObject* arg;
    if(!PyArg_ParseTuple(args, "OOO", &frame, &event, &arg)) 
        return 0;
}


static PyMethodDef module_methods[] = {
    {"_profiling_start", _profiling_start, METH_VARARGS, 0},
    {"_profiling_stop",  _profiling_stop,  METH_NOARGS,  0},
    {"_profile_event", _profile_event, METH_VARARGS, 0},
    {0, 0, 0, 0} // End of list
};


PyMODINIT_FUNC
PyInit__event_profiler_c(void) {
    static struct PyModuleDef theModuleDef = {
        PyModuleDef_HEAD_INIT, "_event_profiler_c",
        "Profiler with the C Python API.",
        -1, module_methods, 0, 0, 0, 0 };
    PyObject* m = PyModule_Create(&theModuleDef);
    return m;
}
