"""
@file
@brief Edit distance.
"""
import numpy
from .edit_distance_c import edit_distance  # pylint: disable=E0611


def edit_distance_string(s1, s2, cmp_cost=1.):
    """
    Computes the edit distance between strings *s1* and *s2*.

    :param s1: first string
    :param s2: second string
    :return: dist, list of tuples of aligned characters
    """
    n1 = len(s1) + 1
    n2 = len(s2) + 1
    dist = numpy.empty((n1, n2), dtype=numpy.float64)
    pred = numpy.empty(dist.shape, dtype=numpy.int32)
    seq1 = numpy.array(list(map(ord, s1)), dtype=numpy.int32)
    seq2 = numpy.array(list(map(ord, s2)), dtype=numpy.int32)
    d = edit_distance(seq1, seq2, cmp_cost, dist, pred)

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
            raise RuntimeError(  # pragma: no cover
                "Unexpected value for p=%d at position=%r." % (p, (i, j)))
        p = pred[i, j]
    return d, list(reversed(equals))
