#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{ //structure for file
    char name[50];
    int start_addr;
} file;

file *newFile(char name[], int addr);
int searchFile(file *root[], char name[], int file_count);
int insertFile(file *root[], int file_count);
void displayFiles(file *root[], int file_count);

int main(void)
{
    int opt = 1, file_count = 0;

    file *root[50];

    printf("\n\t\t\tSingle Level Directory Structure\n");
    while (opt != 0)
    {
        printf("\n\t\tMain Menu\n\t1. Insert a New File\n\t2. Display Existing Files\n\t0. Exit the Program\n\tYour choice -> ");
        scanf("%d", &opt);
        switch (opt)
        {
        case 1:
            if (file_count < 50)
            {
                file_count += insertFile(root, file_count);
                printf("\nCan accomodate %d more files in this directory structure.\n", (50 - file_count));
            }
            break;

        case 2:
            displayFiles(root, file_count);
            break;

        case 0:
            printf("\n\t\tThank You!\n");
            break;

        default:
            printf("\n\tInvalid Option.\n");
            break;
        }
    }

    return 0;
}

file *newFile(char name[], int addr)
{
    file *new_file = (file *)malloc(sizeof(file));
    strcpy(new_file->name, name);
    new_file->start_addr = addr;

    return new_file;
}

int searchFile(file *root[], char name[], int file_count)
{
    int flag = 0, i = 0;

    for (i = 0; i < file_count; i++)
    {
        if (root[i] != NULL)
        {
            if (strcmp(root[i]->name, name) == 0)
            {
                flag = 1;
                break;
            }
        }
    }

    return flag;
}

int insertFile(file *root[], int file_count)
{
    char name[50];
    int flag = 0, addr = 0;

    printf("\nEnter the File Name: ");
    scanf("%s", name);
    printf("\nEnter the Starting Address of File %s: ", name);
    scanf("%d", &addr);

    if (searchFile(root, name, file_count) == 1)
    {
        printf("\nFile %s already exists!\n", name);
        flag = 0;
    }

    else
    {
        root[file_count] = newFile(name, addr);
        printf("\nCreated File %s.\n", name);
        flag = 1;
    }

    return flag;
}

void displayFiles(file *root[], int file_count)
{
    printf("\nContents of Root Directory: \n");

    if (file_count == 0)
    {
        printf("\nRoot directory is empty.\n");
    }

    else
    {
        int i = 0;

        printf("\n-------------------------------------------------");
        printf("\n|\tFile\t|\tStarting Address\t|");
        printf("\n-------------------------------------------------\n");

        for (i = 0; i < file_count; i++)
        {
            if (root[i] != NULL)
            {
                printf("|\t%s\t|\t\t%d\t\t|\n", root[i]->name, root[i]->start_addr);
            }
        }
        printf("-------------------------------------------------\n");
    }
}

/*

OUTPUT:

(base) vishakan@Legion:~/Desktop/Operating-Systems/Ex 12 File Organization Techniques$ gcc SingleLevel.c -o s
(base) vishakan@Legion:~/Desktop/Operating-Systems/Ex 12 File Organization Techniques$ ./s

                        Single Level Directory Structure

                Main Menu
        1. Insert a New File
        2. Display Existing Files
        0. Exit the Program
        Your choice -> 1

Enter the File Name: OS

Enter the Starting Address of File OS: 1344

Created File OS.

Can accomodate 49 more files in this directory structure.

                Main Menu
        1. Insert a New File
        2. Display Existing Files
        0. Exit the Program
        Your choice -> 1

Enter the File Name: DBMS

Enter the Starting Address of File DBMS: 1750

Created File DBMS.

Can accomodate 48 more files in this directory structure.

                Main Menu
        1. Insert a New File
        2. Display Existing Files
        0. Exit the Program
        Your choice -> 1

Enter the File Name: DAA   

Enter the Starting Address of File DAA: 2190

Created File DAA.

Can accomodate 47 more files in this directory structure.

                Main Menu
        1. Insert a New File
        2. Display Existing Files
        0. Exit the Program
        Your choice -> 2

Contents of Root Directory: 

-------------------------------------------------
|       File    |       Starting Address        |
-------------------------------------------------
|       OS      |               1344            |
|       DBMS    |               1750            |
|       DAA     |               2190            |
-------------------------------------------------

                Main Menu
        1. Insert a New File
        2. Display Existing Files
        0. Exit the Program
        Your choice -> 0

                Thank You!

*/
