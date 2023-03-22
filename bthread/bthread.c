#include <bthread.h>
#include <stdio.h>

/* LOCK function to replace pthread_lock                                              *
*      since we use inline assembly here, no need to worry about register protection */

inline void bthread_mutex_lock(unsigned int/*char*/ p)
{
    /* prepare target address of lock pool */
    __asm__ __volatile__
    (
        "spin_in:" "\n"
        "	STMFD    sp!,{r1-r2}""\n"
        "	mov r1, #0x001""\n"
        "	mov r2, #0x000""\n"
        //" mov r2, #0xA00" "\n"
        "	mov r2, r2, LSL #20""\n"
    );
    /* provide lock address accroding to lock variable */
    __asm__ __volatile__
    (
        "	orr r2, r2, %[value]""\n" ::[value] "r"(p)
    );
    /* spin lock implementation with SWP */
    __asm__ __volatile__
    (
        "spin_loop:" "\n"
        "	swpb r1, r1, [r2]""\n"
        "	cmp r1, #0x001""\n"
        "	beq spin_loop""\n"
    );

    /* (lockset) inform hardware which lock held by me now */
    if(LOCKSET_ON)
    {
        __asm__ __volatile__
        (
            "lock_held:" "\n"
            "	str r2, [r1], #0x0""\n"
        );
    }

    __asm__ __volatile__
    (
        "	LDMFD    sp!,{r1-r2}""\n"
    );
}

/* UNLOCK function to replace pthread_unlock                                          *
*      since we use inline assembly here, no need to worry about register protection */

inline void bthread_mutex_unlock(unsigned int/*char*/ p)
{
    /* prepare target address of lock pool */
    __asm__ __volatile__
    (
        "spin_out:" "\n"
        "	STMFD    sp!,{r1-r2}""\n"
        "	mov r1, #0x000""\n"
        "	mov r2, #0x000""\n"
        //" mov r2, #0xA00" "\n"
        "	mov r2, r2, LSL #20""\n"
    );
    /* provide lock address accroding to lock variable */
    __asm__ __volatile__
    (
        "	orr r2, r2, %[value]""\n" ::[value] "r"(p)
    );
    /* realse the lock */
    __asm__ __volatile__
    (
        "	strb r1, [r2, #0x0]""\n"
    );

    /* (lockset) inform hardware which lock realsed by me now */
    if(LOCKSET_ON)
    {
        __asm__ __volatile__
        (
            "lock_realse:" "\n"
            "	str r2, [r1, #0x4]""\n"
        );
    }

    __asm__ __volatile__
    (
        "	LDMFD    sp!,{r1-r2}""\n"
    );
}

/* LOCK function used by bthread_barrier                                              *
*      since we use inline assembly here, no need to worry about register protection *
*      note that no lockset detection here                                           */

inline void bthread_mutex_lock_b(unsigned int/*char*/ p)
{
    /* prepare target address of lock pool */
    __asm__ __volatile__
    (
        "spin_in_b:" "\n"
        "	STMFD    sp!,{r1-r2}""\n"
        "	mov r1, #0x001""\n"
        "	mov r2, #0x000""\n"
        //" mov r2, #0xA00" "\n"
        "	mov r2, r2, LSL #20""\n"
    );
    /* provide lock address accroding to lock variable */
    __asm__ __volatile__
    (
        "	orr r2, r2, %[value]""\n" ::[value] "r"(p)
    );
    /* spin lock implementation with SWP */
    __asm__ __volatile__
    (
        "spin_loop_b:" "\n"
        "	swpb r1, r1, [r2]""\n"
        "	cmp r1, #0x001""\n"
        "	beq spin_loop_b""\n"
    );

    __asm__ __volatile__
    (
        "	LDMFD    sp!,{r1-r2}""\n"
    );
}

/* UNLOCK function used by bthread_barrier                                            *
*      since we use inline assembly here, no need to worry about register protection *
*      note that no lockset detection here                                           */

inline void bthread_mutex_unlock_b(unsigned int/*char*/ p)
{
    /* prepare target address of lock pool */
    __asm__ __volatile__
    (
        "spin_out_b:" "\n"
        "	STMFD    sp!,{r1-r2}""\n"
        "	mov r1, #0x000""\n"
        "	mov r2, #0x000""\n"
        //" mov r2, #0xA00" "\n"
        "	mov r2, r2, LSL #20""\n"
    );
    /* provide lock address accroding to lock variable */
    __asm__ __volatile__
    (
        "	orr r2, r2, %[value]""\n" ::[value] "r"(p)
    );
    /* realse the lock */
    __asm__ __volatile__
    (
        "	strb r1, [r2, #0x0]""\n"
    );

    __asm__ __volatile__
    (
        "	LDMFD    sp!,{r1-r2}""\n"
    );
}

/* BARRIER_INIT function to replace pthread_barrier_init           *
*      note that the address of count is defined in splash2_defs/ *
*      we assume all thread run into the same barrier hare        */

inline void bthread_barrier_init(volatile char* b_cnt)
{
    *b_cnt = CORE_NUM;
}

/* BARRIER_WAIT function to replace pthread_barrier_wait                      *
*      we reset the barrier count after someone has reached the next barrier */

inline void bthread_barrier_wait(volatile char* b_cnt, unsigned int/*char*/ b_lock)
{
    int back = BACK_OFF;

    /* (lockset) inform hardware that I have reached */
    if(LOCKSET_ON)
    {
        __asm__ __volatile__
        (
            "barrier_reached:" "\n"
            "	STMFD    sp!,{r1-r2}""\n"
            "	mov r1, #0x000""\n"
            "	mov r2, #0x000""\n"
            //" mov r2, #0xA00" "\n"
            "	mov r2, r2, LSL #20""\n"
        );
        __asm__ __volatile__
        (
            "	orr r2, r2, %[value]""\n" ::[value] "r"(b_lock)
        );
        __asm__ __volatile__
        (
            "	str r2, [r1, #0x8]""\n"
        );
        __asm__ __volatile__
        (
            "	LDMFD    sp!,{r1-r2}""\n"
        );
    }

    bthread_mutex_lock_b(b_lock);

    /* Am I the first ? */
    if((*b_cnt) == 0x00)
    {
        /* Yes, so I need to reset the count before being seen by someone else */
        if((*b_cnt) != (CORE_NUM - 1))
        {
            *b_cnt = CORE_NUM - 1;
        }
    }

    else
    {
        /* No, so I simply decrease the count by one */
        *b_cnt = *b_cnt - 1;
    }

    bthread_mutex_unlock_b(b_lock);

    while(back--)
    {
        ;
    }

    back = BACK_OFF;

    bthread_mutex_lock_b(b_lock);

    /* Dose everyone reach the barrier ? */
    while(*b_cnt != 0)
    {
        /* No, so I check it out later */
        bthread_mutex_unlock_b(b_lock);


        while(back--)
        {
            ;
        }

        back = BACK_OFF;

        bthread_mutex_lock_b(b_lock);
    }

    /* Yes, so I move forward */

    bthread_mutex_unlock_b(b_lock);

    /* (lockset) inform hardware that I am moving forward now */
    if(LOCKSET_ON)
    {
        __asm__ __volatile__
        (
            "barrier_crossed:" "\n"
            "	STMFD    sp!,{r1-r2}""\n"
            "	mov r1, #0x000""\n"
            "	mov r2, #0x000""\n"
            //" mov r2, #0xA00" "\n"
            "	mov r2, r2, LSL #20""\n"
        );
        __asm__ __volatile__
        (
            "	orr r2, r2, %[value]""\n" ::[value] "r"(b_lock)
        );
        __asm__ __volatile__
        (
            "	str r2, [r1, #0xC]""\n"
        );
        __asm__ __volatile__
        (
            "	LDMFD    sp!,{r1-r2}""\n"
        );
    }
}

/* CONDITION_WAIT function to replace pthread_cond_wait                         *
*      note that you need to perform lock operation before using this function *
*      and you hold the lock automatically after retuning from this function   *
*      (same as Pthread library)                                               */

inline void bthread_cond_wait(volatile int* CondVar, unsigned int/*char*/ CondLock)
{
    int back = BACK_OFF;

    //bthread_mutex_lock(CondLock);
    while(*CondVar == 0)
    {
        bthread_mutex_unlock(CondLock);

        while(back--)
        {
            ;
        }

        back = BACK_OFF;

        bthread_mutex_lock(CondLock);
    }

    //bthread_mutex_unlock(CondLock);
}

/* CONDITION_BROADCAST function to replace pthread_cond_broadcast               *
*      note that you need to perform lock operation before using this function *
*      and this fuction will NOT realse the lock automatically                 *
*      (same as Pthread library)                                               */

inline void bthread_cond_broadcast(volatile int* CondVar)
{
    //bthread_mutex_lock(CondLock);

    *CondVar = 1;

    //bthread_mutex_unlock(CondLock);
}

/* CONDITION_CLEAR function to replace pthread_cond_clear                       *
*      note that you need to perform lock operation before using this function *
*      and this fuction will NOT realse the lock automatically                 *
*      (same as Pthread library)                                               */

inline void bthread_cond_clear(volatile int* CondVar)
{
    //bthread_mutex_lock(CondLock);

    *CondVar = 0;

    //bthread_mutex_unlock(CondLock);
}