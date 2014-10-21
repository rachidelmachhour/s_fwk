#ifdef _WIN32
#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <process.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/locking.h>
#else
#include <pthread.h>
#include <assert.h>
#include <errno.h>
#include <sys/time.h>
#endif

#include "s_thread.h"

typedef VOID (WINAPI* sInitializeConditionVariable) 
			 (PCONDITION_VARIABLE ConditionVariable);

typedef BOOL (WINAPI* sSleepConditionVariableCS)
             (PCONDITION_VARIABLE ConditionVariable,
              PCRITICAL_SECTION CriticalSection,
              DWORD dwMilliseconds);

typedef BOOL (WINAPI* sSleepConditionVariableSRW)
             (PCONDITION_VARIABLE ConditionVariable,
              PSRWLOCK SRWLock,
              DWORD dwMilliseconds,
              ULONG Flags);

typedef VOID (WINAPI* sWakeAllConditionVariable)
             (PCONDITION_VARIABLE ConditionVariable);

typedef VOID (WINAPI* sWakeConditionVariable)
             (PCONDITION_VARIABLE ConditionVariable);


static sInitializeConditionVariable InitializeConditionVariable_fn = NULL;
static sSleepConditionVariableCS    SleepConditionVariableCS_fn = NULL;
static sWakeAllConditionVariable    WakeAllConditionVariable_fn =NULL;
static sWakeConditionVariable       WakeConditionVariable_fn = NULL;

static unsigned char ConditionInit = 0;

static void s_thread_cond_init()
{
    HANDLE lib;

    lib = GetModuleHandle(TEXT("kernel32.dll"));
    if (lib == NULL)
        return;



    InitializeConditionVariable_fn = (sInitializeConditionVariable) GetProcAddress(lib, "InitializeConditionVariable");
    SleepConditionVariableCS_fn    = (sSleepConditionVariableCS) GetProcAddress(lib, "SleepConditionVariableCS");
    WakeAllConditionVariable_fn    = (sWakeAllConditionVariable) GetProcAddress(lib, "WakeAllConditionVariable");
    WakeConditionVariable_fn       = (sWakeConditionVariable) GetProcAddress(lib, "WakeConditionVariable");

   	if(InitializeConditionVariable_fn && SleepConditionVariableCS_fn &&
        WakeAllConditionVariable_fn && WakeConditionVariable_fn){
		ConditionInit = 1;
	}
	else{
		printf("ERROR to initialize variable condition");
	}
	
}


#undef NANOSEC
#define NANOSEC ((uint64_t) 1e9)


struct thread_info {
  void (*cb)(void* arg);
  void* data;
};

#ifdef _WIN32
static UINT __stdcall internal_thread_start(void* arg)
#else
static void* internal_thread_start(void *arg)
#endif
{
  struct thread_info *info = arg;

  info->cb(info->data);
  free(info);

  return 0;
}






int s_thread_create(s_thread_t *tid, void (*callback)(void *arg), void *user_data) {
  struct thread_info* info;
  int err;

  info = malloc(sizeof(struct thread_info));
  if (info == NULL)
    return -1;

  info->cb = callback;
  info->data = user_data;

#ifdef _WIN32
  *tid = (HANDLE) _beginthreadex(NULL, 0, internal_thread_start, info, 0, NULL);
  err = *tid ? 0 : errno;
#else
  err = pthread_create(tid, NULL, internal_thread_start, info);
#endif

  if (err)
    free(info);

  return err ? -1 : 0;
}



unsigned long s_thread_self(void) {
#ifdef _WIN32
  	return (unsigned long) GetCurrentThreadId();
#else
  	return (unsigned long) pthread_self();
#endif
}

int s_thread_join(s_thread_t *tid) {
#ifdef _WIN32
  	if (WaitForSingleObject(*tid, INFINITE))
    	return -1;
  	else {
    	CloseHandle(*tid);
    	*tid = 0;
    	return 0;
  	}
#else
  	return -pthread_join(*tid, NULL);
#endif
}

/*  MUTEX */

int s_mutex_init(s_mutex_t* mutex) {
#ifdef _WIN32
  	InitializeCriticalSection(mutex);
  	return 0;
#else
	return -pthread_mutex_init(mutex, NULL);
#endif
}

void s_mutex_destroy(s_mutex_t* mutex) {
#ifdef _WIN32
  	DeleteCriticalSection(mutex);
#else
	pthread_mutex_destroy(mutex);
#endif
}

void s_mutex_lock(s_mutex_t* mutex) {
#ifdef _WIN32
	EnterCriticalSection(mutex);
#else
  	pthread_mutex_lock(mutex);
#endif
}

int s_mutex_trylock(s_mutex_t* mutex) {
#ifdef _WIN32
  	if (TryEnterCriticalSection(mutex))
		return 0;
  	else
    	return -1;
#else
	if (pthread_mutex_trylock(mutex))
		return 0;
	else
		return -1;
#endif
}

void s_mutex_unlock(s_mutex_t* mutex) {
#ifdef _WIN32
	LeaveCriticalSection(mutex);
#else
	pthread_mutex_unlock(mutex);
#endif	
}

/* semaphore windows + linux support */
int s_sem_init(s_sem_t* sem, unsigned int value) {
#ifdef _WIN32
  	*sem = CreateSemaphore(NULL, value, INT_MAX, NULL);
  	if (*sem == NULL)
    	return -1;
  	else
    	return 0;
#else
  	if (sem_init(sem, 0, value))
    	return -errno;
  	return 0;
#endif
}

void s_sem_destroy(s_sem_t* sem) {
#ifdef _WIN32
	CloseHandle(*sem);
#else
 	sem_destroy(sem);
    	
#endif
}

void s_sem_post(s_sem_t* sem) {
#ifdef _WIN32
	ReleaseSemaphore(*sem, 1, NULL);
#else
  	sem_post(sem);
#endif
}

void s_sem_wait(s_sem_t* sem) {
#ifdef _WIN32
	WaitForSingleObject(*sem, INFINITE);
#else
  	int r;
  	do
    	r = sem_wait(sem);
  	while (r == -1 && errno == EINTR);
#endif
}

int s_sem_trywait(s_sem_t* sem) {
#ifdef _WIN32
  	DWORD r = WaitForSingleObject(*sem, 0);

  	if (r == WAIT_OBJECT_0)
    	return 0;

  	if (r == WAIT_TIMEOUT)
    	return -1;
	/* fix-me check error*/
	return 0;
#else
  	int r;

  	do
    	r = sem_trywait(sem);
  	while (r == -1 && errno == EINTR);

  	if (r) {
    	if (errno == EAGAIN)
      	return -EAGAIN;
    	/* fix-me  error check */
  	}

  return 0;
#endif
}

/*Cond wait */

int s_cond_init(s_cond_t* cond) {
#ifdef _WIN32
	if(ConditionInit == 0) /* initial condtion var func once */
		s_thread_cond_init();
  	InitializeConditionVariable_fn(*cond);
	return 0;
#else
  	pthread_condattr_t attr;
  	int err;
  	err = pthread_condattr_init(&attr);
  	if (err)
    	return -err;
#if !defined(__ANDROID__)
  	err = pthread_condattr_setclock(&attr, CLOCK_MONOTONIC);
  	if (err)
    	goto error2;
#endif
  	err = pthread_cond_init(cond, &attr);
  	if (err)
    	goto error2;
  	err = pthread_condattr_destroy(&attr);
  	if (err)
    	goto error;

  	return 0;

error:
  	pthread_cond_destroy(cond);
error2:
  	pthread_condattr_destroy(&attr);
  	return -err;
#endif

}

void s_cond_destroy(s_cond_t* cond) {
#ifdef _WIN32

#else
  	pthread_cond_destroy(cond);
#endif

}

void s_cond_signal(s_cond_t* cond) {
#ifdef _WIN32
    WakeConditionVariable_fn(cond);
#else
  	pthread_cond_signal(cond);
#endif
}

void s_cond_broadcast(s_cond_t* cond) {
#ifdef _WIN32
    WakeAllConditionVariable_fn(cond);
#else
  	pthread_cond_broadcast(cond);
#endif
    
}

void s_cond_wait(s_cond_t* cond, s_mutex_t* mutex) {
#ifdef _WIN32
  SleepConditionVariableCS_fn(cond, mutex, INFINITE);
#else
	pthread_cond_wait(cond, mutex);
#endif    
}

int s_cond_timedwait(s_cond_t* cond, s_mutex_t* mutex, uint64_t timeout) {
#ifdef _WIN32
    SleepConditionVariableCS_fn(cond, mutex, (unsigned long)timeout);
	return 0;
#else
  	int r;
  	struct timespec ts;

#if defined(__APPLE__) && defined(__MACH__)
  	ts.tv_sec = timeout / NANOSEC;
  	ts.tv_nsec = timeout % NANOSEC;
  	r = pthread_cond_timedwait_relative_np(cond, mutex, &ts);
#else
	clock_gettime(CLOCK_MONOTONIC, &ts);
  	ts.tv_sec += (timeout / NANOSEC) ;
  	ts.tv_nsec += (timeout % NANOSEC);
#if defined(__ANDROID__)
  	/*
  	 * The bionic pthread implementation doesn't support CLOCK_MONOTONIC,
   	* but has this alternative function instead.
   	*/
  	r = pthread_cond_timedwait_monotonic_np(cond, mutex, &ts);
#else
  	r = pthread_cond_timedwait(cond, mutex, &ts);
#endif /* __ANDROID__ */
#endif
  	if (r == 0)
    	return 0;
  	if (r == ETIMEDOUT)
    	return -ETIMEDOUT;
  	/* fix-me check error */
  	return -EINVAL;  /* Satisfy the compiler. */
#endif  /*  _WIN32  */
}

