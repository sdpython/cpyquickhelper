#include "Python.h"
#include "frameobject.h"
#include "robinhood.h"
#include <unordered_map>
#include <iostream>


struct hash_pair {
    template <typename T1, typename T2>
    size_t operator()(const std::pair<T1, T2>& p) const {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};


template<typename TK, typename T>
class PyFastDict {
    public:
        PyFastDict() : _map() {}
        ~PyFastDict() {}
        inline const T& get(const TK& name, const int64_t& key, const T& default_value) const {
            auto it = _map.find(std::pair<int64_t, TK>(key, name));
            return (it == _map.end()) ? default_value : it->second;
        }
        inline void insert(const TK& name, const int64_t& key, const T& value) {
            _map[std::pair<int64_t, TK>(key, name)] = value;
        }
        size_t size() const { return _map.size() ; }

    protected:
        std::unordered_map<std::pair<int64_t, TK>, T, hash_pair> _map;
};


class PyFastDictStrInt64: public PyFastDict<std::string, int64_t> {
};


struct hash_pair_rh {
    template <typename T1, typename T2>
    size_t operator()(const std::pair<T1, T2>& p) const {
        auto hash1 = robin_hood::hash<T1>{}(p.first);
        auto hash2 = robin_hood::hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};


template<typename TK, typename T>
class PyFastDictRH {
    public:
        PyFastDictRH() : _map() {}
        ~PyFastDictRH() {}
        inline const T& get(const TK& name, const int64_t& key, const T& default_value) const {
            auto it = _map.find(std::pair<int64_t, TK>(key, name));
            return (it == _map.end()) ? default_value : it->second;
        }
        inline void insert(const TK& name, const int64_t& key, const T& value) {
            _map[std::pair<int64_t, TK>(key, name)] = value;
        }
        size_t size() const { return _map.size() ; }

    protected:
        robin_hood::unordered_flat_map<std::pair<int64_t, TK>, T, hash_pair_rh> _map;
};


class PyFastDictRHStrInt64: public PyFastDictRH<std::string, int64_t> {
};


// See https://docs.python.org/3/c-api/stable.html#contents-of-limited-api.

void UnorderedMapStrInt64_Destructor(PyObject *caps) {
    PyFastDictStrInt64* ptr = (PyFastDictStrInt64*)PyCapsule_GetPointer(caps, "UnorderedMapStrInt64");
    if (ptr == 0) {
        PyErr_SetString(PyExc_ValueError, "Capsule pointer is null.");
        return;
    }
    delete ptr;
}


PyObject* UnorderedMapStrInt64_Create(PyObject* Py_UNUSED(self), PyObject* args) {
    auto * obj = new PyFastDictStrInt64();
    return PyCapsule_New(obj, "UnorderedMapStrInt64", &UnorderedMapStrInt64_Destructor);
}


void UnorderedMapRHStrInt64_Destructor(PyObject *caps) {
    PyFastDictRHStrInt64* ptr = (PyFastDictRHStrInt64*)PyCapsule_GetPointer(caps, "UnorderedMapRHStrInt64");
    if (ptr == 0) {
        PyErr_SetString(PyExc_ValueError, "Capsule pointer is null.");
        return;
    }
    delete ptr;
}


PyObject* UnorderedMapRHStrInt64_Create(PyObject* Py_UNUSED(self), PyObject* args) {
    auto * obj = new PyFastDictRHStrInt64();
    return PyCapsule_New(obj, "UnorderedMapRHStrInt64", &UnorderedMapRHStrInt64_Destructor);
}


PyObject* UnorderedMapStrInt64_Insert(PyObject* Py_UNUSED(self), PyObject* args) {
    PyObject* caps;
    const char* name;
    int64_t key, value;
    if(!PyArg_ParseTuple(args, "OsLL", &caps, &name, &key, &value)) {
        PyErr_SetString(
            PyExc_TypeError, "Unable to decode the parameters. (capsule, str, int64, int64) are expected.");
        return 0;
    }

    PyFastDictStrInt64* ptr = (PyFastDictStrInt64*)PyCapsule_GetPointer(caps, "UnorderedMapStrInt64");
    if (ptr == 0) {
        PyErr_SetString(PyExc_ValueError, "Capsule pointer is null.");
        return 0;
    }
    
    ptr->insert(name, key, value);
    Py_RETURN_NONE;
}


PyObject* UnorderedMapStrInt64_Get(PyObject* Py_UNUSED(self), PyObject* args) {
    PyObject* caps;
    const char* name;
    int64_t key, value;
    if(!PyArg_ParseTuple(args, "OsLL", &caps, &name, &key, &value)) {
        PyErr_SetString(
            PyExc_TypeError, "Unable to decode the parameters. (capsule, str, int64, int64) are expected.");
        return 0;
    }

    PyFastDictStrInt64* ptr = (PyFastDictStrInt64*)PyCapsule_GetPointer(caps, "UnorderedMapStrInt64");
    if (ptr == 0) {
        PyErr_SetString(PyExc_ValueError, "Capsule pointer is null.");
        return 0;
    }
    
    int64_t res = ptr->get(name, key, value);
    PyGILState_STATE state = PyGILState_Ensure();
    PyObject* obj = PyLong_FromLongLong(res);
    PyGILState_Release(state);
    
    return obj;
}


PyObject* UnorderedMapStrInt64_Insert_Fast(PyObject* Py_UNUSED(self), PyObject *const *args, Py_ssize_t nargs) {
    if (nargs != 4) {
        PyErr_SetString(
            PyExc_TypeError, "Unable to decode the parameters. (capsule, str, int64, int64) are expected.");
        return 0;
    }
    PyObject* caps = args[0];
    const char* name = PyUnicode_AsUTF8(args[1]);
    int64_t key = PyLong_AsLongLong(args[2]);
    int64_t value = PyLong_AsLongLong(args[3]);

    PyFastDictStrInt64* ptr = (PyFastDictStrInt64*)PyCapsule_GetPointer(caps, "UnorderedMapStrInt64");
    if (ptr == 0) {
        PyErr_SetString(PyExc_ValueError, "Capsule pointer is null.");
        return 0;
    }
    
    ptr->insert(name, key, value);
    Py_RETURN_NONE;
}


PyObject* UnorderedMapStrInt64_Get_Fast(PyObject* Py_UNUSED(self), PyObject *const *args, Py_ssize_t nargs) {
    if (nargs != 4) {
        PyErr_SetString(
            PyExc_TypeError, "Unable to decode the parameters. (capsule, str, int64, int64) are expected.");
        return 0;
    }
    PyObject* caps = args[0];
    const char* name = PyUnicode_AsUTF8(args[1]);
    int64_t key = PyLong_AsLongLong(args[2]);
    int64_t value = PyLong_AsLongLong(args[3]);

    PyFastDictStrInt64* ptr = (PyFastDictStrInt64*)PyCapsule_GetPointer(caps, "UnorderedMapStrInt64");
    if (ptr == 0) {
        PyErr_SetString(PyExc_ValueError, "Capsule pointer is null.");
        return 0;
    }
    
    int64_t res = ptr->get(name, key, value);
    PyGILState_STATE state = PyGILState_Ensure();
    PyObject* obj = PyLong_FromLongLong(res);
    PyGILState_Release(state);
    
    return obj;
}


PyObject* UnorderedMapRHStrInt64_Insert_Fast(PyObject* Py_UNUSED(self), PyObject *const *args, Py_ssize_t nargs) {
    if (nargs != 4) {
        PyErr_SetString(
            PyExc_TypeError, "Unable to decode the parameters. (capsule, str, int64, int64) are expected.");
        return 0;
    }
    PyObject* caps = args[0];
    const char* name = PyUnicode_AsUTF8(args[1]);
    int64_t key = PyLong_AsLongLong(args[2]);
    int64_t value = PyLong_AsLongLong(args[3]);

    PyFastDictRHStrInt64* ptr = (PyFastDictRHStrInt64*)PyCapsule_GetPointer(caps, "UnorderedMapRHStrInt64");
    if (ptr == 0) {
        PyErr_SetString(PyExc_ValueError, "Capsule pointer is null.");
        return 0;
    }
    
    ptr->insert(name, key, value);
    Py_RETURN_NONE;
}


PyObject* UnorderedMapRHStrInt64_Get_Fast(PyObject* Py_UNUSED(self), PyObject *const *args, Py_ssize_t nargs) {
    if (nargs != 4) {
        PyErr_SetString(
            PyExc_TypeError, "Unable to decode the parameters. (capsule, str, int64, int64) are expected.");
        return 0;
    }
    PyObject* caps = args[0];
    const char* name = PyUnicode_AsUTF8(args[1]);
    int64_t key = PyLong_AsLongLong(args[2]);
    int64_t value = PyLong_AsLongLong(args[3]);

    PyFastDictRHStrInt64* ptr = (PyFastDictRHStrInt64*)PyCapsule_GetPointer(caps, "UnorderedMapRHStrInt64");
    if (ptr == 0) {
        PyErr_SetString(PyExc_ValueError, "Capsule pointer is null.");
        return 0;
    }
    
    int64_t res = ptr->get(name, key, value);
    PyGILState_STATE state = PyGILState_Ensure();
    PyObject* obj = PyLong_FromLongLong(res);
    PyGILState_Release(state);
    
    return obj;
}


static PyMethodDef module_methods[] = {
    {"UnorderedMapStrInt64_Create", UnorderedMapStrInt64_Create, METH_NOARGS,
        "Creates an unordered map `{ (str, int): int }`."},
    {"UnorderedMapRHStrInt64_Create", UnorderedMapRHStrInt64_Create, METH_NOARGS,
        "Creates a faster unordered map `{ (str, int): int }`.\\n"
        "This map is using `robin_hood implementation "
        "<https://github.com/martinus/robin-hood-hashing>`_."},
    {"UnorderedMapStrInt64_Insert", UnorderedMapStrInt64_Insert, METH_VARARGS,
        "Inserts a value into a map `{ (str, int): int }`"
        ":param obj: unordered_map\n"
        ":param name: str\n"
        ":param key: int\n"
        ":param value: int"},
    {"UnorderedMapStrInt64_Insert_Fast", (PyCFunction)UnorderedMapStrInt64_Insert_Fast, METH_FASTCALL,
        "Inserts a value into a map `{ (str, int): int }`"
        ":param obj: unordered_map\n"
        ":param name: str\n"
        ":param key: int\n"
        ":param value: int"},
    {"UnorderedMapRHStrInt64_Insert_Fast", (PyCFunction)UnorderedMapRHStrInt64_Insert_Fast, METH_FASTCALL,
        "Inserts a value into a map `{ (str, int): int }`"
        ":param obj: unordered_map\n"
        ":param name: str\n"
        ":param key: int\n"
        ":param value: int"},
    {"UnorderedMapStrInt64_Get", UnorderedMapStrInt64_Get, METH_VARARGS,
        "Inserts a value into a map `{ (str, int): int }`"
        ":param obj: unordered_map\n"
        ":param name: str\n"
        ":param key: int\n"
        ":param value: int\n"
        ":return: int"},
    {"UnorderedMapStrInt64_Get_Fast", (PyCFunction)UnorderedMapStrInt64_Get_Fast, METH_FASTCALL,
        "Inserts a value into a map `{ (str, int): int }`"
        ":param obj: unordered_map\n"
        ":param name: str\n"
        ":param key: int\n"
        ":param value: int\n"
        ":return: int"},
    {"UnorderedMapRHStrInt64_Get_Fast", (PyCFunction)UnorderedMapRHStrInt64_Get_Fast, METH_FASTCALL,
        "Inserts a value into a map `{ (str, int): int }`"
        ":param obj: unordered_map\n"
        ":param name: str\n"
        ":param key: int\n"
        ":param value: int\n"
        ":return: int"},
        
    {0, 0, 0, 0} // End of list
};


PyMODINIT_FUNC
PyInit_fast_dict_c(void) {
    static struct PyModuleDef theModuleDef = {
        PyModuleDef_HEAD_INIT, "fast_dict_c",
        "Fast dictionary using Python C API.",
        -1, module_methods, 0, 0, 0, 0 };
    PyObject* m = PyModule_Create(&theModuleDef);
    return m;
}
