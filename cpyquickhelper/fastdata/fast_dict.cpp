#ifndef SKIP_PYTHON
#include <unordered_map>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

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
        const T& get(const TK& name, const int64_t& key, const T& default_value) const {
            auto it = _map.find(std::pair<int64_t, TK>(key, name));
            return (it == _map.end()) ? default_value : it->second;
        }
        void insert(const TK& name, const int64_t& key, const T& value) {
            _map[std::pair<int64_t, TK>(key, name)] = value;
        }
        size_t size() const { return _map.size() ; }

    protected:
        std::unordered_map<std::pair<int64_t, TK>, T, hash_pair> _map;
};


class PyFastDictStrInt64: public PyFastDict<std::string, int64_t> {
};


class PyFastDictInt64Int64: public PyFastDict<int64_t, int64_t> {
};


class PyFastDictIntInt: public PyFastDict<int, int> {
};


PYBIND11_MODULE(fast_dict, m) {
	m.doc() = "Implements faster dictionaries for specific cases.";

	py::class_<PyFastDictStrInt64> c1 (m, "FastDictStrInt64", 
    #if defined(__APPLE__)
    "Dictionary `{ str, int64: int64 }`, faster than the python version."
    #else
    R"pbdoc(Dictionary `{ str, int64: int64 }, faster than the python version.`.
)pbdoc"
    #endif
        );
	c1.def(py::init<>(), "Constructor");
    c1.def("__len__", &PyFastDictStrInt64::size,
           "Returns the number of stored elements.");
    c1.def("get", &PyFastDictStrInt64::get,
           R"pbdoc("Returns an integer indexed by `name,key`.)pbdoc");
    c1.def("insert", &PyFastDictStrInt64::insert,
           R"pbdoc("Inserts an integer indexed by `name,key`.)pbdoc");

	py::class_<PyFastDictInt64Int64> c2 (m, "FastDictInt64Int64", 
    #if defined(__APPLE__)
    "Dictionary `{ int64, int64: int64 }`, faster than the python version."
    #else
    R"pbdoc(Dictionary `{ int64, int64: int64 }, faster than the python version.`.
)pbdoc"
    #endif
        );
	c2.def(py::init<>(), "Constructor");
    c2.def("__len__", &PyFastDictInt64Int64::size,
           "Returns the number of stored elements.");
    c2.def("get", &PyFastDictInt64Int64::get,
           R"pbdoc("Returns an integer indexed by `name,key`.)pbdoc");
    c2.def("insert", &PyFastDictInt64Int64::insert,
           R"pbdoc("Inserts an integer indexed by `name,key`.)pbdoc");

	py::class_<PyFastDictIntInt> c3 (m, "FastDictIntInt", 
    #if defined(__APPLE__)
    "Dictionary `{ int, int: int }`, faster than the python version."
    #else
    R"pbdoc(Dictionary `{ int64, int64: int64 }, faster than the python version.`.
)pbdoc"
    #endif
        );
	c3.def(py::init<>(), "Constructor");
    c3.def("__len__", &PyFastDictIntInt::size,
           "Returns the number of stored elements.");
    c3.def("get", &PyFastDictIntInt::get,
           R"pbdoc("Returns an integer indexed by `name,key`.)pbdoc");
    c3.def("insert", &PyFastDictIntInt::insert,
           R"pbdoc("Inserts an integer indexed by `name,key`.)pbdoc");
}

#endif
