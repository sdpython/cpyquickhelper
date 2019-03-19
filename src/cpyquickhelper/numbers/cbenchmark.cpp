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
}

#endif
