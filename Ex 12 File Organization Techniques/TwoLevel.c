#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{ //file structure
    char name[50];
} file;

typedef struct
{ //directory structure with capacity of 5 files
    char name[50];
    int capacity;
    file *list[5];
} directory;

typedef struct
{   //general structure to represent file/directory
    int type;   //type -> 0 for FILE, type -> 1 for DIRECTORY
    void *pointer;
} unit;

unit root[50];  //array to hold 50 units of files/directories
int count = 0;

file *newFile(char name[]);
directory *newDirectory(char name[]);
int searchFile(char name[]);
directory *searchDirectory(char name[]);
void insertUnit(char name[], int type);
void insertFiletoDir(directory *dir, char name[]);
void displayContents(unit root[]);

int main(void)
{
    int opt = 1, type = 0;
    char name[50], dir_name[50];

    printf("\n\t\t\tTwo Level Directory Structure\n");
    while (opt != 0)
    {
        printf("\n\t\tMain Menu\n\t1. Create a New File\n\t2. Create a New Directory\n\t3. Display Existing Files\n\t0. Exit the Program\n\tYour choice -> ");
        scanf("%d", &opt);
        switch (opt)
        {
        case 1:
            printf("\nEnter \"root\" to create a file in the root directory.\nEnter \"root/directory\" to create a file in the sub-directory.");
            printf("\nEnter the Directory Name: ");
            scanf("%s", dir_name);
            printf("\nEnter the File Name: ");
            scanf("%s", name);
            if (strcmp(dir_name, "root") != 0)
            {   //if user enters a sub-directory
                char *sub_dir = strtok(dir_name, "/");  //split to find sub-dir from input
                sub_dir = strtok(NULL, "/");
                directory *dir = searchDirectory(sub_dir);
                if (dir != NULL)
                {   //inserting file to the sub-directory
                    insertFiletoDir(dir, name);
                }
                else
                {   //invalid sub-directory entered by user
                    printf("\nDirectory %s does not exist.", dir_name);
                }
            }

            else if (strcmp(dir_name, "root") == 0)
            {   //if user enters root as directory
                insertUnit(name, 0);    //file
            }

            printf("\nCan accomodate %d more files in this directory structure.\n", (50 - count));
            break;

        case 2:
            printf("\nEnter the Directory Name: ");
            scanf("%s", dir_name);
            insertUnit(dir_name, 1);    //directory
            break;

        case 3:
            displayContents(root);
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

file *newFile(char name[])
{   //making a new file structure
    file *new_file = (file *)malloc(sizeof(file));
    strcpy(new_file->name, name);

    return new_file;
}

directory *newDirectory(char name[])
{   //making a new directory structure
    int i = 0;

    directory *new_dir = (directory *)malloc(sizeof(directory));
    strcpy(new_dir->name, name);
    new_dir->capacity = 0;

    for (i = 0; i < 5; i++)
    {   //initialise
        new_dir->list[i] = NULL;
    }

    return new_dir;
}

int searchFile(char name[])
{   //searching a file under root directory
    int flag = 0, i = 0;

    for (i = 0; i < count; i++)
    {
        if (root[i].pointer != NULL)
        {
            if (strcmp(((file *)(root[i].pointer))->name, name) == 0)
            {
                flag = 1;
                break;
            }
        }
    }

    return flag;
}

directory *searchDirectory(char name[])
{   //searching for a directory under root directory
    directory *flag_dir = NULL;
    int i = 0;

    for (i = 0; i < count; i++)
    {
        if (root[i].pointer != NULL)
        {
            if (strcmp(((directory *)(root[i].pointer))->name, name) == 0)
            {
                flag_dir = ((directory *)(root[i].pointer));
                break;
            }
        }
    }

    return flag_dir;    //pointer to desired directory
}

void insertUnit(char name[], int type)
{   //inserting a new file or directory under root
    if (count >= 50)
    {   //capacity reached
        printf("\nReached maximum capacity.\n");
        return;
    }

    if (type == 0 && searchFile(name) == 1)
    {   //if file and it already exists
        printf("\nFile %s already exists!\n", name);
        return;
    }

    if (type == 1 && searchDirectory(name) != NULL)
    {   //if directory and it already exists
        printf("\nDirectory %s already exists!\n", name);
        return;
    }

    if (type == 1 && searchFile(name) == 1)
    {   //if directory and already a file with the same name exists
        printf("\nFile named %s already exists!\n", name);
        return;
    }

    if (type == 0)
    {   //creating the file
        root[count].pointer = newFile(name);
        printf("\nCreated File %s.\n", name);
    }
    else
    {   //creating the directory
        root[count].pointer = newDirectory(name);
        printf("\nCreated Directory %s.\n", name);
    }

    root[count].type = type;
    count++;
}

void insertFiletoDir(directory *dir, char name[])
{   //inserting the file to directory
    int i, pos;
    if (dir->capacity >= 5)
    {
        printf("\nDirectory is Full!\n");
    }
    else
    {
        for (i = 0; i < 5; i++)
        {
            if (dir->list[i] != NULL)
            {   //moving through existing files
                if (strcmp(dir->list[i]->name, name) == 0)
                {   //if file already exists
                    printf("\nFile %s already exists!\n", name);
                    return;
                }
            }
            else
            {   //found the position, breaking
                pos = i;
                break;
            }
        }
        dir->list[pos] = newFile(name);     //inserting
        dir->capacity += 1;
        printf("\nCreated File %s in Directory %s.\n", name, dir->name);
    }
}

void displayContents(unit root[])
{   //display the entire contents under root directory
    printf("\nContents of Root Directory:\n");
    if (count == 0)
    {   //if empty
        printf("\nRoot directory is empty\n");
        return;
    }
    else
    {
        int dir_count = 0, file_count = 0, i = 0, j = 0;
        printf("\nFiles:\n");   //all files under root

        for (int i = 0; i < count; i++)
        {
            if (root[i].pointer != NULL)
            {
                if (root[i].type == 0)
                {
                    file_count++;
                    printf("%s ", ((file *)(root[i].pointer))->name);
                }
            }
        }

        if (file_count == 0)
        {
            printf("\t\t-NIL-");
        }

        printf("\n");
        file_count = 0;
        printf("\nDirectories:\n");     //all directories under root

        for (i = 0; i < count; i++)
        {
            if (root[i].pointer != NULL)
            {
                if (root[i].type == 1)
                {
                    dir_count++;
                    printf("%s ", ((directory *)(root[i].pointer))->name);
                }
            }
        }

        if (dir_count == 0)
        {
            printf("\t\t-NIL-");
        }

        printf("\n");
        dir_count = 0;

        for (i = 0; i < count; i++)     //contents of directories under root
        {
            if (root[i].pointer != NULL)
            {
                if (root[i].type == 1)
                {
                    dir_count++;
                    printf("\nContents of Directory %s:\n", ((directory *)(root[i].pointer))->name);
                    file_count = 0;
                    for (j = 0; j < 5; j++)
                    {   //traversing the directory list of files
                        if (((directory *)(root[i].pointer))->list[j] != NULL)
                        {
                            printf("%s ", ((directory *)(root[i].pointer))->list[j]->name);
                            file_count++;
                        }
                    }
                    if (file_count == 0)
                    {
                        printf("\t\t-NIL-");
                    }
                    printf("\n");
                }
            }
        }

        printf("\n");
    }
}

/*
OUTPUT:

(base) vishakan@Legion:~/Desktop/Operating-Systems/Ex 12 File Organization Techniques$ gcc TwoLevel.c -o t
(base) vishakan@Legion:~/Desktop/Operating-Systems/Ex 12 File Organization Techniques$ ./t

                        Two Level Directory Structure

                Main Menu
        1. Create a New File
        2. Create a New Directory
        3. Display Existing Files
        0. Exit the Program
        Your choice -> 2

Enter the Directory Name: OS   

Created Directory OS.

                Main Menu
        1. Create a New File
        2. Create a New Directory
        3. Display Existing Files
        0. Exit the Program
        Your choice -> 2

Enter the Directory Name: DBMS

Created Directory DBMS.

                Main Menu
        1. Create a New File
        2. Create a New Directory
        3. Display Existing Files
        0. Exit the Program
        Your choice -> 1

Enter "root" to create a file in the root directory.
Enter "root/directory" to create a file in the sub-directory.
Enter the Directory Name: root/OS

Enter the File Name: Semaphores

Created File Semaphores in Directory OS.

Can accomodate 48 more files in this directory structure.

                Main Menu
        1. Create a New File
        2. Create a New Directory
        3. Display Existing Files
        0. Exit the Program
        Your choice -> 1

Enter "root" to create a file in the root directory.
Enter "root/directory" to create a file in the sub-directory.
Enter the Directory Name: root/OS

Enter the File Name: Paging

Created File Paging in Directory OS.

Can accomodate 48 more files in this directory structure.

                Main Menu
        1. Create a New File
        2. Create a New Directory
        3. Display Existing Files
        0. Exit the Program
        Your choice -> 1

Enter "root" to create a file in the root directory.
Enter "root/directory" to create a file in the sub-directory.
Enter the Directory Name: root

Enter the File Name: N_Queens.py

Created File N_Queens.py.

Can accomodate 47 more files in this directory structure.

                Main Menu
        1. Create a New File
        2. Create a New Directory
        3. Display Existing Files
        0. Exit the Program
        Your choice -> 3

Contents of Root Directory:

Files:
N_Queens.py 

Directories:
OS DBMS 

Contents of Directory OS:
Semaphores Paging 

Contents of Directory DBMS:
                -NIL-


                Main Menu
        1. Create a New File
        2. Create a New Directory
        3. Display Existing Files
        0. Exit the Program
        Your choice -> 0

                Thank You!

*/