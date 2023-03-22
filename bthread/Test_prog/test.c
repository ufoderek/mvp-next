#include <stdio.h>

#define l_cpuid 0x00100004

int main()
{

    int i;
    /* get core id*/
    //bthread_mutex_lock(l_cpuid);

    for(i = 0; i < 1000; i++)
    {
        printf("i = %d\n", i);
    }

    //bthread_mutex_unlock(l_cpuid);
    while(1)
    {
        ;
    }

    return 0;
}
