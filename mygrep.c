#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void grepcount(char *pattern, char *filename);  //grep -c
void linegrep(char *pattern, char *filename);   // grep -n
void invertgrep(char *pattern, char *filename); //grep -v
void normalgrep(char *pattern, char *filename); //grep

int main(int argc, char *argv[])
{
    if (argc < 3 || argc > 4)
    {
        printf("\nEnter only 3 or 4 arguments.");
        return 0;
    }

    if (argc == 3)
    {
        normalgrep(argv[1], argv[2]);
        return 0;
    }

    else
    {
        if (strcmp(argv[1], "-c") == 0)
            grepcount(argv[2], argv[3]);
        else if (strcmp(argv[1], "-n") == 0)
            linegrep(argv[2], argv[3]);
        else if (strcmp(argv[1], "-v") == 0)
            invertgrep(argv[2], argv[3]);
        else
        {
            printf("\nInvalid Option.\n");
            return 0;
        }
    }
}

void grepcount(char *pattern, char *filename)
{
    FILE *target;
    target = fopen(filename, "r");
    if (target == NULL)
    {
        printf("\nSpecified file could not be opened.");
        fclose(target);
        return;
    }

    char *buffer;
    int count = 0;
    size_t buf_size;

    while (getline(&buffer, &buf_size, target) != -1) //can also use fgets
    {
        if (strstr(buffer, pattern) != NULL)
            count++;
    }
    printf("The number of lines in which '%s' was found in file %s: %d\n", pattern, filename, count);
    fclose(target);
    return;
}

void linegrep(char *pattern, char *filename)
{
    FILE *target;
    target = fopen(filename, "r");
    if (target == NULL)
    {
        printf("\nSpecified file could not be opened.");
        fclose(target);
        return;
    }

    char *buffer;
    int linenumber = 0;
    size_t buf_size;

    while (getline(&buffer, &buf_size, target) != -1)
    {
        ++linenumber;
        if (strstr(buffer, pattern) != NULL)
            printf("%d : %s", linenumber, buffer);
    }

    fclose(target);
    return;
}

void invertgrep(char *pattern, char *filename)
{
    FILE *target;
    target = fopen(filename, "r");
    if (target == NULL)
    {
        printf("\nSpecified file could not be opened.");
        fclose(target);
        return;
    }

    char buffer[255];

    while (fgets(buffer, 255, target))
    {
        if (strstr(buffer, pattern) == NULL)
            printf("%s", buffer);
    }

    fclose(target);
    return;
}

void normalgrep(char *pattern, char *filename)
{
    FILE *target;
    target = fopen(filename, "r");
    if (target == NULL)
    {
        printf("\nSpecified file could not be opened.");
        fclose(target);
        return;
    }

    char buffer[255];

    while (fgets(buffer, 255, target))
    {
        if (strstr(buffer, pattern) != NULL)
            printf("%s", buffer);
    }

    fclose(target);
    return;
}

/*
OUTPUT:

vishakan@legion:~/Desktop/Semester IV/OS Lab/Ex1$ cat content.txt
Hello
This text file was written to check the working of the GREP command 
Grep Grep Grep
Invert!
System calls
calls calls Now callsaregreat.

vishakan@legion:~/Desktop/Semester IV/OS Lab/Ex1$ ./mygrep calls content.txt
System calls
calls calls Now callsaregreat.

vishakan@legion:~/Desktop/Semester IV/OS Lab/Ex1$ ./mygrep -v calls content.txt
Hello
This text file was written to check the working of the GREP command 
Grep Grep Grep
Invert!

vishakan@legion:~/Desktop/Semester IV/OS Lab/Ex1$ ./mygrep -c calls content.txt
The number of lines in which 'calls' was found in file content.txt: 2

vishakan@legion:~/Desktop/Semester IV/OS Lab/Ex1$ ./mygrep -n calls content.txt
5 : System calls
6 : calls calls Now callsaregreat.
*/