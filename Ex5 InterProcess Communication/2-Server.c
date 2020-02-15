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
    char *contents = (char *)malloc(1024);
    char *buf = (char *)malloc(255);
    FILE *src;

    id = shmget(111, 1024, IPC_CREAT|00666);    //getting ID of shared mem.
    msg = shmat(id, NULL, 0);                   //msg var. is now a shared mem. var
    printf("\nReceived the following file name to be opened: %s\n",msg);  
	
    src = fopen(msg, "r");      //opening the file that came thro' the client
    printf("\nFile Opening...");

    if(src == NULL){    //if file is not available
	    strcpy(msg, "File Not Available");
        printf("\nProcess finished execution with File Not Found error.\n");
        exit(0);
    }

    
    while(fgets(buf, 255, src) != NULL){    //copying contents to a temp. var. using fgets()
        strcat(contents, buf);
    };

    fclose(src);
    strcpy(msg, contents);      //copying contents to shared mem. var.
    shmdt(msg);
    printf("\nProcess finished execution without errors.\n");
    exit(0);
}


/*
OUTPUT:

vishakan@Legion:~/Desktop/Operating-Systems/Ex5 InterProcen$ gcc 2-Server.c -o s
(base) vishakan@Legion:~/Desktop/Operating-Systems/Ex5 InterProcess Communication$ ./s

Received the following file name to be opened: sample.txt

File Opening...
Process finished execution.


(base) vishakan@Legion:~/Desktop/Operating-Systems/Ex5 InterProcess Communication$ ./s

Received the following file name to be opened: samp.txt

File Opening...
Process finished execution with File Not Found error.

*/
