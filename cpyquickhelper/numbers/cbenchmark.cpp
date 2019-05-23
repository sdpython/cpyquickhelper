#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <vector>
#include <thread>
#include <iterator>
#include <iostream>
#include "repeat_fct.h"

#ifndef SKIP_PYTHON
#include <pybind11/iostream.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
//#include <numpy/arrayobject.h>

namespace py = pybind11;
#endif

std::string get_simd_available_option()
{
    std::string message = "";
    
#if defined(__SSE__) || (defined(_MSC_VER) && !defined(_M_CEE_PURE))
    // __SSE__ not defined by Visual Studio.
    message += " __SSE__";
#endif
#if defined(__SSE2__) || (defined(_MSC_VER) && !defined(_M_CEE_PURE))
    // __SSE2__ not defined by Visual Studio.
    message += " __SSE2__";
#endif
#if defined(__SSE3__) || (defined(_MSC_VER) && !defined(_M_CEE_PURE))
    // __SSE3__ not defined by Visual Studio.
    message += " __SSE3__";
#endif
#if defined(__SSE4_1__) || (defined(_MSC_VER) && !defined(_M_CEE_PURE))
    // __SSE4_1__ not defined by Visual Studio.
    message += " __SSE4_1__";
#endif
#if defined(__AVX__)
    message += " __AVX__";
#endif
#if defined(__AVX2__)
    message += " __AVX2__";
#endif
#if defined(__AVX512F__)
    message += " __AVX512F__";
#endif
#if defined(__AVX512DQ__)
    message += " __AVX512DQ__";
#endif
#if defined(__AVX512PF__)
    message += " __AVX512PF__";
#endif
#if defined(__AVX512ER__)
    message += " __AVX512ER__";
#endif
#if defined(__AVX512CD__)
    message += " __AVX512CD__";
#endif
#if defined(__AVX512BW__)
    message += " __AVX512BW__";
#endif
#if defined(__AVX512VL__)
    message += " __AVX512VL__";
#endif
#if defined(__FMA__)
    message += " __FMA__";
#endif
#if defined(__AVX512IFMA__)
    message += " __AVX512IFMA__";
#endif
#if defined(__F16C__)
    message += " __F16C__";
#endif
#if defined(__ARM_NEON__)
    message += " __ARM_NEON__";
#endif

    return message.empty() 
                ? "No available options." 
                : (std::string("Available options: ") + message);
}


#ifndef SKIP_PYTHON

PYBIND11_MODULE(cbenchmark, m) {
	m.doc() = "Commom classes to benchmark implementations in C."
    ;

    py::class_<ExecutionStat>(m, "ExecutionStat", 
        "Holds results to compare execution time of functions.")
        .def(py::init<>())
        .def_readwrite("number", &ExecutionStat::number, "number of executions being measured")
        .def_readwrite("repeat", &ExecutionStat::repeat, "number of times the experiment is repeated")
        .def_readwrite("average", &ExecutionStat::average, "average processing time")
        .def_readwrite("deviation", &ExecutionStat::deviation, "standard deviation")
        .def_readwrite("min_exec", &ExecutionStat::min_exec, "minimum execution time")
        .def_readwrite("max_exec", &ExecutionStat::max_exec, "maximum execution time")
        .def("__str__", [](const ExecutionStat& report) -> std::string {
            char buffer[1000];
            sprintf(buffer, "%1.3gs (+/- %1.3g) in [%1.3g, %1.3g] N=%d rep=%d",
                    report.average, report.deviation,
                    report.min_exec, report.max_exec,
                    report.number, report.repeat);
            return buffer;
            },
            "usual operator")
        .def("todict", [](const ExecutionStat& report) -> std::map<std::string, double> {
            std::map<std::string, double> res;
            res["average"] = report.average;
            res["deviation"] = report.deviation;
            res["min_exec"] = report.min_exec;
            res["max_exec"] = report.max_exec;
            res["number"] = report.number;
            res["repeat"] = report.repeat;
            return res;
        }, "Converts the structure into a dictionary.")
        ;

    m.def ("get_simd_available_option", &get_simd_available_option,
        #if defined(__APPLE__)
           "Returns the available compilation options for SIMD."
        #else
            R"pbdoc(Returns the available compilation options for SIMD.
It can simply be called with the following example:

.. runpython::
    :showcode:
    
    from cpyquickhelper.numbers.cbenchmark import get_simd_available_option
    print(get_simd_available_option())
)pbdoc"
        #endif
    );
}

#endif
