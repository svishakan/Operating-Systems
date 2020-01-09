#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

int copyfile(char *src, char *dest); //function to copy contents from one file to another

int main(int argc, char *argv[])
{

    if (argc < 3 || argc > 4)
    {
        printf("Require 3 or 4 arguments only.");
    }

    else if (argc == 3) //default copy
    {
        copyfile(argv[1], argv[2]);
    }

    else //interactive mode copy
    {
        if (strcmp(argv[1], "-i") != 0)
        {
            printf("\nInvalid Option!");
            return 0;
        }

        else
        {
            char c = 'N';
            printf("\n%s's contents are going to be copied to %s.\nEnter (y/Y) to COPY, (n/N) to EXIT\n", argv[2], argv[3]);
            scanf(" %c", &c);
            if (c != 'Y' && c != 'y')
                return 0;
            else
                copyfile(argv[2], argv[3]);
        }
    }
}

int copyfile(char *srcfile, char *destfile)
{
    int src, dest, count;
    char *buffer[1024];

    src = open(srcfile, O_RDONLY); //open src in read only mode

    if (src == -1)
    {
        printf("\nSource File Not Found!");
        close(src);
        return 0;
    }

    else
    {

        dest = open(destfile, O_WRONLY | O_CREAT, S_IRWXU); //open dest in write only mode if it exists, otherwise create it, with permissions RWX for the user
        while ((count = read(src, buffer, sizeof(buffer))) != 0)
            write(dest, buffer, count);
        printf("\nCopied Successfully!\n");
    }

    close(src);
    close(dest);
}

/*
OUTPUT:

vishakan@legion:~/Desktop/Semester IV/OS Lab/Ex1$ gcc mycp2.c -o mycp
vishakan@legion:~/Desktop/Semester IV/OS Lab/Ex1$ ./mycp sample.txt new.txt

Copied Successfully!

vishakan@legion:~/Desktop/Semester IV/OS Lab/Ex1$ cat sample.txt
The quick brown fox jumps over the lazy dog.

vishakan@legion:~/Desktop/Semester IV/OS Lab/Ex1$ cat new.txt
The quick brown fox jumps over the lazy dog.
*/
