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
	int status;		//completed 			or not
	int queued;		//indicates whether or not process is in queue
}pInfo;


typedef struct{
	int *pid;
	int front, rear;
	int capacity, size;
}Queue;


void initQueue(Queue *q,int cap){
	q->front=q->rear=-1;
	q->capacity=cap;
	q->a=(int *)malloc(sizeof(int)*cap);
}

int isEmpty(Queue *q){
	if(q->front>q->rear)
		return 1;
	return 0;
}

int isFull(Queue *q){
	if(q->rear==q->capacity-1)
		return 1;
	return 0;

}

void enQueue(Queue *q,int x){
	if(isFull(q))
		printf("\nQueue is Full! Cannot Enqueue any further!\n");
	else{
		q->a[++q->rear]=x;	
	}

	if(q->front=-1)
		q->front=0;
}

int deQueue(Queue *q){
	return q->a[q->front++];

}

void displayQueue(Queue *q){
	int i;
	printf("\nQueue Elements:\n\t");
	for(i=q->front;i<=q->rear;i++){
		printf("%d ",q->a[i]);
	}
	
}
