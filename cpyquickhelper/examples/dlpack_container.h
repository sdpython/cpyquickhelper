#pragma once


#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#define undef__CRT_SECURE_NO_WARNINGS 1
#endif

#include <string>
#include <exception>
#include "dlpack.h"


typedef void (*deleter_fct)(struct DLManagedTensor * self);

class DLPackContainer {
    public:

        DLPackContainer(int ndim=0,
                        int64_t* shape=NULL,
                        void *p=NULL,
                        DLDataTypeCode dtype=DLDataTypeCode::kDLFloat,
                        int bits=32,
                        int lanes=1,
                        DLDeviceType device=DLDeviceType::kDLCPU,
                        int device_index=0,
                        deleter_fct deleter=NULL,
                        bool copy=true);

        #if false
        DLPackContainer(int ndim=0,
                        ssize_t* shape=NULL,
                        void *p=NULL,
                        DLDataTypeCode dtype=DLDataTypeCode::kDLFloat,
                        int bits=32,
                        int lanes=1,
                        DLDeviceType device=DLDeviceType::kDLCPU,
                        int device_index=0,
                        deleter_fct deleter=NULL,
                        bool copy=true);
        #endif

        ~DLPackContainer();

        bool empty() const;
        DLDataType dtype() const;
        DLDevice device() const;
        int element_size() const;
        int64_t shape_size() const;
        int64_t data_size() const;
        void* data() const;
        void* address(int64_t offset) const;
        void copy(void* src, int64_t size, int64_t offset=0, bool check=false);

    protected:

        DLManagedTensor managed_tensor;
};


inline bool DLPackContainer::empty() const { return managed_tensor.dl_tensor.ndim == 0; }
inline DLDataType DLPackContainer::dtype() const { return managed_tensor.dl_tensor.dtype; }
inline DLDevice DLPackContainer::device() const { return managed_tensor.dl_tensor.device; }
inline void * DLPackContainer::data() const { return managed_tensor.dl_tensor.data; }
inline void * DLPackContainer::address(int64_t offset) const { 
    return (void*)((char*)managed_tensor.dl_tensor.data + offset); 
}


#if defined(undef_CRT_SECURE_NO_WARNINGS)
#undef _CRT_SECURE_NO_WARNINGS
#endif
