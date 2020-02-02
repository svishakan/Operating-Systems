#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio_ext.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(void){
    int id;
    char *msg;

    id = shmget(111, 1024, IPC_CREAT|00666);
    msg = shmat(id, NULL, 0);
    printf("\nReceived the following message:\n%s\n",msg);
    shmdt(msg);
    shmctl(id, IPC_RMID, NULL); //destroying the shared memory contents
    sleep(0);
    exit(0);
}


/*
OUTPUT:

(base) vishakan@Legion:~/Desktop/Operating-Systems/Ex4 InterProcess Communication$ gcc 2-Sender.c -o s
(base) vishakan@Legion:~/Desktop/Operating-Systems/Ex4 InterProcess Communication$ ./s
Enter the Message to be Transferred: Shared Memory Check, One...Two...Three!

(base) vishakan@Legion:~/Desktop/Operating-Systems/Ex4 InterProcess Communication$ gcc 2-Receiver.c -o r
(base) vishakan@Legion:~/Desktop/Operating-Systems/Ex4 InterProcess Communication$ ./r

Received the following message:
Shared Memory Check, One...Two...Three!

*/