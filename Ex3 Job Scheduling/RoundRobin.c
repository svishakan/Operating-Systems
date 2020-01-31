#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Queue.h"

/* DEFINED IN QUEUE.H

typedef struct{
	char pid[10];		//process			ID
	int arrtime;		//arrival			time
	int bursttime;		//burst				time
	int resptime;		//resp				time
	int waittime;		//wait				time
	int turntime;		//turnaround			time
	int comptime;		//completion			time
	int nextidle;		//idle time after completion
	int remtime			//remaining 			time
	int status;			//completed 			or not
	int queued;			//indicates whether or not process is in queue
}pInfo;

*/



pInfo	*processInput(int numprocess);
pInfo	*processArrivalSort(pInfo *process, int numprocess);
void	chartPrinter(pInfo *process, int numprocess);
void	chartGantt(pInfo *process, int numprocess);
pInfo 	*schedulerRR(pInfo *process, int numprocess, int quantum);
int 	min(int a, int b);

int main(void){

	int i, numprocess, opt=1, p=0, quantum = 0;

	while(opt != 0){
		printf("\n\t\tCPU SCHEDULING ALGORITHMS\n");
		printf("\n\t1. ROUND ROBIN\n\t2. PRIORITY\n\t0. EXIT\n\tYour Option -> ");
		scanf("%d",&opt);

		if(opt == 1){
			printf("\n\tROUND ROBIN CPU SCHEDULER\n");
			printf("\nEnter Time Quantum : ");
			scanf("%d", &quantum);
		}

		else if(opt == 2){
			printf("\n\tPRIORITY CPU SCHEDULER\n");
			printf("\t1. PREEMPTIVE\n\t2. NON-PREEMPTIVE\n\tYour Option -> ");
			scanf("%d",&p);

			while(p>2 || p<=0){
				printf("\n\tInvalid Option!\n");
				printf("\t1. PREEMPTIVE\n\t2. NON-PREEMPTIVE\n\tYour Option -> ");
				scanf("%d",&p);
			}

		}

		else if(opt == 0){
			printf("\n\tThank You!\n");
			return 0;
		}

		else{
			printf("\n\tInvalid Option!\n");
			return 0;
		}

		printf("\nEnter the Number of Processes : ");
		scanf("%d",&numprocess);

		pInfo *process;
		process = (pInfo *)malloc(sizeof(pInfo)*numprocess);

		process = processInput(numprocess);
		process = processArrivalSort(process, numprocess);

		if(opt == 1){
			process = schedulerRR(process, numprocess, quantum);
		}

		/*
		else if(opt == 2 && p == 1){
			process = schedulerSJFP(process, numprocess);
		}

		else if(opt == 2 && p ="= 2){
			process = schedulerSJFNP(process, numprocess);
			chartGantt(process,numprocess);
		}
		*/

		else{
			return 0;
		}

	}

	return 0;
}


int min(int a, int b){
	if(a <= b)
		return a;

	return b;

}



pInfo 	*processInput(int numprocess){
	pInfo *process;
	process = (pInfo *)malloc(sizeof(pInfo)*numprocess);
	int i=0;

	while(i<numprocess){
		printf("\nEnter the Process ID: ");
		scanf("%s",process[i].pid);
		printf("\nEnter the Arrival Time of %s: ",process[i].pid);
		scanf("%d",&process[i].arrtime);
		printf("\nEnter the CPU Burst Time of %s: ",process[i].pid);
		scanf("%d",&process[i].bursttime);
		process[i].waittime = 0;
		process[i].resptime = 0;
		process[i].turntime = 0;
		process[i].nextidle = 0;
		process[i].status = 0;
		process[i].comptime = 0;
		process[i].queued = 0;
		process[i].remtime = process[i].bursttime;

		++i;

	}
	return process;
}



pInfo *processArrivalSort(pInfo *process, int numprocess){
	pInfo proctemp;
	int i=0,j=0,temp=0;

	for(i=0;i<numprocess;++i){
		temp = i;
		for(j=i+1;j<numprocess;++j){
			if(process[temp].arrtime > process[j].arrtime){
				temp = j;
			}
		proctemp = process[i];
		process[i] = process[temp];
		process[temp] = proctemp;
		}
	}

	return process;
}



void chartPrinter(pInfo *process, int numprocess){
	int i=0;
	float avgwait=0, avgresponse=0;

	printf("\n---------------------------------------------------------------------\n");
	printf("| %5s | %7s | %7s | %7s | %7s | %7s | %7s |", "P. ID", "A. Time", "B. Time", "C. Time", "T. Time", "W. Time", "R. Time");
	printf("\n---------------------------------------------------------------------\n");

	while(i<numprocess){
		avgwait += process[i].waittime;
		avgresponse += process[i].resptime;
		printf("| %-5s | %-7d | %-7d | %-7d | %-7d | %-7d | %-7d |\n",process[i].pid, process[i].arrtime, process[i].bursttime, process[i].comptime, process[i].turntime, process[i].waittime, process[i].resptime);

		++i;
	}

	avgwait/=numprocess;
	avgresponse/=numprocess;

	printf("---------------------------------------------------------------------\n");
	printf("\nAverage Waiting Time	:	%.2f\nAverage Response Time	:	%.2f",avgwait, avgresponse);
	printf("\n---------------------------------------------------------------------\n");

	return;
}

void chartGantt(pInfo *process, int numprocess){
	int i = 0, numidles = 0, n = 0;

	printf("\nGantt Chart:\n");

	for(i=0;i<numprocess;++i){
		if(process[i].nextidle !=0)
			++numidles;
	}

	n = numprocess + numidles;

	if(process[0].arrtime != 0)		//accounting for init idle time, if any
		++n;

	for(i=0;i<n;++i)
		printf("--------");

	printf("\n|");

	if(process[0].arrtime != 0)
		printf(" %-5s |","IDLE");

	for(i=0;i<numprocess;++i){

		printf(" %-5s |",process[i].pid);

		if(process[i].nextidle != 0){
			printf(" %-5s |","IDLE");
		}
	}

	printf("\n");

	for(i=0;i<n;++i)
		printf("--------");


	if(process[0].arrtime != 0)
		printf("\n%-5d\t",0);
	else
		printf("\n");

	printf("%-5d\t",process[0].arrtime - process[i].waittime);

	for(i=0;i<numprocess;++i){
		printf("%-5d\t",process[i].comptime);

		if(process[i].nextidle != 0){
			printf(" %-5d  ",process[i].comptime + process[i].nextidle);
		}
	}
	printf("\n");
}



int statusChecker(pInfo *process, int numprocess){
	int i = 0;
	while(i <  numprocess){
		if(process[i].status == 1){
			++i;
			continue;
		}

		else{
			return i;
		}
	}

	return -1;
}



pInfo	*schedulerRR(pInfo *process, int numprocess, int quantum){
	Queue *ready;
	ready = (Queue *)malloc(sizeof(Queue));
	pInfo *listGantt;
	listGantt = (pInfo *)malloc(sizeof(pInfo)*100);
	pInfo temp;
	initQueue(ready, numprocess);

	int currentwait = process[0].arrtime, job = 0, nextjob = 0, idletime = 0, i = 0, elapsedtime = 0;

	while(statusChecker(process, numprocess) != -1){ 
		job = nextjob;
		elapsedtime = min(quantum, process[job].remtime);

		if(process[job].remtime == process[job].bursttime){		//new process, first time executing

			if(process[job].remtime <= quantum){				//if it can execute within the time quantum
				process[job].status = 1;
				process[job].waittime = currentwait + idletime - process[job].arrtime;
				process[job].comptime = currentwait + elapsedtime;
				process[job].turntime = process[job].comptime - process[job].arrtime;
				process[job].remtime = 0;

				//for Gantt Chart
				temp = process[job];

			}

			else{		//else, incomplete process.
				process[job].waittime = currentwait - process[job].arrtime + idletime;
				process[job].remtime -= quantum;

				//for Gantt Chart
				temp = process[job];
				temp.comptime = currentwait + quantum;
			}

			process[job].resptime = currentwait - process[job].arrtime;

		}

		else if(process[job].remtime <= quantum){	//if the process has been pre-empted before, and is executing again,
			process[job].status = 1;				//and can execute within time quantum
			process[job].comptime = currentwait + elapsedtime;
			process[job].turntime = process[job].comptime - process[job].arrtime;
			process[job].waittime = process[job].turntime - process[job].bursttime;
			process[job].remtime = 0;
			temp = process[job];
		}

		else{			//otherwise.
			process[job].remtime -= quantum;
			process[job].waittime += (currentwait - quantum);

			//for Gantt Chart
			temp = process[job];
			temp.comptime = currentwait + quantum;
		}

		int j = job+1;	//going to the FCFS-wise next job

		currentwait += elapsedtime;

		//queueing algorithm

		while(j < numprocess && process[j].arrtime <= currentwait){		//Queueing waiting processes to the RQ
			if(process[j].status == 0 && process[j].queued == 0){
				process[j].queued = 1;
				enQueue(ready, j);
			}
			j++;
		}

		if(process[job].status == 0 && process[job].queued == 0){	//If the process currently executed hasn't completed yet.
			enQueue(ready, job);
			process[job].queued = 1;
		}

		j = job + 1;
		while(process[j].arrtime <= currentwait)	//suppose the RQ is empty, and there is an idletime case
			j = j + 1;								//we shouldn't pick a process thats' already pre-empted and done before

		if(isEmpty(ready) && j < numprocess){	
			idletime = process[j].arrtime - currentwait;
			currentwait = process[j].arrtime;
			temp.nextidle = idletime;
			nextjob = j;
			listGantt[i++] = temp;
			continue;
		}
		else{
			idletime = 0;
		}

		listGantt[i++] = temp;

		
		if(!isEmpty(ready)){
			nextjob = deQueue(ready);
			process[nextjob].queued = 0;
		}
		else
			break;
	}

	//check for idletimes
	chartGantt(listGantt, i);
	chartPrinter(process, numprocess);

	return process;
}


/*
OUTPUT:
(base) vishakan@Legion:~/Desktop/Operating-Systems/Ex3 Job Scheduling$ gcc RoundRobin.c -o r
(base) vishakan@Legion:~/Desktop/Operating-Systems/Ex3 Job Scheduling$ ./r


		CPU SCHEDULING ALGORITHMS

	1. ROUND ROBIN
	2. PRIORITY
	0. EXIT
	Your Option -> 1

	ROUND ROBIN CPU SCHEDULER

Enter Time Quantum : 2

Enter the Number of Processes : 7

Enter the Process ID: p1

Enter the Arrival Time of p1: 2

Enter the CPU Burst Time of p1: 3

Enter the Process ID: p2

Enter the Arrival Time of p2: 4

Enter the CPU Burst Time of p2: 2

Enter the Process ID: p3

Enter the Arrival Time of p3: 5

Enter the CPU Burst Time of p3: 1

Enter the Process ID: p4

Enter the Arrival Time of p4: 7

Enter the CPU Burst Time of p4: 4

Enter the Process ID: p5

Enter the Arrival Time of p5: 9

Enter the CPU Burst Time of p5: 2

Enter the Process ID: p6

Enter the Arrival Time of p6: 15

Enter the CPU Burst Time of p6: 6

Enter the Process ID: p7

Enter the Arrival Time of p7: 16

Enter the CPU Burst Time of p7: 8

Gantt Chart:
--------------------------------------------------------------------------------------------------------------------------------
| IDLE  | p1    | p2    | p1    | p3    | p4    | p5    | p4    | IDLE  | p6    | p7    | p6    | p7    | p6    | p7    | p7    |
--------------------------------------------------------------------------------------------------------------------------------
0    	2    	4    	6    	7    	8    	10   	12   	14   	 15     17   	19   	21   	23   	25   	27   	29   	

---------------------------------------------------------------------
| P. ID | A. Time | B. Time | C. Time | T. Time | W. Time | R. Time |
---------------------------------------------------------------------
| p1    | 2       | 3       | 7       | 5       | 2       | 0       |
| p2    | 4       | 2       | 6       | 2       | 0       | 0       |
| p3    | 5       | 1       | 8       | 3       | 2       | 2       |
| p4    | 7       | 4       | 14      | 7       | 3       | 1       |
| p5    | 9       | 2       | 12      | 3       | 1       | 1       |
| p6    | 15      | 6       | 25      | 10      | 4       | 0       |
| p7    | 16      | 8       | 29      | 13      | 5       | 1       |
---------------------------------------------------------------------

Average Waiting Time	:	2.43
Average Response Time	:	0.71
---------------------------------------------------------------------

*/