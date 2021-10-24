#include "_event_profiler_allocator.h"


static MemoryAllocator static_allocator;


static void EmptyCache() {
    PyGILState_STATE state = PyGILState_Ensure();
    // empty cache
    PyObject* obj = (PyObject*)static_allocator.event_profiler->get_pyinstance();
    if (obj == 0) {
        PyErr_SetString(
            PyExc_TypeError,
            "EventProfiler: no callback method, increase the buffer size or specify one.");
        PyGILState_Release(state);
        return;
    }
    PyObject* res = PyObject_CallMethod(obj, "_empty_cache", NULL);
    Py_DECREF(res);
    PyGILState_Release(state);
}


MemoryAllocator& get_static_allocator() {
    return static_allocator;
}


void* profiled_malloc(void *ctx, size_t size) {
    void* ptr = static_allocator.old_allocator.malloc(ctx, size);
    if (!static_allocator.event_profiler->LogEvent(
            0, 0, 1000, (int64_t)ptr, (int64_t)size))
        EmptyCache();
    return ptr;
}


void* profiled_calloc(void *ctx, size_t nelem, size_t elsize) {
    void* ptr = static_allocator.old_allocator.calloc(ctx, nelem, elsize);
    if (!static_allocator.event_profiler->LogEvent(
            0, 0, 1001, (int64_t)ptr, (int64_t)(elsize * nelem)))
        EmptyCache();
    return ptr;
}


void* profiled_realloc(void *ctx, void* old_ptr, size_t new_size) {
    void* ptr = static_allocator.old_allocator.realloc(ctx, old_ptr, new_size);
    static_allocator.event_profiler->LogEvent(
        0, 0, 1004, (int64_t)old_ptr, (int64_t)0);
    if (!static_allocator.event_profiler->LogEvent(
            0, 0, 1002, (int64_t)ptr, (int64_t)new_size))
        EmptyCache();
    return ptr;
}


void profiled_free(void *ctx, void *ptr) {
    if (!static_allocator.event_profiler->LogEvent(0, 0, 1003, (int64_t)ptr, 0))
        EmptyCache();
    static_allocator.old_allocator.free(ctx, ptr);
}
