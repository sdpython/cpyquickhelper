#include "_event_profiler_allocator.h"


static MemoryAllocator static_allocator;


MemoryAllocator& get_static_allocator() {
    return static_allocator;
}


void* profiled_malloc(void *ctx, size_t size) {
    void* ptr = static_allocator.old_allocator.malloc(ctx, size);
    static_allocator.event_profiler->LogEvent(
        0, 0, 1000, (int64_t)ptr, (int64_t)size);
    return ptr;
}


void* profiled_calloc(void *ctx, size_t nelem, size_t elsize) {
    void* ptr = static_allocator.old_allocator.calloc(ctx, nelem, elsize);
    static_allocator.event_profiler->LogEvent(
        0, 0, 1001, (int64_t)ptr, (int64_t)(elsize * nelem));
    return ptr;
}


void* profiled_realloc(void *ctx, void* old_ptr, size_t new_size) {
    void* ptr = static_allocator.old_allocator.realloc(ctx, old_ptr, new_size);
    static_allocator.event_profiler->LogEvent(
        0, 0, 1004, (int64_t)old_ptr, (int64_t)0);
    static_allocator.event_profiler->LogEvent(
        0, 0, 1002, (int64_t)ptr, (int64_t)new_size);
    return ptr;
}


void profiled_free(void *ctx, void *ptr) {
    static_allocator.event_profiler->LogEvent(0, 0, 1003, (int64_t)ptr, 0);
    static_allocator.old_allocator.free(ctx, ptr);
}
