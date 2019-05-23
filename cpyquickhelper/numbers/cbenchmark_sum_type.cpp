#include "cbenchmark_sum_type.h"

#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <vector>
#include <thread>
#include <iterator>
#include "repeat_fct.h"

#ifndef SKIP_PYTHON
#include <pybind11/iostream.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
//#include <numpy/arrayobject.h>

namespace py = pybind11;
#endif

class FunctionMeasureVectorSumFloat : FunctionMeasureVector<float>
{
    public:
    FunctionMeasureVectorSumFloat(const std::vector<float> &v) :
        FunctionMeasureVector<float>(v) { }
    int run(int x)
    {
        // Do not forget to return a result which depends on the
        // computation, otherwise, the compiler might decide 
        // not to do it.
        return vector_float_sum(&values[0], values.size()) ? 1 : 0;
    }
};

class FunctionMeasureVectorSumDouble : FunctionMeasureVector<float>
{
    public:
    FunctionMeasureVectorSumDouble(const std::vector<float> &v) :
        FunctionMeasureVector<float>(v) { }
    int run(int x)
    {
        // Do not forget to return a result which depends on the
        // computation, otherwise, the compiler might decide 
        // not to do it.
        return vector_double_sum(&values[0], values.size()) > 0 ? 1 : 0;
    }
};


/////////////////////////
// dot product
/////////////////////////

#if defined(_MSC_VER)
#define STRING_CONCAT(A, B) A ## B
#define CBENCHMARK_ADDFUNC(suf, C)\
    m.def(STRING_CONCAT("measure_scenario_", #suf), \
            [](const std::vector<float> &values, int repeat=100, int number=1000, bool verbose=false) -> ExecutionStat { \
                ExecutionStat res; \
                FunctionMeasureVectorSum##suf fct(values); \
                repeat_execution<FunctionMeasureVectorSum##suf>(fct, repeat, number, res, verbose); \
                return res; \
            }, \
            C, \
            py::arg("values"), \
            py::arg("repeat")=100, py::arg("number")=10, \
            py::arg("verbose")=false);
#else
#define STRING_CONCAT_GCC(A, B) #A #B
#define CBENCHMARK_ADDFUNC(suf, C)\
    m.def(STRING_CONCAT_GCC(measure_scenario_, suf), \
            [](const std::vector<float> &values, int repeat=100, int number=1000, bool verbose=false) -> ExecutionStat { \
                ExecutionStat res; \
                FunctionMeasureVectorSum##suf fct(values); \
                repeat_execution<FunctionMeasureVectorSum##suf>(fct, repeat, number, res, verbose); \
                return res; \
            }, \
            C, \
            py::arg("values"), \
            py::arg("repeat")=100, py::arg("number")=10, \
            py::arg("verbose")=false);
#endif

/////////////////////
// vector_dot_product
/////////////////////

float vector_float_sum(const float *p1, size_t size)
{
    float sum = 0;
    const float * end1 = p1 + size;
    for(; p1 != end1; ++p1)
        sum += *p1;
    return sum;
}

float vector_double_sum(const float *p1, size_t size)
{
    double sum = 0;
    const float * end1 = p1 + size;
    for(; p1 != end1; ++p1)
        sum += (double)*p1;
    return (float)sum;
}


#ifndef SKIP_PYTHON

float py_vector_float_sum(py::array_t<float> v1)
{
    if (v1.ndim() != 1)
        throw std::runtime_error("Vector v1 must be a vector.");
    return vector_float_sum(v1.data(0), v1.shape(0));
}

float py_vector_double_sum(py::array_t<float> v1)
{
    if (v1.ndim() != 1)
        throw std::runtime_error("Vector v1 must be a vector.");
    return vector_double_sum(v1.data(0), v1.shape(0));
}

#endif



#ifndef SKIP_PYTHON
#define CBENCHMARK_MODULE_NAME "cpyquickhelper.numbers.cbenchmark"

// See https://github.com/pybind/pybind11/issues/616.
// Required to use ExecutionStat defined in cbenchmark.
template <> struct py::detail::type_caster<ExecutionStat> {
    PYBIND11_TYPE_CASTER(ExecutionStat, _("ExecutionStat"));

    bool load(handle src, bool) {
        if (!src) return false;
        value.number = src.attr("number").cast<int>();
        value.repeat = src.attr("repeat").cast<int>();
        value.average = src.attr("average").cast<double>();
        value.deviation = src.attr("deviation").cast<double>();
        value.deviation = src.attr("deviation").cast<double>();
        value.min_exec = src.attr("min_exec").cast<double>();
        value.max_exec = src.attr("max_exec").cast<double>();
        return true;
    }

    static handle cast(ExecutionStat v, return_value_policy /*policy*/, handle /*parent*/) {        
        py::object tv_py = py::module::import(CBENCHMARK_MODULE_NAME).attr("ExecutionStat")();
        tv_py.attr("number") = py::cast(v.number);
        tv_py.attr("repeat") = py::cast(v.repeat);
        tv_py.attr("average") = py::cast(v.average);
        tv_py.attr("deviation") = py::cast(v.deviation);
        tv_py.attr("min_exec") = py::cast(v.min_exec);
        tv_py.attr("max_exec") = py::cast(v.max_exec);
        return tv_py.release();
    }
};


PYBIND11_MODULE(cbenchmark_sum_type, m) {
	m.doc() =
    #if defined(__APPLE__)
    "Measures the execution time of functions implemented in C and about a vector sum."
    #else
    R"pbdoc(Measures the execution time of functions implemented in C, the measures are
also implemented in C. The functions proposes different implements of a
sum of all elements in vector.)pbdoc"
    #endif
    ;
    
    CBENCHMARK_ADDFUNC(Float, "Measure C++ implementation. Sum all elements with a float accumulator.");
    CBENCHMARK_ADDFUNC(Double, "Measure C++ implementation. Sum all elements with a double accumulator.");
        
    m.def("vector_float_sum", &py_vector_float_sum,
          "Computes a sum in C++ with vectors of floats and a float accumulator.");
    m.def("vector_double_sum", &py_vector_double_sum,
          "Computes a sum in C++ with vectors of floats and a double accumulator.");
}

#endif
