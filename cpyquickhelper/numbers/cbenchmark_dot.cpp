#include "cbenchmark_dot.h"

#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifdef USE_OPENMP
#include "omp.h"
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
class FunctionMeasureVectorCountSleep : FunctionMeasureVectorThreshold<DTYPE>
{
    public:
    FunctionMeasureVectorCountSleep(const std::vector<DTYPE> &v, DTYPE t) :
        FunctionMeasureVectorThreshold<DTYPE>(v, t) { }
    int run(int i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds((int)this->th));
        return 0;
    }
};

template<typename DTYPE>
class FunctionMeasureVectorCountA : FunctionMeasureVectorThreshold<DTYPE>
{
    public:
    FunctionMeasureVectorCountA(const std::vector<DTYPE> &v, DTYPE t) :
        FunctionMeasureVectorThreshold<DTYPE>(v, t) { }
    int run(int x)
    {
        int nb = x % 1; // == 0 but otherwise the compiler detects the
                        // the code does not change depending on x
                        // and it caches the results.
        for(int i = 0; i < this->values.size(); ++i)
            REPEAT_INST10(if (this->values[i] >= this->th) ++nb;)
        return nb;
    }
};


template<typename DTYPE>
class FunctionMeasureVectorCountB : FunctionMeasureVectorThreshold<DTYPE>
{
    public:
    FunctionMeasureVectorCountB(const std::vector<DTYPE> &v, DTYPE t) :
        FunctionMeasureVectorThreshold<DTYPE>(v, t) { }
    int run(int x)
    {
        int nb = x % 1;
        for(auto it = this->values.begin(); it != this->values.end(); ++it)
            REPEAT_INST10(if (*it >= this->th) ++nb;)
        return nb;
    }
};

template<typename DTYPE>
class FunctionMeasureVectorCountC : FunctionMeasureVectorThreshold<DTYPE>
{
    public:
    FunctionMeasureVectorCountC(const std::vector<DTYPE> &v, DTYPE t) :
        FunctionMeasureVectorThreshold<DTYPE>(v, t) { }
    int run(int x)
    {
        int nb = x % 1;
        for(auto it = this->values.begin(); it != this->values.end(); ++it)
            REPEAT_INST10(if (*it >= this->th) nb++;)
        return nb;
    }
};

template<typename DTYPE>
class FunctionMeasureVectorCountD : FunctionMeasureVectorThreshold<DTYPE>
{
    public:
    FunctionMeasureVectorCountD(const std::vector<DTYPE> &v, DTYPE t) :
        FunctionMeasureVectorThreshold<DTYPE>(v, t) { }
    int run(int x)
    {
        int nb = x % 1;
        for(auto it = this->values.begin(); it != this->values.end(); ++it)
            REPEAT_INST10(nb += *it >= this->th ? 1 : 0;)
        return nb;
    }
};

template<typename DTYPE>
class FunctionMeasureVectorCountE : FunctionMeasureVectorThreshold<DTYPE>
{
    public:
    FunctionMeasureVectorCountE(const std::vector<DTYPE> &v, DTYPE t) :
        FunctionMeasureVectorThreshold<DTYPE>(v, t) { }
    int run(int x)
    {
        int nb = x % 1;
        for(auto it = this->values.begin(); it != this->values.end(); ++it)
            REPEAT_INST10(if (*it >= this->th) nb += 1;)
        return nb;
    }
};

template<typename DTYPE>
class FunctionMeasureVectorCountF : FunctionMeasureVectorThreshold<DTYPE>
{
    public:
    FunctionMeasureVectorCountF(const std::vector<DTYPE> &v, DTYPE t) :
        FunctionMeasureVectorThreshold<DTYPE>(v, t) { }
    int run(int x)
    {
        int nb = x % 1;
        for(auto it = this->values.begin(); it != this->values.end(); ++it)
            REPEAT_INST10(nb += (*it - this->th) >= 0 ? 1 : 0;)
        return nb;
    }
};

template<typename DTYPE>
class FunctionMeasureVectorCountG : FunctionMeasureVectorThreshold<DTYPE>
{
    public:
    FunctionMeasureVectorCountG(const std::vector<DTYPE> &v, DTYPE t) :
        FunctionMeasureVectorThreshold<DTYPE>(v, t) { }
    int run(int x)
    {
        int nb = x % 1;
        for(auto it = this->values.begin(); it != this->values.end(); ++it)
            REPEAT_INST10(nb += (*it - this->th) < 0 ? 1 : 0;)
        return (int)std::distance(this->values.begin(), this->values.end()) - nb;
    }
};

template<typename DTYPE>
class FunctionMeasureVectorCountH : FunctionMeasureVectorThreshold<DTYPE>
{
    public:
    FunctionMeasureVectorCountH(const std::vector<DTYPE> &v, DTYPE t) :
        FunctionMeasureVectorThreshold<DTYPE>(v, t) { }
    int run(int x)
    {
        int nb = x % 1;
        for(auto it = this->values.begin(); it != this->values.end(); ++it)
            REPEAT_INST10(nb += *it < this->th ? 1 : 0;)
        return (int)std::distance(this->values.begin(), this->values.end()) - nb;
    }
};

template<typename DTYPE>
class FunctionMeasureVectorCountI : FunctionMeasureVectorThreshold<DTYPE>
{
    public:
    FunctionMeasureVectorCountI(const std::vector<DTYPE> &v, DTYPE t) :
        FunctionMeasureVectorThreshold<DTYPE>(v, t) { }
    int run(int x)
    {
        int nb = x % 1;
        for(auto it = this->values.begin(); it != this->values.end(); ++it)
            REPEAT_INST10(nb += 1 ^ ((unsigned int)(*it - this->th) >> (sizeof(int) * CHAR_BIT - 1));)
        return nb;
    }
};

template<typename DTYPE>
class FunctionMeasureVectorCountJ : FunctionMeasureVectorThreshold<DTYPE>
{
    public:
    FunctionMeasureVectorCountJ(const std::vector<DTYPE> &v, DTYPE t) :
        FunctionMeasureVectorThreshold<DTYPE>(v, t) { }
    int run(int x)
    {
        int nb = x % 1; // == 0 but otherwise the compiler detects the
                        // the code does not change depending on x
                        // and it caches the results.
        for(int i = 0; i < this->values.size(); ++i)
            REPEAT_INST10(nb += this->values[i] >= this->th ? 1 : 0;)
        return nb;
    }
};


/////////////////////////
// dot product
/////////////////////////

#if defined(_MSC_VER)
#define STRING_CONCAT(A, B) A ## B
#define CBENCHMARK_ADDFUNC(suf, C, DTYPE)\
    m.def(STRING_CONCAT("measure_scenario_", #suf), \
            [](const std::vector<DTYPE> &values, DTYPE th, int repeat=100, int number=1000, bool verbose=false) -> ExecutionStat { \
                ExecutionStat res; \
                FunctionMeasureVectorCount##suf<DTYPE> fct(values, th); \
                repeat_execution<FunctionMeasureVectorCount##suf<DTYPE>>(fct, repeat, number, res, verbose); \
                return res; \
            }, \
            C, \
            py::arg("values"), py::arg("th"), \
            py::arg("repeat")=100, py::arg("number")=10, \
            py::arg("verbose")=false);
#else
#define STRING_CONCAT_GCC(A, B) #A #B
#define CBENCHMARK_ADDFUNC(suf, C, DTYPE)\
    m.def(STRING_CONCAT_GCC(measure_scenario_, suf), \
            [](const std::vector<DTYPE> &values, DTYPE th, int repeat=100, int number=1000, bool verbose=false) -> ExecutionStat { \
                ExecutionStat res; \
                FunctionMeasureVectorCount##suf<DTYPE> fct(values, th); \
                repeat_execution<FunctionMeasureVectorCount##suf<DTYPE>>(fct, repeat, number, res, verbose); \
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


#ifdef USE_OPENMP
float _vector_dot_product_pointer_openmp(const float *p1, const float *p2,
                                         int size, int nthreads)
{
    float sum = 0;
    #pragma omp parallel for reduction(+ : sum) num_threads(nthreads)
    for (int i = 0; i < size; ++i) 
        sum += (p1[i] * p2[i]);
    return sum;
}
#else
float _vector_dot_product_pointer_openmp(const float *p1, const float *p2,
                                         int size, int nthreads)
{
    float sum = 0;
    for (int i = 0; i < size; ++i) 
        sum += (p1[i] * p2[i]);
    return sum;
}
#endif

float vector_dot_product_pointer_openmp(const float *p1, const float *p2, size_t size,
                                        int nthreads=-1)
{
#ifdef USE_OPENMP
    if (nthreads <= 0)
        nthreads = ::omp_get_max_threads();
    nthreads = nthreads <= 2 ? 2 : nthreads;
    int isize = (int)(size - (size % nthreads));
    float sum = _vector_dot_product_pointer_openmp(p1, p2, isize, nthreads);
    const float * end1 = p1 + size;
    p1 += isize;
    p2 += isize;
    for(; p1 != end1; ++p1, ++p2)
        sum += *p1 * *p2;
#else
    float sum = _vector_dot_product_pointer_openmp(p1, p2, (int)size, 1);
#endif
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

float vector_dot_product_openmp(py::array_t<float> v1, py::array_t<float> v2, int nthreads=-1)
{
    #ifdef USE_OPENMP
    if (v1.ndim() != v2.ndim())
        throw std::runtime_error("Vector v1 and v2 must have the same dimension.");
    if (v1.ndim() != 1)
        throw std::runtime_error("Vector v1 and v2 must be vectors.");
    return vector_dot_product_pointer_openmp(v1.data(0), v2.data(0), v1.shape(0), nthreads);
    #else
    throw std::runtime_error("OPENMP is not enabled.");
    #endif
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
    size -= i;
    for(; size > 0; ++p1, ++p2, --size)
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
    size -= i;
    for(; size > 0; ++p1, ++p2, --size)
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


#ifndef SKIP_PYTHON
#define CBENCHMARK_MODULE_NAME "cpyquickhelper.numbers.cbenchmark"

// See https://github.com/pybind/pybind11/issues/616.
// Required to use ExecutionStat defined in cbenchmark.
namespace pybind11 { namespace detail {
template <> struct type_caster<ExecutionStat> {
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
} }

PYBIND11_MODULE(cbenchmark_dot, m) {
	m.doc() =
    #if defined(__APPLE__)
    "Measures the execution time of functions implemented in C and about dot product."
    #else
    R"pbdoc(Measures the execution time of functions implemented in C, the measures are
also implemented in C. The functions propose different implementations of the dot product.)pbdoc"
    #endif
    ;
    
    CBENCHMARK_ADDFUNC(A, "Measure C++ implementation. Loop on ``if (values[i] >= th) ++nb;``", float);
    CBENCHMARK_ADDFUNC(B, "Measure C++ implementation. Loop on ``if (*it >= th) ++nb;``", float);
    CBENCHMARK_ADDFUNC(C, "Measure C++ implementation. Loop on ``if (*it >= th) nb++;``", float);
    CBENCHMARK_ADDFUNC(D, "Measure C++ implementation. Loop on ``nb += *it >= th ? 1 : 0;``", float);
    CBENCHMARK_ADDFUNC(E, "Measure C++ implementation. Loop on ``if (*it >= th) nb += 1;``", float);
    CBENCHMARK_ADDFUNC(F, "Measure C++ implementation. Loop on ``nb += (*it - th) >= 0 ? 1 : 0;``", float);
    CBENCHMARK_ADDFUNC(G, "Measure C++ implementation. Loop on ``nb += (*it - th) < 0 ? 1 : 0;``", float);
    CBENCHMARK_ADDFUNC(H, "Measure C++ implementation. Loop on ``nb += *it < th ? 1 : 0;``", float);
    CBENCHMARK_ADDFUNC(I, "Measure C++ implementation. Loop on ``nb += 1 ^ ((unsigned int)(*it) >> (sizeof(int) * CHAR_BIT - 1));``", float);
    CBENCHMARK_ADDFUNC(J, "Measure C++ implementation. Loop on ``nb += values[i] >= th ? 1 : 0;``", float);
    CBENCHMARK_ADDFUNC(Sleep, "Measure C++ implementation. Loop on ``sleep``", float);
        
    m.def("vector_dot_product", &vector_dot_product,
          "Computes a dot product in C++ with vectors of floats.");
    m.def("vector_dot_product_openmp", &vector_dot_product_openmp,
          py::arg("p1"), py::arg("p2"), py::arg("nthreads") = -1,
          "Computes a dot product in C++ with vectors of floats and parallelizes with OPENMP.");
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
}

#endif
