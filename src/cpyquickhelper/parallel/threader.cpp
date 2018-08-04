// Source: https://github.com/munawarb/Python-Kill-Thread-Extension/blob/master/threader.c
#include "threader.hpp"
#include <stdexcept>
#include <Python.h>

struct module_state {
    PyObject *error;
};

#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))

///////////////////////////////////////////////
//////////// python function //////////////////
///////////////////////////////////////////////

static PyObject* kill_thread(PyObject* self, PyObject* arg) 
{
    unsigned long uid = 0;
	if (arg == NULL || !PyArg_ParseTuple(arg, "k", &uid)) {
        struct module_state *st = GETSTATE(self);
        PyErr_SetString(st->error, "Null pointer is not allowed.");    
        return 0;     
    }
    
    int succeeded = threader_kill_thread(uid); 
	return Py_BuildValue("i", succeeded);
}



////////////////////////////////////////////////////
//////////// module definition /////////////////////
////////////////////////////////////////////////////

const char * module_name = "threader" ;

static int threader_module_traverse(PyObject *m, visitproc visit, void *arg) {
    Py_VISIT(GETSTATE(m)->error);
    return 0;
}

static int threader_module_clear(PyObject *m) {
    Py_CLEAR(GETSTATE(m)->error);
    return 0;
}

static PyMethodDef fonctions [] = {
  {"kill_thread", kill_thread, METH_VARARGS, R"pbdoc(
                                Terminates a thread.
                                Signature: *def kill_thread(int thread_id):*.
                                )pbdoc"},
  {NULL, NULL}
} ;

static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        module_name,
        "Helper for parallelization with threads with C++.",
        sizeof(struct module_state),
        fonctions,
        NULL,
        threader_module_traverse,
        threader_module_clear,
        NULL
};

#ifdef __cplusplus
extern "C" { 
#endif


PyObject * 
PyInit_threader(void)
{
    PyObject* m ;
    m = PyModule_Create(&moduledef);
    if (m == NULL)
        return NULL;
    
    struct module_state *st = GETSTATE(m);
    if (st == NULL)
        throw new std::runtime_error("GETSTATE returns null.");

    st->error = PyErr_NewException("threader.Error", NULL, NULL);
    if (st->error == NULL) {
        Py_DECREF(m);
        return NULL;
    }
    
    return m ;
}



#ifdef __cplusplus
}
#endif

