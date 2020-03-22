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
    printf("Enter the File to be Transferred: ");    
    scanf("%[^\n]",msg);
    sleep(8);       //Waiting for Server to read the specified file and send output
	
    printf("\nContents of File: \n");
    printf("%s\n",msg);
    shmdt(msg);    

    shmctl(id, IPC_RMID, NULL); //destroying the shared memory & contents
    exit(0);

}


/*
OUTPUT:

(base) vishakan@Legion:~/Desktop/Operating-Systems/Ex5 InterProcess Communication$ gcc 2-Client.c -o c
(base) vishakan@Legion:~/Desktop/Operating-Systems/Ex5 InterProcess Communication$ ./c
Enter the File to be Transferred: sample.txt

Contents of File: 
Hi
This is Vikram V of CSE-C
Today is February 14!


(base) vishakan@Legion:~/Desktop/Operating-Systems/Ex5 InterProcess Communication$ ./c
Enter the File to be Transferred: samp.txt 

Contents of File: 
File Not Available
*/
