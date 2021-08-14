#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <vector>
#include <thread>
#include <iterator>
#include <iostream>

#ifndef SKIP_PYTHON
#include <pybind11/iostream.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
//#include <numpy/arrayobject.h>

namespace py = pybind11;
#endif

double edit_distance(const std::vector<int>& seq1, const std::vector<int>& seq2,
                     double cmp_cost,
                     std::vector<double>& dist, std::vector<int32_t>& pred) {
    int64_t n1 = seq1.size() + 1;
    int64_t n2 = seq2.size() + 1;
    if (dist.size() != n1 * n2)
        throw std::runtime_error("dist has an unexpected size.");
    if (pred.size() != n1 * n2)
        throw std::runtime_error("dist has an unexpected size.");
    std::fill(dist.begin(), dist.end(), n1 * n2);
    std::fill(pred.begin(), pred.end(), 0);
    /*
def edit_distance_string(s1, s2, cmp_cost=1.):
    """
    Computes the edit distance between strings *s1* and *s2*.

    :param s1: first string
    :param s2: second string
    :return: dist, list of tuples of aligned characters
    """
    n1 = len(s1) + 1
    n2 = len(s2) + 1
    dist = numpy.full((n1, n2), n1 * n2, dtype=numpy.float64)
    pred = numpy.full(dist.shape, 0, dtype=numpy.int32)

    for i in range(0, n1):
        dist[i, 0] = i
        pred[i, 0] = 1
    for j in range(1, n2):
        dist[0, j] = j
        pred[0, j] = 2
    pred[0, 0] = -1

    for i in range(1, n1):
        for j in range(1, n2):
            c = dist[i, j]

            p = 0
            if dist[i - 1, j] + 1 < c:
                c = dist[i - 1, j] + 1
                p = 1
            if dist[i, j - 1] + 1 < c:
                c = dist[i, j - 1] + 1
                p = 2
            d = 0 if s1[i - 1] == s2[j - 1] else cmp_cost
            if dist[i - 1, j - 1] + d < c:
                c = dist[i - 1, j - 1] + d
                p = 3
            if p == 0:
                raise RuntimeError(
                    "Unexpected value for p=%d at position=%r." % (p, (i, j)))

            dist[i, j] = c
            pred[i, j] = p

    d = dist[len(s1), len(s2)]
    equals = []
    i, j = len(s1), len(s2)
    p = pred[i, j]
    while p != -1:
        if p == 3:
            equals.append((i - 1, j - 1))
            i -= 1
            j -= 1
        elif p == 2:
            j -= 1
        elif p == 1:
            i -= 1
        else:
            raise RuntimeError(
                "Unexpected value for p=%d at position=%r." % (p, (i, j)))
        p = pred[i, j]
    return d, list(reversed(equals))
    */
}
                     


#ifndef SKIP_PYTHON

PYBIND11_MODULE(edit_distance, m) {
	m.doc() = "Edit distance in C++";

    m.def ("edit_distance", &edit_distance,
        #if defined(__APPLE__)
           "Compute the edit distance between two sequences of integers."
        #else
            R"pbdoc(Compute the edit distance between two sequences of integers.
)pbdoc"
        #endif
    );
}

#endif
