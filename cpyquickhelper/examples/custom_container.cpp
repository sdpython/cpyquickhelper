#include "custom_container.h"
#include <stdlib.h>
#include <string.h>

#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif



CustomContainer::CustomContainer(size_t size, void *p, ContainerType type, bool copy, bool del) {
    _size = size;
    _type = type;
    _del = del;
    if (copy) {
        _data = (void*)malloc(size);
        memcpy(_data, p, size);
    }
    else _data = p;
}


CustomContainer::~CustomContainer() {
    if (_del)
        free(_data);
}

