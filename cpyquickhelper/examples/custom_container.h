#pragma once


#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#define undef__CRT_SECURE_NO_WARNINGS 1
#endif

#include <string>
#include <exception>


typedef enum {
    CT_VOID=0,
    CT_BYTE=1,
    CT_FLOAT=2,
    CT_DOUBLE=3,
    CT_INT64=4,
} ContainerType;


class CustomContainer {
    public:
        
        CustomContainer(size_t size=0, void *p=NULL,
                        ContainerType type=ContainerType::CT_VOID,
                        bool copy=false, bool del=true);
        ~CustomContainer();
        
        bool empty() const;
        size_t size() const;
        void* data() const;
        ContainerType dtype() const;
        unsigned char operator [](size_t index) const;
    
    private:
        
        ContainerType _type;
        void *_data;
        size_t _size;
        bool _del;
};


inline bool CustomContainer::empty() const { return _size == 0; }
inline size_t CustomContainer::size() const { return _size; }
inline void * CustomContainer::data() const { return _data; }
inline ContainerType CustomContainer::dtype() const { return _type; }
inline unsigned char CustomContainer::operator [](size_t index) const { return *((unsigned char*)_data + index); }


#if defined(undef_CRT_SECURE_NO_WARNINGS)
#undef _CRT_SECURE_NO_WARNINGS
#endif
