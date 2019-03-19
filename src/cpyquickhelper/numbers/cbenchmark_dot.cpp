#include "cbenchmark_dot.h"

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


template<typename DTYPE>
class FunctionMeasureVectorCount : FunctionMeasure
{
    protected:

    std::vector<DTYPE> values;
    DTYPE th;

    public:

    FunctionMeasureVectorCount(const std::vector<DTYPE> &v, DTYPE t) : values(v), th(t)
    {
        if (this->values.size() == 0)
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
        std::this_thread::sleep_for(std::chrono::milliseconds(this->th));
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
        for(int i = 0; i < this->values.size(); ++i)
            REPEAT_INST(if (this->values[i] >= this->th) ++nb;)
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
        for(auto it = this->values.begin(); it != this->values.end(); ++it)
            REPEAT_INST(if (*it >= this->th) ++nb;)
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
        for(auto it = this->values.begin(); it != this->values.end(); ++it)
            REPEAT_INST(if (*it >= this->th) nb++;)
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
        for(auto it = this->values.begin(); it != this->values.end(); ++it)
            REPEAT_INST(nb += *it >= this->th ? 1 : 0;)
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
        for(auto it = this->values.begin(); it != this->values.end(); ++it)
            REPEAT_INST(if (*it >= this->th) nb += 1;)
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
        for(auto it = this->values.begin(); it != this->values.end(); ++it)
            REPEAT_INST(nb += (*it - this->th) >= 0 ? 1 : 0;)
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
        for(auto it = this->values.begin(); it != this->values.end(); ++it)
            REPEAT_INST(nb += (*it - this->th) < 0 ? 1 : 0;)
        return (int)std::distance(this->values.begin(), this->values.end()) - nb;
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
        for(auto it = this->values.begin(); it != this->values.end(); ++it)
            REPEAT_INST(nb += *it < this->th ? 1 : 0;)
        return (int)std::distance(this->values.begin(), this->values.end()) - nb;
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
        for(auto it = this->values.begin(); it != this->values.end(); ++it)
            REPEAT_INST(nb += 1 ^ ((unsigned int)(*it - this->th) >> (sizeof(int) * CHAR_BIT - 1));)
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
        for(int i = 0; i < this->values.size(); ++i)
            REPEAT_INST(nb += this->values[i] >= this->th ? 1 : 0;)
        return nb;
    }
};


/////////////////////////
// dot product
/////////////////////////

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
            py::arg("repeat")=100, py::arg("number")=10, \
            py::arg("verbose")=false);
#else
#define STRING_CONCAT_GCC(A, B) #A #B
#define CBENCHMARK_ADDFUNC(suf, C)\
    m.def(STRING_CONCAT_GCC(measure_scenario_, suf), \
            [](const std::vector<int> &values, int th, int repeat=100, int number=1000, bool verbose=false) -> ExecutionStat { \
                ExecutionStat res; \
                FunctionMeasureVectorCount##suf<int> fct(values, th); \
                repeat_execution<FunctionMeasureVectorCount##suf<int>>(fct, repeat, number, res, verbose); \
                return res; \
            }, \
            C, \
            py::arg("values"), py::arg("th"), \
            py::arg("repeat")=100, py::arg("number")=10, \
            py::arg("verbose")=false);
#endif

/////////////////////
// vector_dot_product
/////////////////////

float vector_dot_product_pointer(const float *p1, const float *p2, size_t size)
{
    float sum = 0;
    const float * end1 = p1 + size;
    for(; p1 != end1; ++p1, ++p2)
        sum += *p1 * *p2;
    return sum;
}


#ifndef SKIP_PYTHON

float vector_dot_product(py::array_t<float> v1, py::array_t<float> v2)
{
    if (v1.ndim() != v2.ndim())
        throw std::runtime_error("Vector v1 and v2 must have the same dimension.");
    if (v1.ndim() != 1)
        throw std::runtime_error("Vector v1 and v2 must be vectors.");
    return vector_dot_product_pointer(v1.data(0), v2.data(0), v1.shape(0));
}

#endif


// empty_vector_dot_product

#ifndef SKIP_PYTHON

float empty_vector_dot_product(py::array_t<float> v1, py::array_t<float> v2)
{
    return 0;
}

#endif

/////////////////////
// vector_dot_product16
/////////////////////

float vector_dot_product_pointer16(const float *p1, const float *p2)
{
    float sum = 0;
    
    sum += *(p1++) * *(p2++);
    sum += *(p1++) * *(p2++);
    sum += *(p1++) * *(p2++);
    sum += *(p1++) * *(p2++);
    sum += *(p1++) * *(p2++);
    sum += *(p1++) * *(p2++);
    sum += *(p1++) * *(p2++);
    sum += *(p1++) * *(p2++);

    sum += *(p1++) * *(p2++);
    sum += *(p1++) * *(p2++);
    sum += *(p1++) * *(p2++);
    sum += *(p1++) * *(p2++);
    sum += *(p1++) * *(p2++);
    sum += *(p1++) * *(p2++);
    sum += *(p1++) * *(p2++);
    sum += *(p1++) * *(p2++);

    return sum;
}

#define BYN 16

float vector_dot_product_pointer16(const float *p1, const float *p2, size_t size)
{
    float sum = 0;
    size_t i = 0;
    if (size >= BYN) {
        size_t size_ = size - BYN;
        for(; i < size_; i += BYN, p1 += BYN, p2 += BYN)
            sum += vector_dot_product_pointer16(p1, p2);
    }
    for(; i < size; ++p1, ++p2, ++i)
        sum += *p1 * *p2;
    return sum;
}

#ifndef SKIP_PYTHON

float vector_dot_product16(py::array_t<float> v1, py::array_t<float> v2)
{
    if (v1.ndim() != v2.ndim())
        throw std::runtime_error("Vector v1 and v2 must have the same dimension.");
    if (v1.ndim() != 1)
        throw std::runtime_error("Vector v1 and v2 must be vectors.");
    return vector_dot_product_pointer16(v1.data(0), v2.data(0), v1.shape(0));
}

#endif

/////////////////////
// vector_dot_product16_nofcall
/////////////////////

float vector_dot_product_pointer16_nofcall(const float *p1, const float *p2, size_t size)
{
    float sum = 0;    
    const float * end = p1 + size;
    if (size >= BYN) {
        #if(BYN != 16)
            #error "BYN must be equal to 16";
        #endif
        unsigned int size_ = (unsigned int) size;
        size_ = size_ >> 4;  // division by 16=2^4
        size_ = size_ << 4;  // multiplication by 16=2^4
        const float * end_ = p1 + size_;
        for(; p1 != end_;)
        {
            sum += *p1 * *p2; ++p1, ++p2;
            sum += *p1 * *p2; ++p1, ++p2;
            sum += *p1 * *p2; ++p1, ++p2;
            sum += *p1 * *p2; ++p1, ++p2;
            
            sum += *p1 * *p2; ++p1, ++p2;
            sum += *p1 * *p2; ++p1, ++p2;
            sum += *p1 * *p2; ++p1, ++p2;
            sum += *p1 * *p2; ++p1, ++p2;

            sum += *p1 * *p2; ++p1, ++p2;
            sum += *p1 * *p2; ++p1, ++p2;
            sum += *p1 * *p2; ++p1, ++p2;
            sum += *p1 * *p2; ++p1, ++p2;
            
            sum += *p1 * *p2; ++p1, ++p2;
            sum += *p1 * *p2; ++p1, ++p2;
            sum += *p1 * *p2; ++p1, ++p2;
            sum += *p1 * *p2; ++p1, ++p2;
        }
    }
    for(; p1 != end; ++p1, ++p2)
        sum += *p1 * *p2;
    return sum;
}


#ifndef SKIP_PYTHON

float vector_dot_product16_nofcall(py::array_t<float> v1, py::array_t<float> v2)
{
    if (v1.ndim() != v2.ndim())
        throw std::runtime_error("Vector v1 and v2 must have the same dimension.");
    if (v1.ndim() != 1)
        throw std::runtime_error("Vector v1 and v2 must be vectors.");
    return vector_dot_product_pointer16_nofcall(v1.data(0), v2.data(0), v1.shape(0));
}

#endif

/////////////////////
// vector_dot_product_16_sse
/////////////////////

#include <xmmintrin.h>

float vector_dot_product_pointer16_sse(const float *p1, const float *p2)
{
    __m128 c1 = _mm_load_ps(p1);
    __m128 c2 = _mm_load_ps(p2);
    __m128 r1 = _mm_mul_ps(c1, c2);
    
    p1 += 4;
    p2 += 4;
    
    c1 = _mm_load_ps(p1);
    c2 = _mm_load_ps(p2);
    r1 = _mm_add_ps(r1, _mm_mul_ps(c1, c2));
    
    p1 += 4;
    p2 += 4;
    
    c1 = _mm_load_ps(p1);
    c2 = _mm_load_ps(p2);
    r1 = _mm_add_ps(r1, _mm_mul_ps(c1, c2));
    
    p1 += 4;
    p2 += 4;
    
    c1 = _mm_load_ps(p1);
    c2 = _mm_load_ps(p2);
    r1 = _mm_add_ps(r1, _mm_mul_ps(c1, c2));

    float r[4];
    _mm_store_ps(r, r1);

    return r[0] + r[1] + r[2] + r[3];
}

#define BYN 16

float vector_dot_product_pointer16_sse(const float *p1, const float *p2, size_t size)
{
    float sum = 0;
    size_t i = 0;
    if (size >= BYN) {
        size_t size_ = size - BYN;
        for(; i < size_; i += BYN, p1 += BYN, p2 += BYN)
            sum += vector_dot_product_pointer16_sse(p1, p2);
    }
    for(; i < size; ++p1, ++p2, ++i)
        sum += *p1 * *p2;
    return sum;
}

#ifndef SKIP_PYTHON

float vector_dot_product16_sse(py::array_t<float> v1, py::array_t<float> v2)
{
    if (v1.ndim() != v2.ndim())
        throw std::runtime_error("Vector v1 and v2 must have the same dimension.");
    if (v1.ndim() != 1)
        throw std::runtime_error("Vector v1 and v2 must be vectors.");
    return vector_dot_product_pointer16_sse(v1.data(0), v2.data(0), v1.shape(0));
}

#endif

/////////////////////
// vector_dot_product16_avx512
/////////////////////

#include <immintrin.h>

#if defined(__AVX512F__)

float vector_dot_product_pointer16_avx512(const float *p1, const float *p2)
{
    float sum = 0;
    
    __m512 c1 = _mm512_load_ps(p1);
    __m512 c2 = _mm512_load_ps(p2);
    __m512 r1 = _mm512_mul_ps(c1, c2);
    
    float r[16];
    _mm512_store_ps(r, r1);

    return r[0] + r[1] + r[2] + r[3] + 
           r[4] + r[5] + r[6] + r[7] +
           r[8] + r[9] + r[10] + r[11] + 
           r[12] + r[13] + r[14] + r[15]; 
}

#define BYN 16

float vector_dot_product_pointer16_avx512(const float *p1, const float *p2, size_t size)
{
    float sum = 0;
    size_t i = 0;
    if (size >= BYN) {
        size_t size_ = size - BYN;
        for(; i < size_; i += BYN, p1 += BYN, p2 += BYN)
            sum += vector_dot_product_pointer16_avx512(p1, p2);
    }
    for(; i < size; ++p1, ++p2, ++i)
        sum += *p1 * *p2;
    return sum;
}

#ifndef SKIP_PYTHON

float vector_dot_product16_avx512(py::array_t<float> v1, py::array_t<float> v2)
{
    if (v1.ndim() != v2.ndim())
        throw std::runtime_error("Vector v1 and v2 must have the same dimension.");
    if (v1.ndim() != 1)
        throw std::runtime_error("Vector v1 and v2 must be vectors.");
    return vector_dot_product_pointer16_avx512(v1.data(0), v2.data(0), v1.shape(0));
}

#endif

#endif

/////////////////////
// get_simd_available_option
/////////////////////

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

PYBIND11_MODULE(cbenchmark_dot, m) {
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
        
    m.def("vector_dot_product", &vector_dot_product,
          "Computes a dot product in C++ with vectors of floats.");
    m.def("empty_vector_dot_product", &empty_vector_dot_product,
          "Empty measure to have an idea about the processing due to python binding.");
    m.def("vector_dot_product16", &vector_dot_product16,
          "Computes a dot product in C++ with vectors of floats. Goes 16 by 16.");
    m.def("vector_dot_product16_nofcall", &vector_dot_product16_nofcall,
          "Computes a dot product in C++ with vectors of floats. Goes 16 by 16. Do not call intermediate functions.");
    m.def("vector_dot_product16_sse", &vector_dot_product16_sse,
          "Computes a dot product in C++ with vectors of floats. Goes 16 by 16. Use SSE instructions.");

#if defined(__AVX512F__)          
    m.def("vector_dot_product16_avx512", &vector_dot_product16_avx512,
          "Computes a dot product in C++ with vectors of floats. Goes 16 by 16. Use AVX 512 instructions because ``__AVX512F__`` is defined.");
#else
    m.def("vector_dot_product16_avx512", &vector_dot_product16_sse,
          "Computes a dot product in C++ with vectors of floats. Goes 16 by 16. Use SSE instructions because ``__AVX512F__`` is not defined.");
#endif

    m.def ("get_simd_available_option", &get_simd_available_option,
        #if defined(__APPLE__)
           "Returns the available compilation options for SIMD."
        #else
            R"pbdoc(Returns the available compilation options for SIMD.
It can simply be called with the following example:

.. runpython::
    :showcode:
    
    from cpyquickhelper.numbers.cbenchmark_dot import get_simd_available_option
    print(get_simd_available_option())
)pbdoc"
        #endif
    );

}

#endif
