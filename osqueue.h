#ifndef __QUEUE_H
 
#define __QUEUE_H
 
 
 
#include <stdio.h>
 
#include <stdlib.h>
 
#include <error.h>
 
 
 
struct queuerecord;
 
typedef struct queuerecord *queue;
 
struct queuerecord{
 
    int capacity;
 
    int front;
 
    int rear;
 
    int size;
 
    int *array;
 
};
 
 
 
int isempty(queue q);
 
int isfull(queue q);
 
queue createqueue(int max);
 
void disposequeue(queue q);
 
void makeempty(queue q);
 
void enqueue(int x, queue q);
 
int front(queue q);
 
void dequeue(queue q);
 
int frontanddequeue(queue q);
 
 
void sortByArriveTime(PCB* pcb)
 
{
 
    for(int i = 0; i < num1; i++)
 
    {
 
        /* 最小的到达时间 */
 
        int min = pcb[i].time_start;
 
        /* 最小到达时间的进程下标 */
 
        int minIndex = i;
 
        for(int j = i + 1; j < num1; j++)
 
        {
 
            if(pcb[j].time_start < min)
 
            {
 
                min = pcb[j].time_start;
 
                minIndex = j;
 
            }
 
        }
 
 
 
        PCB temp = pcb[i];
 
        pcb[i] = pcb[minIndex];
 
        pcb[minIndex] = temp;
 
    }
 
}


 
queue createqueue(int x){
 
    queue *q;
 
    q = (queue*)malloc(sizeof(struct queuerecord));
 
    if(q == NULL){
 
        printf("out of space!!\n");
 
    }
 
 
 
    q->array = malloc(sizeof(int) * x);
 
    if(q->array == NULL){
 
        printf("out of space!\n");
 
    }
 
    q->capacity = x;
 
    makeempty(q);
 
    return q;
 
}
 
 
 
void disposequeue(queue q){
 
    if(q != NULL){
 
        free(q->array);
 
        free(q);
 
    }
 
}
 
 
 
int isempty(queue q){
 
    return q->size == 0;
 
}
 
 
 
void makeempty(queue q){
 
    q->size = 0;
 
    q->front = 1;
 
    q->rear = 0;
 
}
 
 
 
int isfull(queue q){
 
    return q->size == q->capacity;
 
}
 
 
 
static int succ(int value, queue q){
 
    if(++value == q->capacity){
 
        value = 0;
 
    }
 
    return value;
 
}
 
 
 
void enqueue(int x, queue q){
 
    if(isfull(q)){
 
        /* printf("full queue\n"); */
 
        perror("full queue\n");
 
    } else {
 
        q->size++;
 
        q->rear = succ(q->rear, q);
 
        q->array[q->rear] = x;
 
    }
 
}
 
 
 
void dequeue(queue q){
 
    if(isempty(q)){
 
        /* printf("empty queue\n"); */
 
        perror("empty queue\n");
 
    } else{
 
        q->size--;
 
        q->front = succ(q->front, q);
 
    }
 
}
 
 
 
int frontanddequeue(queue q){
 
    int temp = -1;          //元素中不能有-1这个值
 
    if(!isempty(q)){
 
        temp = q->array[q->front];
 
        dequeue(q);
 
    } else{
 
        perror("empty queue\n");
 
        /* printf("empty queue\n"); */
 
    }
 
    return temp;
 
}
 
#endif
 
/* -------------------------按照先来先服务并使用时间片轮转------------------------- */
 
 
 
void Timeslice()
{
 
    sortByArriveTime(pcbdata);
 
 
 
    int currentTime3 = 0;
 
    int finishNumber3 = 0;
 
    int temp, j, i = 0, count;
 
    float k;
 
 
 
    queue q = createqueue(num);
 
 
 
    for(; finishNumber3 != num;)
 
    {
 
        /* 还没到该进程的到达时间 */
 
        if(currentTime3 < pcbdata[i].time_start)
 
        {
 
            currentTime3++;
 
            continue;
 
        }
 
        else
 
        {
 
            /* 让那些在其它进程运行期间到达的进程入队 */
 
            for(int t = 0; t < num; t++)
 
            {
 
                if(currentTime3 >= pcbdata[t].time_start && pcbdata[t].state == 'T')
 
                {
 
                    /* 入队 */
 
                    enqueue(t, q);
 
                    pcbdata[t].state = 'W';
 
                }
 
            }
 
 
 
            /* 出队并且取值 */
 
            i = frontanddequeue(q);
 
            printf("%s\n", pcbdata[i].name);
 
 
 
            for(count = 0; count < time_unit; count++)
 
            {
 
                if(pcbdata[i].time_left != 0)
 
                {
 
                    currentTime3++;
 
                    pcbdata[i].state = 'R';
 
                    pcbdata[i].time_used++;
 
                    pcbdata[i].time_left--;
 
                }
 
 
 
 
 
                /* 在经过一个单位时间后如果进程已经完成了 */
 
                if(pcbdata[i].time_left  == 0)
 
                {
 
                    pcbdata[i].state = 'F';
 
                    finishNumber3++;
 
 
 
                    printf("第%d个进程-- %s, ", i+1, pcbdata[i].name);
 
                    printf("到达时间 -- %d， 服务时间 -- %d\n",
 
                            pcbdata[i].time_start,
 
                            pcbdata[i].time_need);
 
                    printf("本进程正在运行-----------");
 
                    _sleep(1);
 
                    printf("运行完毕\n");
 
                    temp = currentTime3;
 
                    j = temp - pcbdata[i].time_start;
 
                    k = (float)j / pcbdata[i].time_need;
 
                    printf("完成时间 -- %d，周转时间 -- %d, 带权周转时间 -- %.1f\n", temp, j, k);
 
 
 
                    break;
 
                }
 
 
 
            }
 
 
 
            for(int t = 0; t < num; t++)
 
            {
 
                if(currentTime3 >= pcbdata[t].time_start && pcbdata[t].state == 'T')
 
                {
 
                    /* 入队 */
 
                    enqueue(t, q);
 
                    pcbdata[t].state = 'W';
 
                }
 
            }
 
 
 
            if(pcbdata[i].state == 'R')
 
            {
 
                enqueue(i, q);
 
                pcbdata[i].state = 'W';
 
            }
 
        }
 
    }
 
}
 
