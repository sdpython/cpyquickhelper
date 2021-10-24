#include "_event_profiler.h"
#include "_event_profiler_allocator.h"
#include "frameobject.h"
// See https://docs.python.org/3/c-api/stable.html#contents-of-limited-api.

static CEventProfiler * _static_profiler = NULL;


static PyObject* _profiling_start(PyObject* Py_UNUSED(self), PyObject* args) {
    PyObject* osize;
    bool debug;
    if(!PyArg_ParseTuple(args, "Op", &osize, &debug)) {
        PyErr_SetString(
            PyExc_TypeError, "Unable to decode the parameters. (int, bool) are expected.");
        return 0;
    }

    if (_static_profiler != NULL) {
        PyErr_SetString(
            PyExc_RuntimeError, "CEventProfiler already started.");
        return 0;
    }

    int64_t size = PyLong_AsLongLong(osize);
    Py_DECREF(osize);
    if (size < 20) {
        PyErr_SetString(
            PyExc_RuntimeError, "CEventProfiler cannot start, size must >= 20.");
        return 0;
    }

    PyGILState_STATE state = PyGILState_Ensure();

    _static_profiler = new CEventProfiler(size);

    MemoryAllocator& static_allocator = get_static_allocator();
    static_allocator.event_profiler = _static_profiler;
    PyMem_GetAllocator(PYMEM_DOMAIN_RAW, &static_allocator.old_allocator);
    static_allocator.new_allocator.ctx = static_allocator.old_allocator.ctx;
    static_allocator.new_allocator.malloc = profiled_malloc;
    static_allocator.new_allocator.calloc = profiled_calloc;
    static_allocator.new_allocator.realloc = profiled_realloc;
    static_allocator.new_allocator.free = profiled_free;
    PyMem_SetAllocator(PYMEM_DOMAIN_RAW, &static_allocator.new_allocator);

    PyGILState_Release(state);

    Py_RETURN_NONE;
}


static PyObject* _profiling_stop(PyObject* Py_UNUSED(self), PyObject* args) {
    if (_static_profiler == NULL) {
        PyErr_SetString(
            PyExc_RuntimeError, "CEventProfiler not started.");
        return 0;
    }
    if (_static_profiler->size() != 0) {
        PyErr_SetString(
            PyExc_RuntimeError, "CEventProfiler not empty.");
        return 0;
    }

    MemoryAllocator& static_allocator = get_static_allocator();
    PyMem_SetAllocator(PYMEM_DOMAIN_RAW, &static_allocator.old_allocator);
    static_allocator.event_profiler = NULL;

    delete _static_profiler;
    _static_profiler = NULL;

    Py_RETURN_NONE;
}


static PyObject* _profiling_n_columns(PyObject* Py_UNUSED(self), PyObject* args) {
    return PyLong_FromLong(CEventProfiler::n_columns());
}

static PyObject* _profiling_log_event(PyObject *self, PyObject *const *args, Py_ssize_t nargs) {
    if(nargs != 3) {
        PyErr_SetString(
            PyExc_TypeError, "Unable to decode the parameters. (frame, str, object) are expected.");
        return 0;
    }
    
    if (_static_profiler == NULL) {
        PyErr_SetString(
            PyExc_RuntimeError, "CEventProfiler not started.");
        return 0;
    }
    
    PyGILState_STATE state = PyGILState_Ensure();

    PyFrameObject* frame = (PyFrameObject*)args[0];
    PyObject* event = args[1];
    PyObject* arg = args[2];

    const char* s_event = PyUnicode_AsUTF8(event);

    bool add_frame, add_arg;
    int64_t i_event = _static_profiler->get_event(s_event);
    bool res = _static_profiler->cLogEvent(
        (void*)frame, (void*)arg, i_event, add_frame, add_arg);
    if (add_frame)
        Py_INCREF(frame);
    if (add_arg)
        Py_INCREF(arg);
    if (!res) {
        // empty cache
        PyObject* obj = (PyObject*)_static_profiler->get_pyinstance();
        if (obj == 0) {
            PyErr_SetString(
                PyExc_TypeError,
                "EventProfiler: no callback method, increase the buffer size or specify one.");
            PyGILState_Release(state);
            return 0;
        }
        PyObject* res = PyObject_CallMethod(obj, "_empty_cache", NULL);
        Py_DECREF(res);
    }
    PyGILState_Release(state);

    Py_RETURN_NONE;
}


static PyObject* _profiling_delete(PyObject* Py_UNUSED(self), PyObject* args) {
    if (_static_profiler == NULL) {
        PyErr_SetString(
            PyExc_RuntimeError, "CEventProfiler not started.");
        return 0;
    }

    PyGILState_STATE state = PyGILState_Ensure();
    for(auto it: _static_profiler->mem_frame()) {
        Py_DECREF((PyObject*)it.second);
    }
    for(auto it: _static_profiler->mem_arg()) {
        Py_DECREF((PyObject*)it.second);
    }
    PyGILState_Release(state);

    _static_profiler->delete_pyobj();
    _static_profiler->clear(false);
    PyObject* obj = (PyObject*)_static_profiler->get_pyinstance();
    if (obj != NULL) {
        Py_DECREF(obj);
    }
    Py_RETURN_NONE;
}


static PyObject* _profiling_get_saved_maps(PyObject* Py_UNUSED(self), PyObject* args) {
    if (_static_profiler == NULL) {
        PyErr_SetString(
            PyExc_RuntimeError, "CEventProfiler not started.");
        return 0;
    }
    PyGILState_STATE state = PyGILState_Ensure();
    PyObject* dict_frame = PyDict_New();
    for(auto it: _static_profiler->mem_frame()) {
        PyObject* key = PyLong_FromLongLong(it.first);
        PyObject_SetItem(dict_frame, key, (PyObject*)it.second);
    }
    PyObject* dict_arg = PyDict_New();
    for(auto it: _static_profiler->mem_arg()) {
        PyObject* key = PyLong_FromLongLong(it.first);
        PyObject_SetItem(dict_arg, key, (PyObject*)it.second);
    }
    PyObject* res = PyTuple_New(2);
    PyTuple_SetItem(res, 0, dict_frame);
    PyTuple_SetItem(res, 1, dict_arg);
    PyGILState_Release(state);
    return res;
}


static PyObject* _profiling_dump_and_clear(PyObject* Py_UNUSED(self), PyObject* args) {
    if (_static_profiler == NULL) {
        PyErr_SetString(
            PyExc_RuntimeError, "CEventProfiler not started.");
        return 0;
    }

    int64_t* ptr;
    int64_t size;
    bool lock;
    
    PyGILState_STATE state = PyGILState_Ensure();
    if(!PyArg_ParseTuple(args, "LLp", &ptr, &size, &lock)) {
        PyGILState_Release(state);
        PyErr_SetString(
            PyExc_TypeError, "Unable to decode the parameters. (void*, int64_t, bool) are expected.");
        return 0;
    }
    int64_t copied = _static_profiler->dump_and_clear(ptr, size, lock);
    PyObject* res = PyLong_FromLongLong(copied);
    PyGILState_Release(state);
    return res;
}


static PyObject* _profiling_register_pyinstance(PyObject* Py_UNUSED(self), PyObject* args) {
    if (_static_profiler == NULL) {
        PyErr_SetString(
            PyExc_RuntimeError, "CEventProfiler not started.");
        return 0;
    }

    PyObject* obj;
    PyGILState_STATE state = PyGILState_Ensure();
    
    if(!PyArg_ParseTuple(args, "O", &obj)) {
        PyGILState_Release(state);
        PyErr_SetString(
            PyExc_TypeError, "Unable to decode the parameters. (PyObject*) is expected.");
        return 0;
    }
    Py_INCREF(obj);
    PyGILState_Release(state);
    
    _static_profiler->register_pyinstance(obj);

    Py_RETURN_NONE;
}


static PyMethodDef module_methods[] = {
    {"_profiling_start", _profiling_start, METH_VARARGS,
        "Starts the profiler. "
        "One instance is stored in a static pointer.\n\n"
        ":param size: buffer size (number of stored events)\n"
        ":param bool: unused\n"},
    {"_profiling_stop", _profiling_stop, METH_NOARGS,
        "Stops the profiler. The function checks that the profiler "
        "was emptied. The static instance is removed."},
    {"_profiling_delete", _profiling_delete, METH_NOARGS,
        "Release all references on stored python objects."},
    {"_profiling_n_columns", _profiling_n_columns, METH_NOARGS,
        "Returns the number of informations stored per event. "
        "Total size must be multiplied `sizeof(int64_t)`.\n\n"
        ":return: size"},
    {"_profiling_get_saved_maps", _profiling_get_saved_maps, METH_NOARGS,
        "Returns a tuple with two dictionaries. One for all the event frames, "
        "a second one for the event arguments."},
    {"_profiling_log_event", (PyCFunction)(void(*)(void))_profiling_log_event, METH_FASTCALL,
        "Logs an event.\n\n"
        ":param frame: frame\n"
        ":param event: a string describing the event\n"
        ":param arg: additional argument (used for built-in functions)\n"},
    {"_profiling_dump_and_clear", _profiling_dump_and_clear, METH_VARARGS,
        "Copies the buffer into another one allocated before calling this method."
        "The function does not lock the :epkg:`GIL`.\n\n"
        ":param ptr: pointer on numpy array, something like `.__array_interface__['data'][0]`\n"
        ":param size: buffer size (it must be higher than the buffer size of the profiler\n"
        ":param lock: locks the profiler while copying the buffer"},
    {"_profiling_register_pyinstance", _profiling_register_pyinstance, METH_VARARGS,
        "Registers an object called when the buffer is full. This one "
        "should call @see fn _profiling_dump_and_clear to empty the cached.\n\n"
        ":param fct: instance of the python object implementing method `_empty_cache`"},
    {0, 0, 0, 0} // End of list
};


PyMODINIT_FUNC
PyInit__event_profiler_c(void) {
    static struct PyModuleDef theModuleDef = {
        PyModuleDef_HEAD_INIT, "_event_profiler_c",
        "Event Profiler with the C Python API.",
        -1, module_methods, 0, 0, 0, 0 };
    PyObject* m = PyModule_Create(&theModuleDef);
    return m;
}
