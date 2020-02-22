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
int p = 0, c = 0, buff_size = 0;

void consumeRandomNumbers()
{ // Consumer function to consume random numbers generated
	int i = 0;
	buff_size = strlen(buff);
	while (i < buff_size)
	{
		printf("\nConsumer %d trying to acquire Semaphore Full. \n", getpid());
		sem_wait(full);

		printf("\nConsumer %d successfully acquired Semaphore Full. \n", getpid());

		printf("\nConsumer %d trying to acquire Semaphore Mutex. \n", getpid());
		sem_wait(mutex);

		printf("\nConsumer %d successfully acquired Semaphore Mutex.\n", getpid());

		printf("\nConsumer %d Consumed Item: [ %c ] \n", getpid(), buff[c]);
		buff[c] = ' ';
		c++;

		printf("\nItems in Buffer: %d \n", (buff_size - (++i)));
		sem_post(mutex);

		printf("\nConsumer %d released Semaphore Mutex. \n", getpid());
		sem_post(empty);

		printf("\nConsumer %d released Semaphore Empty. \n", getpid());
		sleep(1);
	}
}

int main()
{
	int i = 0;

	segid = shmget(100, 5, IPC_EXCL | SHMPERM);
	empty_id = shmget(101, sizeof(sem_t), IPC_EXCL | SHMPERM);
	full_id = shmget(102, sizeof(sem_t), IPC_EXCL | SHMPERM);
	mutex_id = shmget(103, sizeof(sem_t), IPC_EXCL | SHMPERM);
	buff = shmat(segid, (char *)0, 0);
	empty = shmat(empty_id, (char *)0, 0);
	full = shmat(full_id, (char *)0, 0);
	mutex = shmat(mutex_id, (char *)0, 0);
	

	printf("\nConsumer Process Started To Consume Random Numbers. \n");

	consumeRandomNumbers();

	shmdt(buff);
	shmdt(empty);
	shmdt(full);
	shmdt(mutex);
	shmctl(segid, IPC_RMID, NULL);
	shmctl(empty_id, IPC_RMID, NULL);
	shmctl(full_id, IPC_RMID, NULL);
	shmctl(mutex_id, IPC_RMID, NULL);
	//semctl(empty_id, 0, IPC_RMID, NULL);
	//semctl(full_id, 0, IPC_RMID, NULL);
	//semctl(mutex_id, 0, IPC_RMID, NULL);
	//sem_destroy(empty);
	//sem_destroy(full);
	//sem_destroy(mutex);
	
	//NOTE: semctl() or sem_destroy() causes seg.faults after one successful execution.

	printf("\nConsumer process exited. \n\n");
	return (0);
}

/*
OUTPUT:

(base) vishakan@Legion:~/Desktop/Operating-Systems/Ex6 Semaphores$ ./c

Consumer Process Started To Consume Random Numbers. 

Consumer 2507 trying to acquire Semaphore Full. 

Consumer 2507 successfully acquired Semaphore Full. 

Consumer 2507 trying to acquire Semaphore Mutex. 

Consumer 2507 successfully acquired Semaphore Mutex.

Consumer 2507 Consumed Item: [ 3 ] 

Items in Buffer: 9 

Consumer 2507 released Semaphore Mutex. 

Consumer 2507 released Semaphore Empty. 

Consumer 2507 trying to acquire Semaphore Full. 

Consumer 2507 successfully acquired Semaphore Full. 

Consumer 2507 trying to acquire Semaphore Mutex. 

Consumer 2507 successfully acquired Semaphore Mutex.

Consumer 2507 Consumed Item: [ 6 ] 

Items in Buffer: 8 

Consumer 2507 released Semaphore Mutex. 

Consumer 2507 released Semaphore Empty. 

Consumer 2507 trying to acquire Semaphore Full. 

Consumer 2507 successfully acquired Semaphore Full. 

Consumer 2507 trying to acquire Semaphore Mutex. 

Consumer 2507 successfully acquired Semaphore Mutex.

Consumer 2507 Consumed Item: [ 7 ] 

Items in Buffer: 7 

Consumer 2507 released Semaphore Mutex. 

Consumer 2507 released Semaphore Empty. 

Consumer 2507 trying to acquire Semaphore Full. 

Consumer 2507 successfully acquired Semaphore Full. 

Consumer 2507 trying to acquire Semaphore Mutex. 

Consumer 2507 successfully acquired Semaphore Mutex.

Consumer 2507 Consumed Item: [ 5 ] 

Items in Buffer: 6 

Consumer 2507 released Semaphore Mutex. 

Consumer 2507 released Semaphore Empty. 

Consumer 2507 trying to acquire Semaphore Full. 

Consumer 2507 successfully acquired Semaphore Full. 

Consumer 2507 trying to acquire Semaphore Mutex. 

Consumer 2507 successfully acquired Semaphore Mutex.

Consumer 2507 Consumed Item: [ 3 ] 

Items in Buffer: 5 

Consumer 2507 released Semaphore Mutex. 

Consumer 2507 released Semaphore Empty. 

Consumer process exited. 

*/