#pragma once


#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#define undef__CRT_SECURE_NO_WARNINGS 1
#endif

#include <string>
#include <vector>
#include <mutex>
#include <chrono>
#include <iostream>
#include <map>


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
        std::map<int64_t,void*> _mem_frame;
        std::map<int64_t,void*> _mem_arg;
        uint64_t _last_position;
        uint64_t _size;
        std::map<std::string, int64_t> _event_mapping;
        std::mutex _mtx;
    public:
        CEventProfiler(int64_t size) : _buffer(size), _last_position(0), _size(size-2) {
            _event_mapping = _get_mapping();
        }

        void clear(bool lock) {
            if (lock)
                _mtx.lock();
            _last_position = 0; 
            if (lock)
                _mtx.unlock();
        }

        const std::map<int64_t,void*>& mem_frame() const { return _mem_frame; }
        const std::map<int64_t,void*>& mem_arg() const { return _mem_arg; }
        void delete_pyobj();
        std::map<std::string, int64_t> _get_mapping() const;
        int64_t get_event(const std::string& event) { return _event_mapping[event]; }
        std::string __str__() const;
        std::string __repr__() const;
        int64_t size() const { return (int64_t)_last_position; }
        int n_columns() const { return sizeof(CEventProfilerEvent) / sizeof(int64_t); }
        void lock() { _mtx.lock(); }
        void unlock() { _mtx.unlock(); }
        const CEventProfilerEvent& operator[](int64_t p) const { return _buffer[p]; }
        const std::vector<CEventProfilerEvent>::const_iterator begin() const { return _buffer.begin(); }
        const std::vector<CEventProfilerEvent>::const_iterator end() const { return _buffer.end(); }
        int64_t dump(int64_t* buffer, int64_t size, bool lock);

        bool LogEvent(int64_t id_frame, int64_t id_arg, int64_t event, int64_t value1, int64_t value2) {
            if (_last_position >= (uint64_t)_buffer.size())
                throw std::runtime_error("CEventProfiler has a full cache.");
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

        bool cLogEvent(int64_t id_frame, void* frame,
                       int64_t id_arg, void* arg,
                       int64_t event, bool &add_frame, bool& add_arg) {
            if (_last_position >= (uint64_t)_buffer.size())
                throw std::runtime_error("CEventProfiler has a full cache.");
            std::chrono::time_point<std::chrono::high_resolution_clock> tp =
                std::chrono::high_resolution_clock::now();
            auto time_a = std::chrono::duration_cast<std::chrono::nanoseconds>(tp.time_since_epoch());
            auto time = std::chrono::duration<int64_t, std::nano>(time_a).count();
            bool res;
            CEventProfilerEvent* ev;
            add_frame = false;
            add_arg = false;

            _mtx.lock();
            if (_mem_frame.find(id_frame) == _mem_frame.end()) {
                _mem_frame[id_frame] = frame;
                add_frame = true;
            }
            if (_mem_arg.find(id_arg) == _mem_arg.end()) {
                _mem_arg[id_arg] = arg;
                add_arg = true;
            }

            ev = &(_buffer[_last_position]);
            ++_last_position;
            res = _last_position < _size;
            _mtx.unlock();

            ev->id_arg = id_arg;
            ev->id_frame = id_frame;
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
