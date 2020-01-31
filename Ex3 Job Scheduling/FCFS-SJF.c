
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct{
	char pid[10];		//process			ID
	int arrtime;		//arrival			time
	int bursttime;		//burst				time
	int resptime;		//resp				time 
	int waittime;		//wait				time
	int turntime;		//turnaround			time
	int comptime;		//completion			time
	int nextidle;		//idle time after completion
	int bufftime;		//buffering 		time for SJFP
	int temptime;		//current comp.		time for SJFP
	int status;		//completed 		or not
}pInfo;


pInfo	*processInput(int numprocess);
pInfo	*processArrivalSort(pInfo *process, int numprocess);
void	processPrinter(pInfo *process, int numprocess);
pInfo	*schedulerFCFS(pInfo *process, int numprocess);
pInfo	*schedulerSJFP(pInfo *process, int numprocess);
pInfo	*schedulerSJFNP(pInfo *process, int numprocess);
int		statusCheck(pInfo *process, int numprocess);
void	chartPrinter(pInfo *process, int numprocess);
void	chartGantt(pInfo *process, int numprocess);	



int	main(void){
	int i,numprocess,opt=1,p=0;

	while(opt != 0){
		printf("\n\t\tCPU SCHEDULING ALGORITHMS\n");
		printf("\n\t1. FCFS\n\t2. SJF\n\t0. EXIT\n\tYour Option -> ");
		scanf("%d",&opt);

		if(opt == 1){
			printf("\n\tFCFS CPU SCHEDULER\n");
		}

		else if(opt == 2){
			printf("\n\tSJF CPU SCHEDULER\n");
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
			process = schedulerFCFS(process, numprocess);
			chartGantt(process, numprocess);
		}

		else if(opt == 2 && p == 1){
			process = schedulerSJFP(process, numprocess);
		}

		else if(opt == 2 && p == 2){
			process = schedulerSJFNP(process, numprocess);
			chartGantt(process,numprocess);
		}

		else{
			return 0;
		}		
		
		chartPrinter(process,numprocess);

	}


	return 0;
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
		process[i].bufftime = 0;
		process[i].temptime = 0;
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

void processPrinter(pInfo *process, int numprocess){
	int i = 0;
	printf("\nProcesses Currently In : \n\n");
	while(i<numprocess){
		printf("\nProcess ID 	: %s",process[i].pid);
		printf("\nArrival Time 	: %d",process[i].arrtime);
		printf("\nBurst Time 	: %d",process[i].bursttime);
		printf("\nIdle Time 	: %d",process[i].nextidle);
		printf("\n");
		++i;
	}
	printf("\n");
	return;
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

pInfo *schedulerFCFS(pInfo *process, int numprocess){
	int i = 0, currentwait = process[0].arrtime, idletime = 0;
	
	while(i<numprocess){
		process[i].waittime = currentwait - process[i].arrtime + idletime;
		process[i].resptime = process[i].waittime;
		process[i].comptime = currentwait + process[i].bursttime + idletime;
		process[i].turntime = process[i].comptime - process[i].arrtime;
		currentwait += process[i].bursttime;
		
		if(i+1<numprocess && process[i+1].arrtime > currentwait){
			idletime = process[i+1].arrtime - currentwait;
			process[i].nextidle = idletime;	//indicates the amt of idletime after process[i]
		}
		else{
			idletime = 0;
		}

		++i;
	}

	return process;
}

pInfo *schedulerSJFNP(pInfo *process, int numprocess){
	int i = 0, currentwait = process[0].arrtime, idletime = 0, j = 0, nextproc = 0, iter = 1;
	pInfo temp;


	while(i<numprocess){
		//++iter;	//keeping no. of iterations in check
		process[i].waittime = currentwait - process[i].arrtime + idletime;
		process[i].resptime = process[i].waittime;
		//printf("\n%s %d %d %d",process[i].pid, currentwait, idletime, process[i].bursttime);
		process[i].comptime = currentwait + process[i].bursttime + idletime;
		process[i].turntime = process[i].comptime - process[i].arrtime;
		process[i].status = 1;		//indicates process completion
		
		currentwait = process[i].comptime;
		nextproc = i;
		
		for(j=0;j<numprocess;j++){		//finding the next process
			if(process[j].status == 0){
				if(process[j].arrtime <= process[i].comptime){
					if(nextproc == i)
						nextproc = j;
					else{
						if(process[j].bursttime < process[nextproc].bursttime)
							nextproc = j;
					}
					
				}
			}	
		}
		
		idletime = 0;			//setting default idletime = 0

		if(nextproc == i)		//if nextproc is unchanged
			nextproc+=1;

		if(i+1<numprocess){
			temp = process[i+1];		//swapping the nextproc with i+1 to follow order
			process[i+1] = process[nextproc];
			process[nextproc] = temp;
			//printf("\nPID : %s  i Val : %d",process[i+1].pid, i+1);

			if(process[i+1].arrtime > currentwait){		//finding idletime if any
				idletime = process[i+1].arrtime - currentwait;
				process[i].nextidle = idletime;	//indicates the amt of idletime after process[i]
			}
		}

		++i;
	}
	

	return process;
}

int statusChecker(pInfo *process, int numprocess){
	int i = 0, min = 0, flag = -1;
	while(i <  numprocess){
		if(process[i].status == 1){
			++i;
			continue;
		}
			
		else{
			flag = i;
			break;
		}
		++i;
	}

	if(flag == -1)		//returning -1 if all processes are complete.
		return -1;

	i = min + 1;
	min = flag;

	while(process[i].status == 0 && i < numprocess){
		if(process[i].bursttime < process[min].bursttime){
			min = i;
		}
		++i;
	}
	
	return min;		//returning the minimum burst time unfinished process
	
}

pInfo *schedulerSJFP(pInfo *process, int numprocess){
	int i = 0, currentwait = process[0].arrtime, idletime = 0;
	int curtime = 0, nextjob = 0, j = 0, jobsdone = 0, job = 0, prevjob = 0;
	pInfo *queue, temp;	//queue for Gantt Chart
	queue = (pInfo *)malloc(sizeof(pInfo)*100);
	

	while(statusChecker(process, numprocess) != -1){		//checking if all jobs are complete
		job = nextjob;
		int etime = process[job].bursttime + process[job].arrtime;
		nextjob = -1;

		//finding jobs that might trigger pre-empt
		for(j=job+1;(process[j].arrtime < etime) && (j < numprocess) && (j!=job);j = (j+1)%numprocess){	
			int temp = process[j].arrtime + process[j].bursttime;
			if(process[j].status == 1 || j == job)
				continue;
			if(nextjob == -1 && temp <= etime)
				nextjob = j;
			
			else if(temp <= process[nextjob].bursttime + process[nextjob].arrtime)
				if(process[j].bursttime < process[nextjob].bursttime)	//Shorter Job First
					nextjob = j;
				else
					continue;
		}


		if(nextjob == -1){		//if there's no job to pre-empt
			if(process[job].bufftime == 0){		//if the process is hasn't been pre-empted before
				process[job].resptime = currentwait - process[job].arrtime;	//- idletime
				process[job].comptime = currentwait + process[job].bursttime;	//- idletime
				
			}
			else{
				process[job].comptime = -process[job].comptime;
				process[job].comptime += (2* process[prevjob].comptime - process[job].arrtime);

			}

			process[job].waittime += (currentwait - process[job].arrtime - 2*process[job].waittime);	// - idletime
			process[job].status = 1;
			process[job].turntime = process[job].comptime - process[job].arrtime;
			process[job].temptime = process[job].bursttime;
			nextjob = statusChecker(process,numprocess);
			currentwait += process[job].bursttime;
			temp = process[job];
			
		}

		else{			//pre-empt current job and queue next job
			int stoptime = process[nextjob].arrtime;

			if(process[job].bufftime == 0){
				process[job].resptime = currentwait - process[job].arrtime;	//- idletime
				process[job].comptime = stoptime - currentwait;
			}

			temp = process[job];
			process[job].bufftime = process[job].bursttime - stoptime;

			process[job].waittime = currentwait - process[job].arrtime;	// - idletime
			process[job].turntime = process[job].comptime - process[job].arrtime;
			
			process[job].temptime = process[nextjob].arrtime - currentwait;
			temp.comptime = process[job].temptime + currentwait;
			currentwait += process[job].temptime;
		}
		
		//printf("\n%d : %d",currentwait, process[nextjob].arrtime);

		if(process[nextjob].arrtime > temp.comptime)	//finding idletime if any
			idletime = process[nextjob].arrtime - temp.comptime;
				//indicates the amt of idletime after process[i]
		else
			idletime = 0;
			
		//printf("\n%d, %d",job, idletime);

		process[job].nextidle = idletime;
		temp.nextidle = idletime;
		queue[i++] = temp;	//queueing for Gantt Chart
		prevjob = job;

	}

	chartGantt(queue,i);
	return process;
}


/*
OUTPUT:
(base) vishakan@Legion:~/Desktop/Operating-Systems/Ex3 FCFS & SJF$ gcc Scheduler.c -o j
(base) vishakan@Legion:~/Desktop/Operating-Systems/Ex3 FCFS & SJF$ ./j
                CPU SCHEDULING ALGORITHMS
        1. FCFS
        2. SJF
        0. EXIT
        Your Option -> 1
        FCFS CPU SCHEDULER
Enter the Number of Processes : 3
Enter the Process ID: P1
Enter the Arrival Time of P1: 0
Enter the CPU Burst Time of P1: 3
Enter the Process ID: P2
Enter the Arrival Time of P2: 1
Enter the CPU Burst Time of P2: 2
Enter the Process ID: P3
Enter the Arrival Time of P3: 5
Enter the CPU Burst Time of P3: 6
Gantt Chart:
------------------------
| P1    | P2    | P3    |
------------------------
0       3       5       11   
---------------------------------------------------------------------
| P. ID | A. Time | B. Time | C. Time | T. Time | W. Time | R. Time |
---------------------------------------------------------------------
| P1    | 0       | 3       | 3       | 3       | 0       | 0       |
| P2    | 1       | 2       | 5       | 4       | 2       | 2       |
| P3    | 5       | 6       | 11      | 6       | 0       | 0       |
---------------------------------------------------------------------
Average Waiting Time    :       0.67
Average Response Time   :       0.67
---------------------------------------------------------------------
                CPU SCHEDULING ALGORITHMS
        1. FCFS
        2. SJF
        0. EXIT
        Your Option -> 2
        SJF CPU SCHEDULER
        1. PREEMPTIVE
        2. NON-PREEMPTIVE
        Your Option -> 1
Enter the Number of Processes : 3
Enter the Process ID: P1
Enter the Arrival Time of P1: 0
Enter the CPU Burst Time of P1: 4
Enter the Process ID: P2
Enter the Arrival Time of P2: 3
Enter the CPU Burst Time of P2: 5
Enter the Process ID: P3
Enter the Arrival Time of P3: 4
Enter the CPU Burst Time of P3: 1
Gantt Chart:
------------------------
| P1    | P3    | P2    |
------------------------
0       4       5       10   
---------------------------------------------------------------------
| P. ID | A. Time | B. Time | C. Time | T. Time | W. Time | R. Time |
---------------------------------------------------------------------
| P1    | 0       | 4       | 4       | 4       | 0       | 0       |
| P2    | 3       | 5       | 10      | 7       | 2       | 2       |
| P3    | 4       | 1       | 5       | 1       | 0       | 0       |
---------------------------------------------------------------------
Average Waiting Time    :       0.67
Average Response Time   :       0.67
---------------------------------------------------------------------

		CPU SCHEDULING ALGORITHMS

	1. FCFS
	2. SJF
	0. EXIT
	Your Option -> 2

	SJF CPU SCHEDULER
	1. PREEMPTIVE
	2. NON-PREEMPTIVE
	Your Option -> 1

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
--------------------------------------------------------------------------------
| IDLE  | p1    | p3    | p2    | p4    | p5    | p4    | IDLE  | p6    | p7    |
--------------------------------------------------------------------------------
0    	2    	5    	6    	8    	9    	11   	14   	 15     21   	29   	

---------------------------------------------------------------------
| P. ID | A. Time | B. Time | C. Time | T. Time | W. Time | R. Time |
---------------------------------------------------------------------
| p1    | 2       | 3       | 5       | 3       | 0       | 0       |
| p2    | 4       | 2       | 8       | 4       | 2       | 2       |
| p3    | 5       | 1       | 6       | 1       | 0       | 0       |
| p4    | 7       | 4       | 14      | 7       | 3       | 1       |
| p5    | 9       | 2       | 11      | 2       | 0       | 0       |
| p6    | 15      | 6       | 21      | 6       | 0       | 0       |
| p7    | 16      | 8       | 29      | 13      | 5       | 5       |
---------------------------------------------------------------------

Average Waiting Time	:	1.43
Average Response Time	:	1.14
---------------------------------------------------------------------

*/



