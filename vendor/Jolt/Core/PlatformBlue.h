#pragma once

#define SCE_OK 0
#include <kernel.h>

#define JPH_PLATFORM_BLUE_GET_TICKS sceKernelReadTsc

#define JPH_PLATFORM_BLUE_RWLOCK ScePthreadRwlock
#define JPH_PLATFORM_BLUE_RWLOCK_INIT(x) scePthreadRwlockInit(&x, nullptr, nullptr)
#define JPH_PLATFORM_BLUE_RWLOCK_DESTROY(x) scePthreadRwlockDestroy(&x)
#define JPH_PLATFORM_BLUE_RWLOCK_TRYWLOCK(x) scePthreadRwlockTrywrlock(&x) == SCE_OK
#define JPH_PLATFORM_BLUE_RWLOCK_TRYRLOCK(x) scePthreadRwlockTryrdlock(&x) == SCE_OK
#define JPH_PLATFORM_BLUE_RWLOCK_WLOCK(x) scePthreadRwlockWrlock(&x)
#define JPH_PLATFORM_BLUE_RWLOCK_RLOCK(x) scePthreadRwlockRdlock(&x)
#define JPH_PLATFORM_BLUE_RWLOCK_WUNLOCK(x) scePthreadRwlockUnlock(&x)
#define JPH_PLATFORM_BLUE_RWLOCK_RUNLOCK(x) scePthreadRwlockUnlock(&x)

#define JPH_PLATFORM_BLUE_MUTEX ScePthreadMutex
#define JPH_PLATFORM_BLUE_MUTEX_INIT(x)										\
	ScePthreadMutexattr attr;												\
	scePthreadMutexattrInit(&attr);											\
	scePthreadMutexattrSettype(&attr, SCE_PTHREAD_MUTEX_NORMAL);			\
	scePthreadMutexattrSetprotocol(&attr, SCE_PTHREAD_PRIO_INHERIT);		\
	scePthreadMutexInit(&x, &attr, nullptr);								\
	scePthreadMutexattrDestroy(&attr)
#define JPH_PLATFORM_BLUE_MUTEX_DESTROY(x) scePthreadMutexDestroy(&x)
#define JPH_PLATFORM_BLUE_MUTEX_TRYLOCK(x) scePthreadMutexTrylock(&x) == SCE_OK
#define JPH_PLATFORM_BLUE_MUTEX_LOCK(x) scePthreadMutexLock(&x)
#define JPH_PLATFORM_BLUE_MUTEX_UNLOCK(x) scePthreadMutexUnlock(&x)

#define JPH_PLATFORM_BLUE_SEMAPHORE SceKernelSema
#define JPH_PLATFORM_BLUE_SEMAPHORE_INIT(x) sceKernelCreateSema(&x, nullptr, 0, 0, INT_MAX, nullptr)
#define JPH_PLATFORM_BLUE_SEMAPHORE_DESTROY(x) sceKernelDeleteSema(x)
#define JPH_PLATFORM_BLUE_SEMAPHORE_SIGNAL(x, nr) sceKernelSignalSema(x, nr)
#define JPH_PLATFORM_BLUE_SEMAPHORE_WAIT(x, nr) sceKernelWaitSema(x, nr, nullptr)

#define JPH_BREAKPOINT __debugbreak()
