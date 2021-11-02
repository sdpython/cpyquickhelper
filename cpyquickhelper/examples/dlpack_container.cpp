#include "dlpack_container.h"
#include <stdlib.h>
#include <string.h>
#include "make_string.hpp"

#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif


void dlpack_container_deleter(struct DLManagedTensor * managed_tensor) {
    DLPackContainer * cont = (DLPackContainer*)(managed_tensor);
    delete cont;
}


DLPackContainer::DLPackContainer(int ndim, int64_t* shape,
                                 void *p,
                                 DLDataTypeCode dtype,
                                 int bits,
                                 int lanes,
                                 DLDeviceType device_type,
                                 int device_index,
                                 deleter_fct deleter,
                                 bool copy) {
    managed_tensor.manager_ctx = (void*) this;
    managed_tensor.dl_tensor.device.device_type = device_type;
    managed_tensor.dl_tensor.device.device_id = device_index;
    managed_tensor.dl_tensor.dtype.code = (uint8_t)dtype;
    managed_tensor.dl_tensor.dtype.bits = bits;
    managed_tensor.dl_tensor.dtype.lanes = lanes;
                                     
    managed_tensor.dl_tensor.strides = NULL;
    managed_tensor.dl_tensor.byte_offset = 0;
    managed_tensor.dl_tensor.ndim = ndim;

    if (ndim > 0) {
        managed_tensor.dl_tensor.shape = new int64_t[ndim];
        memcpy(managed_tensor.dl_tensor.shape, shape, ndim * sizeof(int64_t));
    }
    else {
        managed_tensor.dl_tensor.shape = NULL;
    }
    
    if (p == NULL) {
        managed_tensor.dl_tensor.data = NULL;
        managed_tensor.deleter = NULL;
    }
    else if (managed_tensor.deleter == NULL) {
        if (copy) {
            // We copy the data.
            int64_t s = data_size();
            managed_tensor.dl_tensor.data = malloc(s);
            memcpy(managed_tensor.dl_tensor.data, p, s);
            managed_tensor.deleter = &dlpack_container_deleter;
        }
        else {
            // No copy.
            managed_tensor.dl_tensor.data = p;
            managed_tensor.deleter = NULL;
        }
    }
    else {
        // No copy.
        managed_tensor.dl_tensor.data = p;
        managed_tensor.deleter = deleter;
    }
}

#if false

DLPackContainer::DLPackContainer(int ndim, ssize_t* shape,
                                 void *p,
                                 DLDataTypeCode dtype,
                                 int bits,
                                 int lanes,
                                 DLDeviceType device_type,
                                 int device_index,
                                 deleter_fct deleter,
                                 bool copy) {
    managed_tensor.manager_ctx = (void*) this;
    managed_tensor.dl_tensor.device.device_type = device_type;
    managed_tensor.dl_tensor.device.device_id = device_index;
    managed_tensor.dl_tensor.dtype.code = (uint8_t)dtype;
    managed_tensor.dl_tensor.dtype.bits = bits;
    managed_tensor.dl_tensor.dtype.lanes = lanes;
                                     
    managed_tensor.dl_tensor.strides = NULL;
    managed_tensor.dl_tensor.byte_offset = 0;
    managed_tensor.dl_tensor.ndim = ndim;

    if (ndim > 0) {
        managed_tensor.dl_tensor.shape = new int64_t[ndim];
        for(int i = 0; i < ndim; ++i)
            managed_tensor.dl_tensor.shape[i] = (int64_t)shape[i];
    }
    else {
        managed_tensor.dl_tensor.shape = NULL;
    }
    
    if (p == NULL) {
        managed_tensor.dl_tensor.data = NULL;
        managed_tensor.deleter = NULL;
    }
    else if (managed_tensor.deleter == NULL) {
        if (copy) {
            // We copy the data.
            int64_t s = data_size();
            managed_tensor.dl_tensor.data = malloc(s);
            memcpy(managed_tensor.dl_tensor.data, p, s);
            managed_tensor.deleter = &dlpack_container_deleter;
        }
        else {
            // No copy.
            managed_tensor.dl_tensor.data = p;
            managed_tensor.deleter = NULL;
        }
    }
    else {
        // No copy.
        managed_tensor.dl_tensor.data = p;
        managed_tensor.deleter = deleter;
    }
}

 #endif


DLPackContainer::~DLPackContainer() {
    if (managed_tensor.dl_tensor.shape != NULL)
        delete [] managed_tensor.dl_tensor.shape;
    if (managed_tensor.dl_tensor.strides != NULL)
        delete [] managed_tensor.dl_tensor.strides;
}


int64_t DLPackContainer::shape_size() const {
    if (managed_tensor.dl_tensor.ndim == 0)
        return 0;
    int64_t s = 1;
    for(int i = 0; i < managed_tensor.dl_tensor.ndim; ++i)
        s *= managed_tensor.dl_tensor.shape[i];
    return s;
}


int64_t DLPackContainer::data_size() const {
    int64_t s = shape_size();
    s *= (managed_tensor.dl_tensor.dtype.bits *
          managed_tensor.dl_tensor.dtype.lanes + 7) / 8;
    return s;
}


int DLPackContainer::element_size() const {
    return (managed_tensor.dl_tensor.dtype.bits *
            managed_tensor.dl_tensor.dtype.lanes + 7) / 8;
}


void DLPackContainer::copy(void* src, int64_t siz, int64_t offset, bool check) {
    if (check) {
        if (offset < 0)
            throw std::runtime_error(MakeString(
                "offset(", offset, ") cannot be negative."));
        if (siz < 0)
            throw std::runtime_error(MakeString(
                "size(", siz, ") cannot be negative."));
        int64_t s = data_size();
        if (offset + siz > 0)
            throw std::runtime_error(MakeString(
                "offset(", offset, ") + size(", siz, ") > tensor.size(", s, ")."));
    }
    void * off = address(offset);
    memcpy(off, src, siz);
}
