#pragma once


#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#define undef__CRT_SECURE_NO_WARNINGS 1
#endif

#include <chrono>
#include <functional>
#include <iostream>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>
#include "make_string.hpp"


typedef struct CEventProfilerEvent {
    int64_t id_frame;
    int64_t id_arg;
    int64_t event;
        // 0: nothing
        // 1: call
        // 2: return
        // 3: c_call
        // 4: c_return
        // 5: exception
        // 6: c_exception
        // 10: profiler operation begin
        // 11: profiler operation end
        // 100: profiler starts
        // 101: profiler stops
        // 1000: malloc
        // 1001: calloc
        // 1002: realloc
        // 1003: free
        // 1004: realloc_free
    int64_t time;
    int64_t value1;
    int64_t value2;
    std::string to_string() const;
} _CEventProfilerEvent;


class CEventProfiler {
    protected:
        std::vector<CEventProfilerEvent> _buffer;
        std::unordered_map<int64_t,void*> _mem_frame;
        std::unordered_map<int64_t,void*> _mem_arg;
        uint64_t _last_position;
        uint64_t _size;
        std::unordered_map<std::string, int64_t> _event_mapping;
        std::mutex _mtx;
        void* pyinstance;
    public:

        static std::unordered_map<std::string, int64_t> _get_mapping();
        int64_t get_event(const std::string& event) { return _event_mapping[event]; }

        CEventProfiler(int64_t size) : _buffer(size), _last_position(0), _size(size-10) {
            _event_mapping = _get_mapping();
            pyinstance = NULL;
        }
        
        ~CEventProfiler();

        void register_pyinstance(void* f) {
            pyinstance = f;
        }
        
        void clear(bool lock) {
            if (lock)
                _mtx.lock();
            _last_position = 0; 
            if (lock)
                _mtx.unlock();
        }

        const std::unordered_map<int64_t,void*>& mem_frame() const { return _mem_frame; }
        const std::unordered_map<int64_t,void*>& mem_arg() const { return _mem_arg; }
        void delete_pyobj();

        std::string __str__() const;
        std::string __repr__() const;
        inline void* get_pyinstance() const { return pyinstance; }
        inline int64_t size() const { return (int64_t)_last_position; }
        inline int n_columns() const { return sizeof(CEventProfilerEvent) / sizeof(int64_t); }
        inline void lock() { _mtx.lock(); }
        inline void unlock() { _mtx.unlock(); }
        inline const CEventProfilerEvent& operator[](int64_t p) const { return _buffer[p]; }

        const std::vector<CEventProfilerEvent>::const_iterator begin() const { return _buffer.begin(); }
        const std::vector<CEventProfilerEvent>::const_iterator end() const { return _buffer.end(); }
        int64_t dump_and_clear(int64_t* buffer, int64_t size, bool lock);

        inline bool LogEvent(int64_t id_frame, int64_t id_arg, int64_t event,
                             int64_t value1, int64_t value2) {
            if (_last_position >= (uint64_t)_buffer.size())
                throw std::runtime_error(MakeString(
                    "CEventProfiler has a full cache (last_position_=",
                    _last_position, " buffer.size=", _buffer.size(), "."));
            std::chrono::time_point<std::chrono::high_resolution_clock> tp =
                std::chrono::high_resolution_clock::now();
            auto time_a = std::chrono::duration_cast<std::chrono::nanoseconds>(tp.time_since_epoch());
            auto time = std::chrono::duration<int64_t, std::nano>(time_a).count();
            bool res;
            CEventProfilerEvent* ev;

            _mtx.lock();
            ev = &(_buffer[_last_position]);
            ++_last_position;
            res = _last_position < _size;
            _mtx.unlock();

            ev->id_arg = id_arg;
            ev->id_frame = id_frame;
            ev->event = event;
            ev->time = time;
            ev->value1 = value1;
            ev->value2 = value2;
            return res;
        }

        inline bool cLogEvent(void* frame, void* arg,
                              int64_t event, bool &add_frame, bool& add_arg) {
            if (_last_position >= (uint64_t)_buffer.size())
                throw std::runtime_error(MakeString(
                    "CEventProfiler has a full cache (last_position_=",
                    _last_position, " buffer.size=", _buffer.size(), "."));
            std::chrono::time_point<std::chrono::high_resolution_clock> tp =
                std::chrono::high_resolution_clock::now();
            auto time_a = std::chrono::duration_cast<std::chrono::nanoseconds>(tp.time_since_epoch());
            auto time = std::chrono::duration<int64_t, std::nano>(time_a).count();
            bool res;
            CEventProfilerEvent* ev;
            add_frame = false;
            add_arg = false;

            _mtx.lock();
            if (_mem_frame.find((int64_t)frame) == _mem_frame.end()) {
                _mem_frame[(int64_t)frame] = frame;
                add_frame = true;
            }
            if (_mem_arg.find((int64_t)arg) == _mem_arg.end()) {
                _mem_arg[(int64_t)arg] = arg;
                add_arg = true;
            }

            ev = &(_buffer[_last_position]);
            ++_last_position;
            res = _last_position < _size;
            _mtx.unlock();

            ev->id_arg = (int64_t)arg;
            ev->id_frame = (int64_t)frame;
            ev->event = event;
            ev->time = time;
            ev->value1 = 0;
            ev->value2 = 0;

            return res;
        }
};

#if defined(undef_CRT_SECURE_NO_WARNINGS)
#undef _CRT_SECURE_NO_WARNINGS
#endif
