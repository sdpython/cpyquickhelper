#pragma once


#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#define undef__CRT_SECURE_NO_WARNINGS 1
#endif

#include <string>

enum ContainerType {
    VOID=0,
    BYTE=1,
    FLOAT=2,
    DOUBLE=3,
};


class CustomContainer
{
    public:
        
    CustomContainer(size_t size=0, void *p=NULL,
                    ContainerType type=ContainerType::VOID,
                    bool copy=false, bool del=true);
    ~CustomContainer();
    
    bool empty();
    size_t size();
    void * data();
    ContainerType dtype();
    unsigned char operator [](size_t index);
    
    private:
        
    ContainerType _type;
    void *_data;
    size_t _size;
    bool _del;
};

inline bool CustomContainer::empty() { return _size == 0; }
inline size_t CustomContainer::size() { return _size; }
inline void * CustomContainer::data() { return _data; }
inline ContainerType CustomContainer::dtype() { return _type; }
inline unsigned char CustomContainer::operator [](size_t index) { return *((unsigned char*)_data + index); }


#if defined(undef_CRT_SECURE_NO_WARNINGS)
#undef _CRT_SECURE_NO_WARNINGS
#endif
