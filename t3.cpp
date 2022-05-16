#include "stdio.h"
#include<stdlib.h>
// #include<conio.h>
#include<time.h>
#include<string.h>
#include <iostream>
#include"osqueue.h"
#define getpch(type) (type*)malloc(sizeof(type))
typedef struct pcb PCB;
using namespace std;
 
//定义进程控制块的PDB
struct pcb{
 
	int id; 	//标识符
 
	char name[10];	//名称
 
	int time_start;	//到达时间
 
	int time_need;	//服务时间
	
	int time_left;//剩余运行时间
 
	int time_used;	//已使用的CPU时间
 
	char state;	//进程状态

	int startTime;

	int leftTime;
};
 
//系统函数

/* 按到达时间排序 */
 
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


 
//停顿几秒
void _sleep(int n){
	clock_t goal;
	goal = (clock_t)n * CLOCKS_PER_SEC + clock();
	while(goal>clock());
}
 
//按任意键继续
char _keygo(){
	char c;
	printf("按任意键继续.......\n");
	c = getchar();
	return c;
}
 
///用户函数///
 
//数据设置区域
 
 /* 当前时间 */
 
int currentTime = 0;
 
/* 已经完成的进程个数 */
 
int finishNumber = 0;
 
int time_unit = 2;//时间片长度
 
const int maxnum = 10;//最大进程数量
 
int num = 5;//实际进程数量
PCB pcbdata[maxnum] = {
	{1000,"A",0,4,4,0,'R'},
	{1001,"B",2,5,3,0,'R'},
	{1002,"C",5,3,5,0,'R'},
	{1003,"D",4,2,2,0,'R'},
	{1004,"E",1,1,4,0,'R'},
 
};
 
//就绪队列，存放进程的pcb
int ready[maxnum];
//记录排序使用哪个数值作为排序对象
int order[maxnum];
 
//手工输入数据
void input(){
	int i;
	printf("进程总数为：");
	scanf("%d",&num);
	for(i = 0; i< num; i++){
		//系统自动分配标识符
		pcbdata[i].id = 1000 + i;
		printf("输入第%d个进程名：",i+1);
		scanf("%s",&pcbdata[i].name);
		printf("输入第%d个进程达到时间：",i+1);
		scanf("%s",&pcbdata[i].time_start);
		printf("输入第%d个进程服务时间：",i+1);
		scanf("%s",&pcbdata[i].time_need);
	
		pcbdata[i].time_left = pcbdata[i].time_need;	//剩余运行时间，初始值和服务时间相同
		printf("\n");
		
		pcbdata[i].time_used = 0;//已使用CPU时间初始值为0
	
		pcbdata[i].state = 'R';	//进程初始状态为就绪R
	}
}
 
 
///调度函数/
 
//先来先服务算法
void FCFS(){
	int i,j,temp;
	double k;
	for(i = 0;i < num; i++){
		order[i] = pcbdata[i].time_start;
		ready[i] = i;
	}
	//冒泡排序
	for(i = 0; i< num; i++){//按照到达时间大小排序
		for(j = i+1; j<num; j++){
			if(order[i] > order[i+1]){
				temp = order[i];
				order[i] = order[j];
				order[j] = temp;
				temp = ready[i];
				ready[i] = ready[j];
				ready[j] = temp;
			} 
		}
	
	}
	printf("--先来先服务算法调度：非抢占，无时间片---\n");
	printf("\n");
	temp = pcbdata[ready[0]].time_start;
	for(i = 0; i < num; i++){
		printf("第%d个进程--%s,",i+1,pcbdata[ready[i]].name);
		printf("到达时间--%d,服务时间--%d\n",
			pcbdata[ready[i]].time_start,
			pcbdata[ready[i]].time_need);
		printf("本进程正在运行...........\n");
		_sleep(1);
		printf("运行完毕\n");
		temp += pcbdata[ready[i]].time_need;
		j = temp-pcbdata[ready[i]].time_start;
		k = (float)j/pcbdata[ready[i]].time_need;
		printf("完成时间--%d,周转时间--%d,带权周转时间--%.1f\n",
			temp,j,k);
		printf("\n");
	}
	printf("--------所有进程调度完毕------\n");
}
 
//短作业优先算法
void SJF(){
	int i,j,temp,min,index,temp1;
	double k;
	for(i = 0;i < num; i++){
		order[i] = pcbdata[i].time_start;
		ready[i] = i;
	}
	//冒泡排序
	for(i = 0; i< num; i++){//按照到达时间大小排序
		for(j = i+1; j<num; j++){
			if(order[i] > order[i+1]){
				temp = order[i];
				order[i] = order[j];
				order[j] = temp;
				temp = ready[i];
				ready[i] = ready[j];
				ready[j] = temp;
			} 
		}
	
	}
	temp = pcbdata[ready[0]].time_start;
	for(i=0;i<num-1;i++)
	{
	//	cout<<"ready[i]="<<ready[i]<<endl;
		temp =temp+ pcbdata[ready[i]].time_need;//完成时间
		min=999;
		for(j=i+1;j<num-1;j++)
		{
	//		cout<<"temp="<<temp<<endl;
			
			if(pcbdata[ready[j]].time_start < temp)//到达时间<完成时间
			{
	//			cout<<pcbdata[ready[j]].time_start<<" "<<temp<<endl;
				 if(pcbdata[ready[j]].time_need<min)//服务时间最小
				 {
					 index=j;//记录下标
					 min=pcbdata[ready[j]].time_need;
				 }
			}
		}
		//把服务时间最小的放到前面优先调度
		temp1 = ready[i+1];
		ready[i+1] = ready[index];
		ready[index] = temp1;
 
	}
 
	printf("--短作业优先算法调度：非抢占，无时间片---\n");
	printf("\n");
	temp = pcbdata[ready[0]].time_start;
	for(i = 0; i < num; i++){
		printf("第%d个进程--%s,",i+1,pcbdata[ready[i]].name);
		printf("到达时间--%d,服务时间--%d\n",
			pcbdata[ready[i]].time_start,
			pcbdata[ready[i]].time_need);
		printf("本进程正在运行...........\n");
		_sleep(1);
		printf("运行完毕\n");
		temp += pcbdata[ready[i]].time_need;
		j = temp-pcbdata[ready[i]].time_start;
		k = (float)j/pcbdata[ready[i]].time_need;
		printf("完成时间--%d,周转时间--%d,带权周转时间--%.1f\n",
			temp,j,k);
		printf("\n");
	}
	printf("--------所有进程调度完毕------\n");
 
}
 
//高响应比算法
void HRF(){
	    int currentTime2 = 0;
 
    int finishNumber2 = 0;
 
    float priority[4];
    sortByArriveTime(pcbdata);
 
    int j = 0;
 
    double k;
 
    int temp = pcbdata[0].time_start;
    for(; finishNumber2 != num; currentTime2++)
 
    {
 
        float maxPriority = 0.0;
 
        int indexPriority = 0;
 
        if(currentTime2 < pcbdata[0].time_start)
 
        {

            continue;
        }
 
        else
 
        {
 
            /* 找出优先级最大的进程下标indexPriority */
 
            for(int i = 0; i < num; i++)
 
            {
 
                /* 在没完成的进程中进行优先级计算 */
 
                if(pcbdata[i].state != 'F')
 
                {
 
                    int waitTime = currentTime2 - pcbdata[i].time_start;
 
                    priority[i] = (waitTime + pcbdata[i].time_need) * 1.0 / pcbdata[i].time_need;
 
                    if(priority[i] > maxPriority)
 
                    {
 
                        maxPriority = priority[i];
 
                        indexPriority = i;
 
                    }
 
                }
 
            }
 
            /* 记录该进程的开始时间 */
 
            pcbdata[indexPriority].startTime = currentTime2;
 
            pcbdata[indexPriority].state = 'R';
 
            /* 计算该进程的结束时间 */
 
            int endTime = pcbdata[indexPriority].startTime + pcbdata[indexPriority].time_need;
 
            while(1)
 
            {
 
                if(currentTime2 == endTime)
 
                {
 
                    pcbdata[indexPriority].state = 'F';
 
                    finishNumber2++;
 
 
 
                    /* 与前面的基本一样 */
 
                    printf("第%d个进程-- %s, ", indexPriority+1, pcbdata[indexPriority].name);
 
                    printf("到达时间 -- %d， 服务时间 -- %d\n",
 
                            pcbdata[indexPriority].time_start,
 
                            pcbdata[indexPriority].time_need);
 
                    printf("本进程正在运行-----------");
 
                    _sleep(1);
 
                    printf("运行完毕\n");
 
                    /* temp += pcbdata[indexPriority].time_need; */
 
                    temp = currentTime2;
 
                    j = temp - pcbdata[indexPriority].time_start;
 
                    k = (double)j / pcbdata[indexPriority].time_need;
 
                    printf("完成时间 -- %d，周转时间 -- %d, 带权周转时间 -- %.1f\n", temp, j, k);
 
 
 
                    currentTime2--;
 
                    break;
 
				}
                else
                {
                    currentTime2++;
                }
            } 
        }
 
    }
 
 
 
    currentTime2 = 0;
 
    finishNumber2 = 0;
 
}
 
//按照先来先服务并使用时间片轮转
void Timeslice();
 
//多级反馈调度队列，抢占式调度
    int currentTime4 = 0;    
    int finishNumber4 = 0;
    int time_unit1 = 4;
    int time_unit2 = 8;
    int time_unit3 = 16;

static int selectProcess(queue q1, queue q2, queue q3)
{
 
    /* 优先从高优先级的队列取出进程 */
 
    if(!isempty(q1))
 
    {
 
        return frontanddequeue(q1);
 
    }
 
    else if(!isempty(q2))
 
    {
 
        return frontanddequeue(q2);
 
    }
 
    else if(!isempty(q3))
 
    {
 
        return frontanddequeue(q3);
 
}
 
return -1; //无意义，只是为了消除警告
 
}

void MRLA(){
   queue q1 = createqueue(num);
 
    queue q2 = createqueue(num);
 
    queue q3 = createqueue(num);
 
    int temp, j, count;
 
    double k;
 
    sortByArriveTime(pcbdata);
 
    int i = 0;
 
 
 
    for(; finishNumber4 != num; )
 
    {
 
        if(currentTime4 < pcbdata[i].time_start)
 
        {
 
            currentTime4++;
 
            continue;
 
        }
 
        else
 
        {
 
            /* 让那些在其它进程运行期间到达的进程入队 */
 
            for(int t = 0; t < num; t++)
 
            {
 
                if(currentTime4 >= pcbdata[t].time_start && pcbdata[t].state == 'T')
 
                {
 
                    /* 入队 */
 
                    enqueue(t, q1);
 
                    pcbdata[t].state = 'W';
 
                }
 
            }
 
 
 
            int q1Flag = 0;
 
            int q2Flag = 0;
 
            int q3Flag = 0;
 
            int tmp1 = q1->size;
 
            int tmp2 = q2->size;
 
            int tmp3 = q3->size;
 
            i = selectProcess(q1, q2, q3);
 
            printf("%s\n", pcbdata[i].name);
 
            /* 用来判断是从哪个队列取的进程 */
 
            if(tmp1 > q1->size)
 
                q1Flag = 1;
 
            else if(tmp2 > q2->size)
 
                q2Flag = 1;
 
            else if(tmp3 > q3->size)
 
                q3Flag = 1;
 
 
 
            if(q1Flag)
 
            {
 
                /* 执行队列一的时间片长度 */
 
                for(count = 0; count < time_unit1; count++)
 
                {
 
                    if(pcbdata[i].time_left != 0)
 
                    {
 
                        currentTime4++;
 
                        pcbdata[i].state = 'R';
 
                        pcbdata[i].time_used++;
 
                        pcbdata[i].time_left--;
 
                    }
 
 
 
                    if(pcbdata[i].time_left  == 0)
 
                    {
 
                        pcbdata[i].state = 'F';
 
                        finishNumber4++;
 
 
 
                        printf("第%d个进程-- %s, ", i+1, pcbdata[i].name);
 
                        printf("到达时间 -- %d， 服务时间 -- %d\n",
 
                                pcbdata[i].time_start,
 
                                pcbdata[i].time_need);
 
                        printf("本进程正在运行-----------");
 
                        _sleep(1);
 
                        printf("运行完毕\n");
 
                        temp = currentTime4;
 
                        j = temp - pcbdata[i].time_start;
 
                        k = (double)j / pcbdata[i].time_need;
 
                        printf("完成时间 -- %d，周转时间 -- %d, 带权周转时间 -- %.1f\n", temp, j, k);
 
                        break;
 
                    }
 
                }
 
                /* 完成第一个队列的时间片仍未完成，然后添加到第二个队列 */
 
                if(pcbdata[i].state == 'R')
 
                {
 
                    enqueue(i, q2);
 
                    pcbdata[i].state = 'W';
 
                }
 
            }
 
            else if(q2Flag)
 
            {
 
                if(pcbdata[i].leftTime == 0)
 
                    pcbdata[i].leftTime = time_unit2;
 
                for(count = 0; count < pcbdata[i].leftTime; count++)
 
                {
 
                    if(pcbdata[i].time_left != 0)
 
                    {
 
                        currentTime4++;
 
                        pcbdata[i].state = 'R';
 
                        pcbdata[i].time_used++;
 
                        pcbdata[i].time_left--;
 
                    }
 
 
 
 
 
                    if(pcbdata[i].time_left  == 0)
 
                    {
 
                        pcbdata[i].state = 'F';
 
                        finishNumber4++;
 
 
 
                        printf("第%d个进程-- %s, ", i+1, pcbdata[i].name);
 
                        printf("到达时间 -- %d， 服务时间 -- %d\n",
 
                                pcbdata[i].time_start,
 
                                pcbdata[i].time_need);
 
                        printf("本进程正在运行-----------");
 
                        _sleep(1);
 
                        printf("运行完毕\n");
 
                        temp = currentTime4;
 
                        j = temp - pcbdata[i].time_start;
 
                        k = (double)j / pcbdata[i].time_need;
 
                        printf("完成时间 -- %d，周转时间 -- %d, 带权周转时间 -- %.1f\n", temp, j, k);
 
                        break;
 
                    }
 
 
 
                    /* 每调用一个单位时间就判断是否第一级队列是否有进程抢占 */
 
                    for(int t = 0; t < num; t++)
 
                    {
 
                        if(currentTime4 >= pcbdata[t].time_start && pcbdata[t].state == 'T')
 
                        {
 
                            /* 入队 */
 
                            enqueue(t, q1);
 
                            pcbdata[t].state = 'W';
 
                        }
 
                    }
 
 
 
                    if(!isempty(q1))
 
                    {
 
                        /* 有第一队列的进程抢占 */
 
                        pcbdata[i].state = 'W';
 
                        /* 添加到队列末尾 */
 
                        enqueue(i, q2);
 
                        pcbdata[i].leftTime = pcbdata[i].leftTime - count - 1;
 
                        break;
 
                    }
 
                }
 
                if(pcbdata[i].state == 'R')
 
                {
 
                    enqueue(i, q3);
 
                    pcbdata[i].state = 'W';
 
                }
 
            }
 
            else if(q3Flag)
 
            {
 
                for(count = 0; count < time_unit3; count++)
 
                {
 
                    if(pcbdata[i].time_left != 0)
 
                    {
 
                        currentTime4++;
 
                        pcbdata[i].state = 'R';
 
                        pcbdata[i].time_used++;
 
                        pcbdata[i].time_left--;
 
                    }
 
 
 
                    /* 在经过一个单位时间后如果进程已经完成了 */
 
                    if(pcbdata[i].time_left  == 0)
 
                    {
 
                        pcbdata[i].state = 'F';
 
                        finishNumber4++;
 
 
 
                        printf("第%d个进程-- %s, ", i+1, pcbdata[i].name);
 
                        printf("到达时间 -- %d， 服务时间 -- %d\n",
 
                                pcbdata[i].time_start,
 
                                pcbdata[i].time_need);
 
                        printf("本进程正在运行-----------");
 
                        _sleep(1);
 
                        printf("运行完毕\n");
 
                        temp = currentTime4;
 
                        j = temp - pcbdata[i].time_start;
 
                        k = (double)j / pcbdata[i].time_need;
 
                        printf("完成时间 -- %d，周转时间 -- %d, 带权周转时间 -- %.1f\n", temp, j, k);
 
                        break;
 
                    }
 
 
 
                    for(int t = 0; t < num; t++)
 
                    {
 
                        if(currentTime4 >= pcbdata[t].time_start && pcbdata[t].state == 'T')
 
                        {
 
                            /* 入队 */
 
                            enqueue(t, q1);
 
                            pcbdata[t].state = 'W';
 
                        }
 
                    }
 
 
 
                    if(!isempty(q1))
 
                    {
 
                        pcbdata[i].state = 'W';
 
                        enqueue(i, q3);
 
                        pcbdata[i].leftTime = pcbdata[i].leftTime - count - 1;
 
                        break;
 
                    }
 
 
 
                    if(!isempty(q2))
 
                    {
 
                        pcbdata[i].state = 'W';
 
                        enqueue(i, q3);
 
                        pcbdata[i].leftTime = pcbdata[i].leftTime - count - 1;
 
                        break;
 
                    }
 
                }
 
            }
 
        }
 
    }

}
 
// 主函数
int main(){
	int i = 0, sch = 99;
	while(sch != 0){
		printf("\n请选择其中一种调度算法:\n");
		printf("(1)先来先服务FCFS\n");
		printf("(2)短作业优先SJF\n");
		printf("(3)高相应比HRF\n");
		printf("(4)时间片轮转Timeslice\n");
		printf("(5)多级反馈队列MRLA\n");
		printf("(0)退出\n");
		printf("请输入一个数字：");
		scanf("%d",&sch);
		switch (sch){
		case 1:FCFS();break;
		case 2: SJF();break;
		case 3:HRF();break;
		case 4:Timeslice();break;
		case 5:MRLA();break;
		case 0:	printf("退出程序\n");;break;
		}
	}
	_keygo();
}
 
 
//一下函数主要用于程序调试跟踪
 
//显示一个PCB的具体参数内容
void dis_pcb(PCB *pr){
	printf("%s的PCB:\n",pr->name);
	printf("标识符--%d,状态--%c,到达时间--%d\n",
		pr->id,pr->state,pr->time_start);
	printf("服务时间--%d,剩余运行时间--%d,已用时间--%d\n",
		pr->time_need,pr->time_left,pr->time_used);
	printf("-------------------------\n");
}
 
//像是所有进程PCB
void dis_pcb_all(){
	int i;
	printf("******当前所有进程状态*****\n");
	for(i = 0; i< num; i++)
		dis_pcb(&pcbdata[i]);
}
 
//显示就绪队列
void dis_ready(){
	int i;
	printf("当前就绪队列为：\n");
	for(i = 0; i< num-1; i++)
		printf("%s--",pcbdata[order[i]].name);
	printf("%s\n",pcbdata[order[i]].name);
}