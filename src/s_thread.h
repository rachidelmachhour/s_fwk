#ifndef _S_THREAD_H
#define _S_THREAD_H

#ifdef _WIN32
#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <process.h>
#include <signal.h>
#include <sys/stat.h>
#include <stdio.h>
#else
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
#include <assert.h>
#include <errno.h>
#include <sys/time.h>
#endif

#include "s_type.h"


#ifdef _WIN32 
typedef CONDITION_VARIABLE s_cond_t;
#if 0
 typedef struct s_cond_t {
    void *Ptr;
} s_cond_t;
#endif
typedef HANDLE s_thread_t;
typedef HANDLE s_sem_t;
typedef CRITICAL_SECTION s_mutex_t;
#else
typedef pthread_t s_thread_t;
typedef pthread_mutex_t s_mutex_t;
typedef sem_t s_sem_t;
typedef pthread_cond_t s_cond_t;
#endif


#endif
