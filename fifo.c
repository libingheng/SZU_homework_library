#include<sched.h>
#include<unistd.h>
#include<stdio.h>

int main()
{
    struct sched_param param;
    param.sched_priority=2;
    int scheduler = sched_setscheduler(getpid(), SCHED_FIFO,&param);

    long long count = 100000000000;
    while(count--);
    return 0;
    
}