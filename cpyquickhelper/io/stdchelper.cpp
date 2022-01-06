// Source: https://stackoverflow.com/questions/5419356/redirect-stdout-stderr-to-a-string 
// 
//

#include "stdcapture.hpp"
#include <stdio.h>
#include <iostream>
// needed for Python>=3.10
#define PY_SSIZE_T_CLEAN
#include "Python.h"

struct module_state {
    PyObject *error;
};

#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))


///////////////////////////////////////////////
//////////// python function //////////////////
///////////////////////////////////////////////

static PyObject* begin_capture(PyObject *self, PyObject *unused) {
    StdCaptureStatic::BeginCapture();
    PyObject* res = Py_None;
    Py_INCREF(res);
    return res;
}

static PyObject* end_capture(PyObject *self, PyObject *unused) {
    StdCaptureStatic::EndCapture();
    PyObject* res = Py_None;
    Py_INCREF(res);
    return res;
}

static PyObject* get_capture(PyObject *self, PyObject *unused) {
    std::vector<unsigned char> out, err;
    StdCaptureStatic::GetCapture(out, err);
    PyObject * message;
    message = Py_BuildValue("y#", out.data(), out.size());
    if (message == NULL) {
        throw std::runtime_error("Wrong conversion into bytes array.");
    }
    // Py_INCREF(message);
    return message;
}

static PyObject* cprint(PyObject *self, PyObject* unicode) {
    if (unicode == NULL) {
        struct module_state *st = GETSTATE(self);
        PyErr_SetString(st->error, "Null pointer is not allowed.");    
        return 0;        
    }
    
    const Py_UNICODE *uni;
    if (!PyArg_ParseTuple(unicode, "u", &uni))
        return NULL;
    wprintf(uni);

    PyObject* res = Py_None;
    Py_INCREF(res);
    return res;
}


////////////////////////////////////////////////////
//////////// module definition /////////////////////
////////////////////////////////////////////////////

const char * module_name = "cpyquickhelper.io.stdchelper" ;

static int stdchelper_module_traverse(PyObject *m, visitproc visit, void *arg) {
    Py_VISIT(GETSTATE(m)->error);
    return 0;
}

static int stdchelper_module_clear(PyObject *m) {
    Py_CLEAR(GETSTATE(m)->error);
    return 0;
}

static PyMethodDef fonctions [] = {
  {"begin_capture", begin_capture, METH_VARARGS, 
    "Starts capturing the standard output. Signature: *def begin_capture():*."},
  {"end_capture",  end_capture, METH_VARARGS,
    "Stops capturing the standard output. Signature: *def end_capture():*."},
  {"get_capture",  get_capture, METH_VARARGS,
    "Gets the captured output. Signature: *def get_capture() -> (str, str):*."},
  {"cprint",  cprint, METH_VARARGS,
    "Displays a string on the standard output in C++. Signature: *def cprint(*args):*."},
  {NULL, NULL}
} ;

static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        module_name,
        "Helper for IO with C++.",
        sizeof(struct module_state),
        fonctions,
        NULL,
        stdchelper_module_traverse,
        stdchelper_module_clear,
        NULL
};

#ifdef __cplusplus
extern "C" { 
#endif


PyObject * 
PyInit_stdchelper(void) {
    PyObject* m ;
    m = PyModule_Create(&moduledef);
    if (m == NULL)
        return NULL;
    
    struct module_state *st = GETSTATE(m);
    if (st == NULL)
        throw new std::runtime_error("GETSTATE returns null.");

    st->error = PyErr_NewException("stdchelper.Error", NULL, NULL);
    if (st->error == NULL) {
        Py_DECREF(m);
        return NULL;
    }
    
    return m ;
}


#ifdef __cplusplus
}
#endif
