// Source: https://github.com/munawarb/Python-Kill-Thread-Extension/blob/master/threader.c
#include "threader.hpp"

#define PTW32_STATIC_LIB

#ifdef _MSC_VER
#define _TIMESPEC_DEFINED
#include <windows.h>

typedef struct {
	HANDLE handle;
	void *(*start_routine)(void*);
	void *arg;
} pthread_t;

int pthread_cancel(pthread_t th)
{
    TerminateThread(th.handle, 0);
    CloseHandle(th.handle);
    return 0;
}

#else
// Source taken from: ftp://sourceware.org/pub/pthreads-win32/dll-latest/
// See http://web.cs.du.edu/~sturtevant/pthread.html
#include <pthread.h>
#endif


int threader_kill_thread(unsigned long upid)
{
    // See https://github.com/python/cpython/blob/master/Python/thread_pthread.h#L233.
    // And function PyThread_get_thread_ident.
    pthread_t *ptc = (pthread_t*)(void*)(&upid);
    pthread_t pu = *ptc;    
    int succeeded = pthread_cancel(pu);
    return succeeded;
}

