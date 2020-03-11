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


template <typename NTYPE>
void gemm(bool transA, bool transB,
          size_t M, size_t N, size_t K, NTYPE alpha,
          NTYPE* A, NTYPE* B, NTYPE beta,
          NTYPE* C) {
    if (!transA && !transB) {
        // a A B + b C, dimension = M * N
        NTYPE* begin;
        register NTYPE val;
        size_t i, j, k;
        NTYPE *pA, *pB;
        for(i = 0, begin = C; i < M; ++i) {
            for(j = 0; j < N; ++j, ++begin) {
                val = *begin * beta;
                pA = A + i * K;
                pB = B + j;
                for(k = K; k > 0; --k, ++pA, pB += N)
                    val += *pA * *pB * alpha;
                *begin = val;
            }
        }
        return;
    }
    throw std::runtime_error("Not implemented for transposed matrices.");
}    


template <typename NTYPE>
void pygemm(bool transA, bool transB, NTYPE alpha,
            py::array_t<NTYPE> A, py::array_t<NTYPE> B,
            NTYPE beta, py::array_t<NTYPE> C) {
    if (A.shape(1) != B.shape(0))
        throw std::runtime_error("Dimension mismatch between A and B.");
    if (A.shape(0) != C.shape(0))
        throw std::runtime_error("Dimension mismatch between A and C.");
    if (B.shape(1) != C.shape(1))
        throw std::runtime_error("Dimension mismatch between B and C.");
    gemm<NTYPE>(transA, transB, A.shape(0), B.shape(1), A.shape(1), alpha,
                (NTYPE*)A.data(0), (NTYPE*)B.data(0), beta, (NTYPE*)C.data(0));
}


#ifndef SKIP_PYTHON


PYBIND11_MODULE(slowcode, m) {
	m.doc() =
    #if defined(__APPLE__)
    "Experiments around matrices."
    #else
    R"pbdoc(Experiments around matrices.)pbdoc"
    #endif
    ;
    
    m.def("sgemm", &pygemm<float>,
          R"pbdoc(C++ implementation of gemm function for single floats. 
Computes one of the following expressions
``C = a A B + b C``, ``C = a A' B + b C``,
``C = a A B' + b C``, ``C = a A' B' + b C``.
The function assumes C is allocated.)pbdoc");
    
    m.def("dgemm", &pygemm<double>,
          R"pbdoc(C++ implementation of gemm function for double floats. 
Computes one of the following expressions
``C = a A B + b C``, ``C = a A' B + b C``,
``C = a A B' + b C``, ``C = a A' B' + b C``.
The function assumes C is allocated.)pbdoc");
}

#endif
