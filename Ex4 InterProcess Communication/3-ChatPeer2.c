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
    char msg[100], *buffer;
    char temp1[100], temp2[100];
    
    printf("\n\t\t\t\tP2P Chat\n\n");
    
    id = shmget(111, 1024, IPC_CREAT|00666);    //Opening a shared memory access
    buffer = shmat(id, NULL, 0);                //Attaching a buffer to it
    
    printf("\n\n\t\tThe Chat Connection has been Opened!.\n\t\t\tEnter \"Bye\" to Quit.\n");
    
    strcpy(msg, "`");       //Clearing the strings with a preset ` value
    strcpy(buffer, "`");
    
    while(1){
    	strcpy(msg, "`");                   //Clearing the msg buffer with ` again for next time
        while(buffer[0] == '`');            //Waiting, if the buffer is empty.
        strcpy(temp2, buffer);
        char *sep = strtok(temp2, "~");     //Splitting the string at the preset value ~ for the other client
        
        printf("\nPeer:\n\t%s\n", sep);

        if(strcmp(sep, "Bye\n") == 0){      //Ending the chat if "Bye" is entered by the other user.
            break;
        }

        else{
            printf("You:\n\t");
            fgets(temp1, 100, stdin);
            strcat(msg, temp1);             //Putting the scanned value into buffer
            strcpy(buffer, msg);            //Now buffer is like `<msg>
            strcat(msg, "`");               //Now msg is like `<msg>`

            if(strcmp(temp1, "Bye\n") == 0){    //Exiting the chat this user enters "Bye"
                break;
            }
        }
        
    }

    printf("\n\n\t\tThe Chat Connection has been Closed.\n");
    shmdt(buffer);
    shmctl(id, IPC_RMID, NULL);     //Deleting the shared memory addressing
    sleep(0);
    exit(0);

}


/*

OUTPUT:

(base) vishakan@Legion:~/Desktop/Operating-Systems/Ex4 InterProcess Communication$ gcc 3-ChatPeer2.c -o c2
(base) vishakan@Legion:~/Desktop/Operating-Systems/Ex4 InterProcess Communication$ ./c2

		        P2P Chat



		The Chat Connection has been Opened!.
			Enter "Bye" to Quit.

Peer:
	Hey

You:
	Hello

Peer:
	How's it going?

You:
	It's going good!

Peer:
	Good to know :)

You:
	Bye


		The Chat Connection has been Closed.
*/