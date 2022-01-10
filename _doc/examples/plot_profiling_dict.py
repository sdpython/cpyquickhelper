# -*- coding: utf-8 -*-
"""

.. _l-example-event-profling-dict:

Profiling of dictionary
=======================

.. index:: event, profiling, dictionary

Implementation Python dictionary is quite efficient.
It is possible to replace it with a C++ implementation?
The following code compares several implementation of
a dictionary ``{ str, int: int }``.

.. contents::
    :local:

Implementation to compare
+++++++++++++++++++++++++
"""

import matplotlib.pyplot as plt
import pandas
from cpyquickhelper.fastdata.fast_dict import (
    FastDictStrInt64, FastDictInt64Int64, FastDictIntInt)
from cpyquickhelper.fastdata.fast_dict_c import (
    UnorderedMapStrInt64_Create, UnorderedMapStrInt64_Insert,
    UnorderedMapStrInt64_Get, UnorderedMapStrInt64_Get_Fast,
    UnorderedMapStrInt64_Insert_Fast,
    UnorderedMapRHStrInt64_Create, UnorderedMapRHStrInt64_Insert_Fast,
    UnorderedMapRHStrInt64_Get_Fast)
from pyquickhelper.pycode.profiling import profile, profile2graph


data = [("*" * (i // 2), i, i) for i in range(0, 1000)]
fcts = []


class UnorderedMapRHStrInt64:

    def __init__(self):
        self.obj = UnorderedMapRHStrInt64_Create()

    def insert_fast(self, name, key, value):
        UnorderedMapRHStrInt64_Insert_Fast(self.obj, name, key, value)

    def get_fast(self, name, key, default_value):
        return UnorderedMapRHStrInt64_Get_Fast(self.obj, name, key, default_value)

####################################
# Python implementation


def insert_py(d):
    for n, k, v in data:
        d[n, k] = v


def insert_py2(d):
    for n, k, v in data:
        if k in d:
            d[k][n] = v
        else:
            d[k] = {n: v}


def fct_py():
    d1 = {}
    insert_py(d1)
    get_py(d1)


fcts.append(fct_py)

#########################################
# Second Python implementation


def g_py(d, n, k, v):
    return d.get((n, k), 0)


def get_py(d):
    for n, k, v in data:
        g_py(d, n, k, v)


def g_py2(d, n, k, v):
    if k in d:
        if n in d[k]:
            return d[k][n]
    return v


def get_py2(d):
    for n, k, v in data:
        g_py2(d, n, k, v)


def fct_py2():
    d11 = {}
    insert_py2(d11)
    get_py2(d11)


fcts.append(fct_py2)


#############################################
# pybind11 + unordered_map

def insert_c11(d):
    for n, k, v in data:
        d.insert(n, k, v)


def get_c11(d):
    for n, k, _ in data:
        d.get(n, k, 0)


def fct_c11():
    d2 = FastDictStrInt64()
    insert_c11(d2)
    get_c11(d2)


fcts.append(fct_c11)

#############################################
# C API + unordered_map + wrapper


class UnorderedMapStrInt64:

    def __init__(self):
        self.obj = UnorderedMapStrInt64_Create()

    def insert(self, name, key, value):
        UnorderedMapStrInt64_Insert(self.obj, name, key, value)

    def insert_fast(self, name, key, value):
        UnorderedMapStrInt64_Insert_Fast(self.obj, name, key, value)

    def get(self, name, key, default_value):
        return UnorderedMapStrInt64_Get(self.obj, name, key, default_value)

    def get_fast(self, name, key, default_value):
        return UnorderedMapStrInt64_Get_Fast(self.obj, name, key, default_value)


def insert_c(d):
    for n, k, v in data:
        d.insert(n, k, v)


def get_c(d):
    for n, k, _ in data:
        d.get(n, k, 0)


def fct_c():
    d3 = UnorderedMapStrInt64()
    insert_c(d3)
    get_c(d3)


fcts.append(fct_c)

#############################################
# C API + unordered_map


def insert_c2(d):
    for n, k, v in data:
        UnorderedMapStrInt64_Insert(d, n, k, v)


def get_c2(d):
    for n, k, _ in data:
        UnorderedMapStrInt64_Get(d, n, k, 0)


def fct_c2():
    d3c = UnorderedMapStrInt64_Create()
    insert_c2(d3c)
    get_c2(d3c)


fcts.append(fct_c2)

#############################################
# C API + unordered_map + FAST CALL


def insert_c_fast(d):
    for n, k, v in data:
        UnorderedMapStrInt64_Insert_Fast(d, n, k, v)


def get_c_fast(d):
    for n, k, _ in data:
        UnorderedMapStrInt64_Get_Fast(d, n, k, 0)


def fct_c_fast():
    d3cf = UnorderedMapStrInt64_Create()
    insert_c_fast(d3cf)
    get_c_fast(d3cf)


fcts.append(fct_c_fast)

########################################
# C API + unordered_map + FAST CALL + a different
# implementation, `martinus/robin-hood-hashing
# <https://github.com/martinus/robin-hood-hashing>`_


def insert_c_fast_rh(d):
    for n, k, v in data:
        UnorderedMapRHStrInt64_Insert_Fast(d, n, k, v)


def get_c_fast_rh(d):
    for n, k, _ in data:
        UnorderedMapRHStrInt64_Get_Fast(d, n, k, 0)


def fct_c_fast_rh():
    d3cfrh = UnorderedMapRHStrInt64_Create()
    insert_c_fast_rh(d3cfrh)
    get_c_fast_rh(d3cfrh)


fcts.append(fct_c_fast_rh)

#########################################
# Profiling
# +++++++++


def fctN(N=4000):
    for _ in range(N):
        for f in fcts:
            f()


def clean_name(text):
    text = text.replace("\\", "/")
    return text.split("/examples/")[-1]


ps = profile(fctN)[0]
root, nodes = profile2graph(ps, clean_text=clean_name)
text = root.to_text()
print(text)

###################################
# Visually.

data = []
for node in root:
    data.append(dict(name=node.func_name, time=node.tall))
df = pandas.DataFrame(data).set_index('name').sort_values('time')

fig, ax = plt.subplots(1, 1, figsize=(20, 4))
df.plot.barh(title="Profiling", ax=ax)
fig.tight_layout()

# plt.show()
