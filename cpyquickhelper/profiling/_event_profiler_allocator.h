#pragma once

#include "Python.h"
#include "_event_profiler.h"


typedef struct MemoryAllocator {
    PyMemAllocatorEx old_allocator;
    PyMemAllocatorEx new_allocator;
    CEventProfiler* event_profiler;
    MemoryAllocator() { event_profiler = NULL; }
} _MemoryAllocator ;


MemoryAllocator& get_static_allocator();
void* profiled_malloc(void *ctx, size_t size);
void* profiled_calloc(void *ctx, size_t nelem, size_t elsize);
void* profiled_realloc(void *ctx, void* old_ptr, size_t new_size);
void profiled_free(void *ctx, void *ptr);
