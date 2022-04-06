#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <vector>
#include <thread>
#include <iterator>
#include <iostream>
#include <sys/types.h>
#include "make_string.hpp"

#ifndef SKIP_PYTHON
#include <pybind11/iostream.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
//#include <numpy/arrayobject.h>

namespace py = pybind11;
#endif

double edit_distance(const py::array_t<int32_t, py::array::c_style | py::array::forcecast>& seq1,
                     const py::array_t<int32_t, py::array::c_style | py::array::forcecast>& seq2,
                     double cmp_cost,
                     py::array_t<double, py::array::c_style | py::array::forcecast>& dist,
                     py::array_t<int32_t, py::array::c_style | py::array::forcecast>& pred) {
    size_t n1 = seq1.size() + 1;
    size_t n2 = seq2.size() + 1;
    size_t size = n1 * n2;
    if ((size_t)dist.size() < size)
        throw std::runtime_error(MakeString(
            "dist has an unexpected size ", dist.size(), " < ", size, "."));
    if ((size_t)pred.size() < size)
        throw std::runtime_error(MakeString(
            "pred has an unexpected size ", pred.size(), " < ", size, "."));

    py::buffer_info b_seq1 = seq1.request();
    py::buffer_info b_seq2 = seq2.request();
    py::buffer_info b_dist = dist.request();
    py::buffer_info b_pred = pred.request();
    const int32_t* p_seq1 = static_cast<int32_t*>(b_seq1.ptr);
    const int32_t* p_seq2 = static_cast<int32_t*>(b_seq2.ptr);
    double* p_dist = static_cast<double*>(b_dist.ptr);
    int32_t* p_pred = static_cast<int32_t*>(b_pred.ptr);
    std::fill(p_dist, p_dist + size, (double)size);
    std::fill(p_pred, p_pred + size, 0);

    for(size_t i = 0; i < n1; ++i) {
        p_dist[i * n2] = (int32_t)i;
        p_pred[i * n2] = 1;
    }
    for(size_t j = 0; j < n2; ++j) {
        p_dist[j] = (int32_t)j;
        p_pred[j] = 2;
    }
    p_pred[0] = -1;

    double c, d;
    size_t ind;
    int32_t p;
    for(size_t i = 1; i < n1; ++i) {
        for(size_t j = 1; j < n2; ++j) {
            ind = i * n2 + j;
            c = p_dist[ind];

            p = 0;
            if (p_dist[ind - n2] + 1 < c) {
                c = p_dist[ind - n2] + 1;
                p = 1;
            }
            if (p_dist[ind - 1] + 1 < c) {
                c = p_dist[ind - 1] + 1;
                p = 2;
            }
            d = p_seq1[i - 1] == p_seq2[j - 1] ? 0 : cmp_cost;
            if (p_dist[ind - 1 - n2] + d < c) {
                c = p_dist[ind - 1 - n2] + d;
                p = 3;
            }
            if (p == 0)
                throw std::runtime_error(
                    MakeString("Unexpected value for p=", p, " at position=(", i, ", ", j, ")."));

            p_dist[ind] = c;
            p_pred[ind] = p;
        }
    }

    d = p_dist[dist.size()-1];
    return d;
}                     


#ifndef SKIP_PYTHON

PYBIND11_MODULE(edit_distance_c, m) {
	m.doc() = "Edit distance in C++";

    m.def ("edit_distance", &edit_distance,
        #if defined(__APPLE__)
           "Compute the edit distance between two sequences of integers."
        #else
            R"pbdoc(Compute the edit distance between two sequences of integers.
    :param seq1: first sequence
    :param seq2: second sequence
    :param cmp_cost: cost of a comparison
    :param dist: an allocated array of `double`,
        size is at least `(len(seq1) + 1) * (len(seq2) + 1)`
    :param pred: an allocated array of `int32_t`,
        size is at least `(len(seq1) + 1) * (len(seq2) + 1)`
    :return: distance

    Arrays dist and pred are filled during the execution of the function.
    """
)pbdoc"
        #endif
    );
}

#endif
