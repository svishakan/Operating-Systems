typedef struct{
	char pid[10];		//process			ID
	int arrtime;		//arrival			time
	int bursttime;		//burst				time
	int resptime;		//resp				time 
	int waittime;		//wait				time
	int turntime;		//turnaround			time
	int comptime;		//completion			time
	int nextidle;		//idle time after completion
	int remtime;		//remaining 			time
	int temptime;		//current comp.		time for PPP
	int bufftime;		//buffering 		time for PPP
	int status;			//completed 			or not
	int queued;			//indicates whether or not process is in queue
	int prio;			//process priority
}pInfo;


typedef struct{
	int *pid;
	int front, rear;
	int capacity, size;
}Queue;



void initQueue(Queue *q, int cap){
	q->front = q->rear = -1;
	q->capacity = cap;
	q->pid = (int *)malloc(sizeof(int)*cap);
	q->size	= 0;
}

int isEmpty(Queue *q){
	if(q->size==0)
		return 1;
	return 0;
}

int isFull(Queue *q){
	if(q->size==q->capacity)
		return 1;
	return 0;

}

void enQueue(Queue *q, int p){
	if(isFull(q))
		printf("\nQueue is Full! Cannot Enqueue any further!\n");
	else{
		q->rear=(q->rear+1)%q->capacity;
		q->pid[q->rear]=p;
		q->size++;	
	}

	if(q->front==-1)
		q->front=0;

	
}

int deQueue(Queue *q){
	int pop;
	pop = -1; 	//default, in case no ID returned
	
	if(isEmpty(q)){
		printf("\nQueue is Empty! Cannot Dequeue any further!\n");
		return pop;
	}

	pop= q->pid[q->front];
	if(q->rear==q->front){
		q->front=q->rear=-1;
	}
	else
		q->front=(q->front+1)%q->capacity;
	
	q->size--;
	
	
	return pop;
}

void displayQueue(Queue *q){
	int i=0;
	int ptr=q->front;
	if(isEmpty(q))
		printf("\nQueue is Empty!");
	else{
		printf("\nQueue Elements:\n\t");
		while(i<q->size){
			printf("%d ",q->pid[ptr]);		
			i++;
			ptr=(ptr+1)%q->capacity;
		}
	}
}
