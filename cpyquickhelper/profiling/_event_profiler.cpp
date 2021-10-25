#include "_event_profiler.h"

#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <cstring>


std::string CEventProfilerEvent::to_string() const {
    char buf[50];
    sprintf(buf, "%lld,%lld,%lld,%lld,%lld,%lld",
            id_frame, id_arg, event, time, value1, value2);
    return buf;
}


std::unordered_map<std::string, int64_t> CEventProfiler::_get_mapping() {
    std::unordered_map<std::string, int64_t> ev;
    ev["call"] = 1;
    ev["return"] = 2;
    ev["c_call"] = 3;
    ev["c_return"] = 4;
    ev["exception"] = 5;
    ev["c_exception"] = 6;
    ev["profiler_call"] = 10;
    ev["profiler_return"] = 11;
    ev["profiler_start"] = 100;
    ev["profiler_sort"] = 101;
    ev["malloc"] = 1000;
    ev["calloc"] = 1001;
    ev["realloc"] = 1002;
    ev["free"] = 1003;
    ev["realloc_free"] = 1004;
    return ev;
}


void CEventProfiler::delete_pyobj() {
    _mtx.lock();
    _mem_frame.clear();
    _mem_arg.clear();
    _mtx.unlock();
}


CEventProfiler::~CEventProfiler() {
    if (_mem_frame.size() > 0 || _mem_arg.size() > 0)
        throw std::runtime_error(MakeString(
            "Method delete must be called explicitely. There are ",
            _mem_frame.size(), " remaining python objects to free."));
}


std::string CEventProfiler::__str__() const {
    char buf[100];
    sprintf(buf, "CEventProfiler(%d)", (int)_buffer.size());
    return buf;
}


std::string CEventProfiler::__repr__() const {
    char buf[100];
    sprintf(buf, "CEventProfiler(%d)", (int)_buffer.size());
    return buf;
}


int64_t CEventProfiler::dump_and_clear(int64_t* buffer, int64_t size, bool lock) {
    if (lock)
        _mtx.lock();
    int64_t cpy_size = sizeof(CEventProfilerEvent) * _last_position;
    if (size * (int64_t)sizeof(int64_t) < cpy_size)
        throw std::runtime_error(MakeString(
            "Buffer is not big enough: ", size * (int64_t)sizeof(int64_t),
            " >= ", cpy_size, " (copy buffer size)."));
    memcpy(buffer, _buffer.data(), cpy_size);
    int64_t ret = _last_position;
    _last_position = 0;
    if (lock)
        _mtx.unlock();
    return ret;
}

