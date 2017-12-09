// Source: https://github.com/munawarb/Python-Kill-Thread-Extension/blob/master/threader.c
#include "threader.hpp"

#ifdef _MSC_VER
#define _TIMESPEC_DEFINED
#include "win32/pthread.h"
#else
// Source taken from: ftp://sourceware.org/pub/pthreads-win32/dll-latest/
// See http://web.cs.du.edu/~sturtevant/pthread.html
#include <pthread.h>
#endif


int threader_kill_thread(int pid)
{
    pthread_t *pt =(pthread_t *)pid;
    int succeeded = pthread_cancel(*pt);
    return succeeded;
}
