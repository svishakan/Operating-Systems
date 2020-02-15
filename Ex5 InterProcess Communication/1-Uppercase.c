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

char *upperCase(char *word);    //function for converting to uppercase

int main(void){
    int pid, id;
    char *l_word, *u_word;

    id = shmget(111, 1024, IPC_CREAT|00666);    //identifier for shared memory
    pid = fork();   //child process and parent process share the same memory

    if(pid > 0){
        l_word = (char*) shmat(id,(void*)0,0);  //attaching to shared memory
        printf("\nEnter a Word: ");
        fgets(l_word, 1000, stdin);
        wait(0);
        shmdt(l_word);  //detaching from shared memory after placing the word
    }

    else{
        sleep(7);   //waiting for 7 seconds when the writer process is putting words into shared memory
        u_word = (char*) shmat(id,(void*)0,0);  //attaching once writer is complete
        printf("Received word : %s",upperCase(u_word));
        shmdt(u_word);  //detach
        exit(0);
    }

}


char *upperCase(char *word){
    int len = strlen(word);
    int i = 0;
    char *uword;
    uword = (char *)malloc(sizeof(char)*len);

    for(;i<len;i++)
        uword[i] = toupper(word[i]);
    
    return uword;
}

/*
OUTPUT:

(base) vishakan@Legion:~/Desktop/Operating-Systems/Ex4 InterProcess Communication$ gcc 1-Uppercase.c -o u
(base) vishakan@Legion:~/Desktop/Operating-Systems/Ex4 InterProcess Communication$ ./u

Enter a Word: interprocess communication
Received word : INTERPROCESS COMMUNICATION

*/
