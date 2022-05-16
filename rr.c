#include<stdio.h>
#include<time.h>
#include<unistd.h>
#include<sched.h>

int main()
{
    fork();
    struct  sched_param param;
    param.sched_priority =1;
    int scheduler = sched_setscheduler(getpid(),SCHED_RR,&param);
    printf("%d\n",scheduler);
    int count =0;
    while (1)
    {
        count++;
        if(count>10000000)
        {
            count=0;
            time_t t;
            time(&t);
            printf("PID = %d, time = %s", getpid(),ctime(&t));
        }
    }
    return 0;

    
}