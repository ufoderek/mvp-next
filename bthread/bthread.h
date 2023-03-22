#ifndef _BTHREAD_H_
#define _BTHREAD_H_

#define CORE_NUM 0x04
#define BACK_OFF 0x03

#define LOCKSET_ON 0x01

extern inline void bthread_mutex_lock(/*char*/ unsigned int);
extern inline void bthread_mutex_unlock(/*char*/ unsigned int);
extern inline void bthread_barrier_wait(volatile char*,/*char*/ unsigned int);
extern inline void bthread_barrier_init(volatile char*);
extern inline void bthread_cond_wait(volatile int* CondVar,/*char*/ unsigned int CondLock);
extern inline void bthread_cond_broadcast(volatile int* CondVar);
extern inline void bthread_cond_clear(volatile int* CondVar);

/*
   extern inline void bthread_mutex_lock(unsigned int );
   extern inline void bthread_mutex_unlock(unsigned int );
   extern inline void bthread_barrier_wait(volatile char *, unsigned int );
   extern inline void bthread_barrier_init(volatile char *);
   extern inline void bthread_cond_wait(volatile int *CondVar, unsigned int CondLock);
   extern inline void bthread_cond_broadcast(volatile int *CondVar);
   extern inline void bthread_cond_clear(volatile int *CondVar);
 */

#endif