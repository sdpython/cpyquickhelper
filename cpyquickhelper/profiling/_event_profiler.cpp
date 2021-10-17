#include "_event_profiler.h"

#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>


std::string CEventProfilerEvent::to_string() const {
    char buf[50];
    sprintf(buf, "%ld,%ld,%ld", id_frame, time, event);
    return buf;
}


std::string CEventProfiler::__str__() const {
    char buf[100];
    sprintf(buf, "CEventProfiler(%d)", _buffer.size());
    return buf;
}


std::string CEventProfiler::__repr__() const {
    char buf[100];
    sprintf(buf, "CEventProfiler(%d)", _buffer.size());
    return buf;
}


int64_t CEventProfiler::dump(int64_t* buffer, int64_t size, bool lock) {
    if (lock)
        _mtx.lock();
    int64_t cpy_size = sizeof(CEventProfilerEvent) * _last_position;
    if (size * sizeof(int64_t) < cpy_size)
        throw std::runtime_error("Buffer is not big enough.");
    memcpy(buffer, _buffer.data(), cpy_size);
    if (lock)
        _mtx.unlock();
    return cpy_size;
}
