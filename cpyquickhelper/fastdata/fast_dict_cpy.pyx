"""
@file
@brief Implements fast dictionaries with :epkg:`cython` in a specific case.
"""
from cython.operator cimport dereference
from libcpp.pair cimport pair
from libcpp.map cimport map as cmap


ctypedef cmap[pair[int, int], int] cpp_cmap_type


cdef class cmap_int_int:

    cdef cpp_cmap_type container

    def __cinit__(self):
        self.container = cpp_cmap_type()

    def get(self, int name, int key, int default_value):
        it = self.container.find(pair[int, int](key, name))
        if it == self.container.end():
            return default_value
        return dereference(it).second

    def insert(self, int name, int key, int value):
        self.container.insert(pair[pair[int, int], int](pair[int, int](key, name), value))

    def __len__(self):
        return self.container.size()