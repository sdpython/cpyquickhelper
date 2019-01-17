#include <pybind11/iostream.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
//#include <numpy/arrayobject.h>
#include <vector>
#include <thread>
#include <iterator>
#include "repeat_fct.h"

namespace py = pybind11;

template<typename DTYPE>
class FunctionMeasureVectorCount : FunctionMeasure
{
    protected:
        
    std::vector<DTYPE> values;
    DTYPE th;
    
    public:
        
    FunctionMeasureVectorCount(const std::vector<DTYPE> &v, DTYPE t) : values(v), th(t)
    { 
        if (values.size() == 0)
            throw std::runtime_error("Array to process must not be empty.");
    }
};


template<typename DTYPE>
class FunctionMeasureVectorCountSleep : FunctionMeasureVectorCount<DTYPE>
{
    public:
    FunctionMeasureVectorCountSleep(const std::vector<DTYPE> &v, DTYPE t) :
        FunctionMeasureVectorCount<DTYPE>(v, t) { }
    int run(int i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(th));
        return 0;
    }
}; 

#define REPEAT_INST(INST) {\
    INST INST INST INST INST\
    INST INST INST INST INST\
    }


template<typename DTYPE>
class FunctionMeasureVectorCountA : FunctionMeasureVectorCount<DTYPE>
{
    public:
    FunctionMeasureVectorCountA(const std::vector<DTYPE> &v, DTYPE t) :
        FunctionMeasureVectorCount<DTYPE>(v, t) { }
    int run(int x)
    {
        int nb = x % 1; // == 0 but otherwise the compiler detects the
                        // the code does not change depending on x
                        // and it caches the results.
        for(int i = 0; i < values.size(); ++i)
            REPEAT_INST(if (values[i] >= th) ++nb;)
        return nb;
    }
};


template<typename DTYPE>
class FunctionMeasureVectorCountB : FunctionMeasureVectorCount<DTYPE>
{
    public:
    FunctionMeasureVectorCountB(const std::vector<DTYPE> &v, DTYPE t) :
        FunctionMeasureVectorCount<DTYPE>(v, t) { }        
    int run(int x)
    {
        int nb = x % 1;
        for(auto it = values.begin(); it != values.end(); ++it)
            REPEAT_INST(if (*it >= th) ++nb;)
        return nb;
    }
};

template<typename DTYPE>
class FunctionMeasureVectorCountC : FunctionMeasureVectorCount<DTYPE>
{
    public:
    FunctionMeasureVectorCountC(const std::vector<DTYPE> &v, DTYPE t) :
        FunctionMeasureVectorCount<DTYPE>(v, t) { }        
    int run(int x)
    {
        int nb = x % 1;
        for(auto it = values.begin(); it != values.end(); ++it)
            REPEAT_INST(if (*it >= th) nb++;)
        return nb;
    }
};

template<typename DTYPE>
class FunctionMeasureVectorCountD : FunctionMeasureVectorCount<DTYPE>
{
    public:
    FunctionMeasureVectorCountD(const std::vector<DTYPE> &v, DTYPE t) :
        FunctionMeasureVectorCount<DTYPE>(v, t) { }        
    int run(int x)
    {
        int nb = x % 1;
        for(auto it = values.begin(); it != values.end(); ++it)
            REPEAT_INST(nb += *it >= th ? 1 : 0;)
        return nb;
    }
};

template<typename DTYPE>
class FunctionMeasureVectorCountE : FunctionMeasureVectorCount<DTYPE>
{
    public:
    FunctionMeasureVectorCountE(const std::vector<DTYPE> &v, DTYPE t) :
        FunctionMeasureVectorCount<DTYPE>(v, t) { }        
    int run(int x)
    {
        int nb = x % 1;
        for(auto it = values.begin(); it != values.end(); ++it)
            REPEAT_INST(if (*it >= th) nb += 1;)
        return nb;
    }
};

template<typename DTYPE>
class FunctionMeasureVectorCountF : FunctionMeasureVectorCount<DTYPE>
{
    public:
    FunctionMeasureVectorCountF(const std::vector<DTYPE> &v, DTYPE t) :
        FunctionMeasureVectorCount<DTYPE>(v, t) { }        
    int run(int x)
    {
        int nb = x % 1;
        for(auto it = values.begin(); it != values.end(); ++it)
            REPEAT_INST(nb += (*it - th) >= 0 ? 1 : 0;)
        return nb;
    }
};

template<typename DTYPE>
class FunctionMeasureVectorCountG : FunctionMeasureVectorCount<DTYPE>
{
    public:
    FunctionMeasureVectorCountG(const std::vector<DTYPE> &v, DTYPE t) :
        FunctionMeasureVectorCount<DTYPE>(v, t) { }        
    int run(int x)
    {
        int nb = x % 1;
        for(auto it = values.begin(); it != values.end(); ++it)
            REPEAT_INST(nb += (*it - th) < 0 ? 1 : 0;)
        return (int)std::distance(values.begin(), values.end()) - nb;
    }
};

template<typename DTYPE>
class FunctionMeasureVectorCountH : FunctionMeasureVectorCount<DTYPE>
{
    public:
    FunctionMeasureVectorCountH(const std::vector<DTYPE> &v, DTYPE t) :
        FunctionMeasureVectorCount<DTYPE>(v, t) { }        
    int run(int x)
    {
        int nb = x % 1;
        for(auto it = values.begin(); it != values.end(); ++it)
            REPEAT_INST(nb += *it < th ? 1 : 0;)
        return (int)std::distance(values.begin(), values.end()) - nb;
    }
};

template<typename DTYPE>
class FunctionMeasureVectorCountI : FunctionMeasureVectorCount<DTYPE>
{
    public:
    FunctionMeasureVectorCountI(const std::vector<DTYPE> &v, DTYPE t) :
        FunctionMeasureVectorCount<DTYPE>(v, t) { }        
    int run(int x)
    {
        int nb = x % 1;
        for(auto it = values.begin(); it != values.end(); ++it)
            REPEAT_INST(nb += 1 ^ ((unsigned int)(*it) >> (sizeof(int) * CHAR_BIT - 1));)
        return nb;
    }
};

template<typename DTYPE>
class FunctionMeasureVectorCountJ : FunctionMeasureVectorCount<DTYPE>
{
    public:
    FunctionMeasureVectorCountJ(const std::vector<DTYPE> &v, DTYPE t) :
        FunctionMeasureVectorCount<DTYPE>(v, t) { }
    int run(int x)
    {
        int nb = x % 1; // == 0 but otherwise the compiler detects the
                        // the code does not change depending on x
                        // and it caches the results.
        for(int i = 0; i < values.size(); ++i)
            REPEAT_INST(nb += values[i] >= th ? 1 : 0;)
        return nb;
    }
};



#if defined(_MSC_VER)
#define STRING_CONCAT(A, B) A ## B
#define CBENCHMARK_ADDFUNC(suf, C)\
    m.def(STRING_CONCAT("measure_scenario_", #suf), \
            [](const std::vector<int> &values, int th, int repeat=100, int number=1000, bool verbose=false) -> ExecutionStat { \
                ExecutionStat res; \
                FunctionMeasureVectorCount##suf<int> fct(values, th); \
                repeat_execution<FunctionMeasureVectorCount##suf<int>>(fct, repeat, number, res, verbose); \
                return res; \
            }, \
            C, \
            py::arg("values"), py::arg("th"), \
            py::arg("repeat")=10, py::arg("number")=100, \
            py::arg("verbose")=false);
#else
#define CBENCHMARK_ADDFUNC(suf, C)\
    m.def(measure_scenario_ ##suf), \
            [](const std::vector<int> &values, int th, int repeat=100, int number=1000, bool verbose=false) -> ExecutionStat { \
                ExecutionStat res; \
                FunctionMeasureVectorCount##suf<int> fct(values, th); \
                repeat_execution<FunctionMeasureVectorCount##suf<int>>(fct, repeat, number, res, verbose); \
                return res; \
            }, \
            C, \
            py::arg("values"), py::arg("th"), \
            py::arg("repeat")=10, py::arg("number")=100, \
            py::arg("verbose")=false);
#endif


PYBIND11_MODULE(cbenchmark, m) {
	m.doc() =     
    #if defined(__APPLE__)
    "Measures the execution time of functions implemented in C."
    #else
    R"pbdoc(Measures the execution time of functions implemented in C, the measures are
also implemented in C.)pbdoc"
    #endif
    ;
    
    py::class_<ExecutionStat>(m, "ExecutionStat", 
        "Holds results to compare execution time of functions.")
        .def(py::init<>())
        .def_readwrite("number", &ExecutionStat::number, "number of execution")
        .def_readwrite("repeat", &ExecutionStat::repeat, "number of calls per executions")
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
            
    CBENCHMARK_ADDFUNC(A, "Measure C++ implementation. Loop on ``if (values[i] >= th) ++nb;``");
    CBENCHMARK_ADDFUNC(B, "Measure C++ implementation. Loop on ``if (*it >= th) ++nb;``");
    CBENCHMARK_ADDFUNC(C, "Measure C++ implementation. Loop on ``if (*it >= th) nb++;``");
    CBENCHMARK_ADDFUNC(D, "Measure C++ implementation. Loop on ``nb += *it >= th ? 1 : 0;``");
    CBENCHMARK_ADDFUNC(E, "Measure C++ implementation. Loop on ``if (*it >= th) nb += 1;``");
    CBENCHMARK_ADDFUNC(F, "Measure C++ implementation. Loop on ``nb += (*it - th) >= 0 ? 1 : 0;``");
    CBENCHMARK_ADDFUNC(G, "Measure C++ implementation. Loop on ``nb += (*it - th) < 0 ? 1 : 0;``");
    CBENCHMARK_ADDFUNC(H, "Measure C++ implementation. Loop on ``nb += *it < th ? 1 : 0;``");
    CBENCHMARK_ADDFUNC(I, "Measure C++ implementation. Loop on ``nb += 1 ^ ((unsigned int)(*it) >> (sizeof(int) * CHAR_BIT - 1));``");
    CBENCHMARK_ADDFUNC(J, "Measure C++ implementation. Loop on ``nb += values[i] >= th ? 1 : 0;``");        
    CBENCHMARK_ADDFUNC(Sleep, "Measure C++ implementation. Loop on ``sleep``");        
}
