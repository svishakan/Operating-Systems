#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h> // for semaphore operations sem_init,sem_wait,sem_post
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <sys/types.h>

extern int errno;

#define VARSIZE 1 /* size of shared variable=1byte*/
#define INPUTSIZE 20
#define SHMPERM 0666 /* shared memory permissions */

int segid; /* id for shared memory buffer */
int empty_id;
int full_id;
int mutex_id;
char *buff;
sem_t *empty;
sem_t *full;
sem_t *mutex;
int p = 0, c = 0, n = 0;

void produceRandomNumbers()
{ // Producer function to generate random numbers
	int i = 0, random_num = 0;
	while (i < n)
	{
		random_num = random() % n;

		printf("\nProducer %d trying to acquire Semaphore Empty. \n", getpid());
		sem_wait(empty);

		printf("\nProducer %d successfully acquired Semaphore Empty. \n", getpid());

		printf("\nProducer %d trying to acquire Semaphore Mutex. \n", getpid());
		sem_wait(mutex);

		printf("\nProducer %d successfully acquired Semaphore Mutex. \n", getpid());
		buff[p] = (char)random_num + 48; //converting it to char to store in array

		printf("\nProducer %d Produced Item: [ %d ] \n", getpid(), random_num);
		i++;
		p++;

		printf("\nItems in Buffer: %d \n", p);
		sem_post(mutex);

		printf("\nProducer %d released Semaphore Mutex. \n", getpid());
		sem_post(full);

		printf("\nProducer %d released Semaphore Full. \n", getpid());
		sleep(2);
	}
}

int main()
{
	int i = 0;
	segid = shmget(100, 5, IPC_CREAT | IPC_EXCL | SHMPERM);
	empty_id = shmget(101, sizeof(sem_t), IPC_CREAT | IPC_EXCL | SHMPERM);
	full_id = shmget(102, sizeof(sem_t), IPC_CREAT | IPC_EXCL | SHMPERM);
	mutex_id = shmget(103, sizeof(sem_t), IPC_CREAT | IPC_EXCL | SHMPERM);
	buff = shmat(segid, (char *)0, 0);
	empty = shmat(empty_id, (char *)0, 0);
	full = shmat(full_id, (char *)0, 0);
	mutex = shmat(mutex_id, (char *)0, 0);

	printf("\nEnter the number of random numbers to generate: ");
	scanf("%d", &n);

	// Initializing Semaphores Empty , Full & Mutex
	sem_init(empty, 1, n);
	sem_init(full, 1, 0);
	sem_init(mutex, 1, 1);

	printf("\nProducer Process Started To Produce Random Numbers. \n");
	produceRandomNumbers();

	shmdt(buff);
	shmdt(empty);
	shmdt(full);
	shmdt(mutex);

	printf("\nProducer Process Exited. \n\n");
	return (0);
}

/*
OUTPUT:

(base) vishakan@Legion:~/Desktop/Operating-Systems/Ex6 Semaphores$ ./p

Enter the number of random numbers to generate: 5

Producer Process Started To Produce Random Numbers. 

Producer 2446 trying to acquire Semaphore Empty. 

Producer 2446 successfully acquired Semaphore Empty. 

Producer 2446 trying to acquire Semaphore Mutex. 

Producer 2446 successfully acquired Semaphore Mutex. 

Producer 2446 Produced Item: [ 3 ] 

Items in Buffer: 1 

Producer 2446 released Semaphore Mutex. 

Producer 2446 released Semaphore Full. 

Producer 2446 trying to acquire Semaphore Empty. 

Producer 2446 successfully acquired Semaphore Empty. 

Producer 2446 trying to acquire Semaphore Mutex. 

Producer 2446 successfully acquired Semaphore Mutex. 

Producer 2446 Produced Item: [ 6 ] 

Items in Buffer: 2 

Producer 2446 released Semaphore Mutex. 

Producer 2446 released Semaphore Full. 

Producer 2446 trying to acquire Semaphore Empty. 

Producer 2446 successfully acquired Semaphore Empty. 

Producer 2446 trying to acquire Semaphore Mutex. 

Producer 2446 successfully acquired Semaphore Mutex. 

Producer 2446 Produced Item: [ 7 ] 

Items in Buffer: 3 

Producer 2446 released Semaphore Mutex. 

Producer 2446 released Semaphore Full. 

Producer 2446 trying to acquire Semaphore Empty. 

Producer 2446 successfully acquired Semaphore Empty. 

Producer 2446 trying to acquire Semaphore Mutex. 

Producer 2446 successfully acquired Semaphore Mutex. 

Producer 2446 Produced Item: [ 5 ] 

Items in Buffer: 4 

Producer 2446 released Semaphore Mutex. 

Producer 2446 released Semaphore Full. 

Producer 2446 trying to acquire Semaphore Empty. 

Producer 2446 successfully acquired Semaphore Empty. 

Producer 2446 trying to acquire Semaphore Mutex. 

Producer 2446 successfully acquired Semaphore Mutex. 

Producer 2446 Produced Item: [ 3 ] 

Items in Buffer: 5 

Producer 2446 released Semaphore Mutex. 

Producer 2446 released Semaphore Full. 

Producer Process Exited. 

*/
