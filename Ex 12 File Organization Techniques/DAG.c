#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{ //struct for file
    char name[50];
} file;

typedef struct Dir
{ //struct for tree-like directory
    char name[50];
    struct Dir *dir1, *dir2, *dir3;
    file *file1, *file2;
} dir;

dir *root = NULL; //root directory

dir *initRoot();
void insertDirectory(char path[]);
void insertFile(char path[]);
void displayContents(dir *root, char path[]);
file *getFilePointer(char path[]);
dir *getDirectoryPointer(char path[]);
void createLink(char path[], char dir_name[]);

int main(void)
{
    root = initRoot();
    int opt = 1;
    char dir_name[50], name[50], path[500], path_name[50];

    printf("\n\t\t\tTwo Level Directory Structure\n");

    while (opt != 0)
    {
        printf("\n\t\tMain Menu\n\t1. Create a New File\n\t2. Create a New Directory\n\t3. Create a Link to a File\n\t4. Display Existing Files\n\t0. Exit the Program\n\tYour choice -> ");
        scanf("%d", &opt);
        switch (opt)
        {
        case 1:
            printf("\nEnter \"root\" to create a file in the root directory.\nEnter \"root/directory\" to create a file in the sub-directory.");
            printf("\n\nEnter the Path: ");
            scanf("%s", dir_name);
            insertFile(dir_name);
            break;

        case 2:
            printf("\nEnter \"root\" to create a file in the root directory.\nEnter \"root/directory\" to create a file in the sub-directory.");
            printf("\n\nEnter the Path: ");
            scanf("%s", dir_name);
            insertDirectory(dir_name);
            break;

        case 3:
            printf("\nEnter the Path of File (Including File Name): ");
            scanf("%s", path_name);
            printf("\nEnter the Path of Directory to Create Link in: ");
            scanf("%s", dir_name);
            createLink(path_name, dir_name);
            break;

        case 4:
            strcpy(path, "");
            printf("\nFile\t\t\t\tPath\n\n");
            displayContents(root, path);
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

dir *initRoot()
{ //initialising root directory
    root = (dir *)malloc(sizeof(dir));

    strcpy(root->name, "root");
    root->dir1 = NULL;
    root->dir2 = NULL;
    root->dir3 = NULL;
    root->file1 = NULL;
    root->file2 = NULL;

    return root;
}

void insertDirectory(char path[])
{ //inserting a new directory to specified path
    dir *temp = root;
    char *dir_name = strtok(path, "/");
    dir_name = strtok(NULL, "/");

    while (dir_name != NULL)
    { //moving to the specified sub-directory
        if (temp->dir1 != NULL && strcmp(dir_name, temp->dir1->name) == 0)
        {
            temp = temp->dir1;
        }
        else if (temp->dir2 != NULL && strcmp(dir_name, temp->dir2->name) == 0)
        {
            temp = temp->dir2;
        }
        else if (temp->dir3 != NULL && strcmp(dir_name, temp->dir3->name) == 0)
        {
            temp = temp->dir3;
        }

        dir_name = strtok(NULL, "/");
    }

    if (dir_name == NULL)
    {
        if (temp->dir1 == NULL || temp->dir2 == NULL || temp->dir3 == NULL)
        { //if space exists in the specified sub-directory
            char dirname[50];
            printf("\nEnter the Directory Name: ");
            scanf("%s", dirname);

            dir *new_dir = (dir *)malloc(sizeof(dir));
            new_dir->dir1 = NULL;
            new_dir->dir2 = NULL;
            new_dir->dir3 = NULL;
            new_dir->file1 = NULL;
            new_dir->file2 = NULL;
            strcpy(new_dir->name, dirname);
            //connecting it to a free pointer of the parent directory
            if (temp->dir1 == NULL)
            {
                temp->dir1 = new_dir;
            }
            else if (temp->dir2 == NULL && strcmp(dirname, temp->dir1->name) != 0)
            {
                temp->dir2 = new_dir;
            }
            else if (strcmp(dirname, temp->dir1->name) != 0 && strcmp(dirname, temp->dir2->name) != 0)
            {
                temp->dir3 = new_dir;
            }
            else if (strcmp(dirname, temp->dir1->name) == 0 || strcmp(dirname, temp->dir2->name) == 0)
            { //if it already exists
                printf("\nDirectory %s already exists!\n", dirname);
            }
            else
            { //if no space is free in the sub-directory
                printf("\nDirectory Limit Exceeded.(Only 3 directories allowed under any directory)\n");
            }
        }
    }
}

void insertFile(char path[])
{ //inserting a new file to specified path
    dir *temp = root;
    char *split = strtok(path, "/");
    split = strtok(NULL, "/");

    while (split != NULL)
    { //moving to specified sub-directory
        if (temp->dir1 != NULL && strcmp(split, temp->dir1->name) == 0)
        {
            temp = temp->dir1;
        }
        else if (temp->dir2 != NULL && strcmp(split, temp->dir2->name) == 0)
        {
            temp = temp->dir2;
        }
        else if (temp->dir3 != NULL && strcmp(split, temp->dir3->name) == 0)
        {
            temp = temp->dir3;
        }
        split = strtok(NULL, "/");
    }

    if (split == NULL)
    {
        if (temp->file1 == NULL || temp->file2 == NULL)
        { //if the sub-directory has space for files
            char file_name[50];
            printf("\nEnter the File Name: ");
            scanf("%s", file_name);

            file *new_file = (file *)malloc(sizeof(file));
            strcpy(new_file->name, file_name);

            if (temp->file1 == NULL)
            {
                temp->file1 = new_file;
            }
            else if (temp->file2 == NULL)
            {
                temp->file2 = new_file;
            }
        }
        else
        { //if it doesn't have space for files
            printf("\nFile Limit Exceeded.(Only 2 Files allowed in any directory)\n");
        }
    }
}

void displayContents(dir *root, char path[])
{ //to display the contents of the a directory
    char temp[50];

    if (root != NULL)
    {
        strcat(path, root->name);
        strcat(path, "/");
        if (root->file1 != NULL)
        {
            printf("%s\t\t\t%s\n", root->file1->name, path);
        }
        if (root->file2 != NULL)
        {
            printf("%s\t\t\t%s\n", root->file2->name, path);
        }
        if (root->dir1 != NULL)
        {
            strcpy(temp, path);
            displayContents(root->dir1, temp);
        }
        if (root->dir2 != NULL)
        {
            strcpy(temp, path);
            displayContents(root->dir2, temp);
        }
        if (root->dir3 != NULL)
        {
            strcpy(temp, path);
            displayContents(root->dir3, temp);
        }
    }
}

file *getFilePointer(char path[])
{ //to return the file pointer to the specified file
    dir *temp = root;
    char *split = strtok(path, "/");
    char *t;

    while (split != NULL)
    { //traversing to the specified sub-directory
        if (temp->dir1 != NULL && strcmp(split, temp->dir1->name) == 0)
        {
            temp = temp->dir1;
        }
        else if (temp->dir2 != NULL && strcmp(split, temp->dir2->name) == 0)
        {
            temp = temp->dir2;
        }
        else if (temp->dir3 != NULL && strcmp(split, temp->dir3->name) == 0)
        {
            temp = temp->dir3;
        }
        t = split;
        split = strtok(NULL, "/");
        if (split == NULL)
        { //reached the parent directory of the file
            if (strcmp(temp->file1->name, t) == 0)
            {
                return temp->file1;
            }
            else if (strcmp(temp->file2->name, t) == 0)
            {
                return temp->file2;
            }
            else
            {
                printf("\nThe specified file does not exist.\n");
                return NULL;
            }
        }
    }
    return NULL;
}

dir *getDirectoryPointer(char path[])
{ //to return the directory pointer to the specified directory
    char *split = strtok(path, "/");
    dir *temp = root;

    while (split != NULL)
    { //traversing to the specified sub-directory
        if (temp->dir1 != NULL && strcmp(split, temp->dir1->name) == 0)
        {
            temp = temp->dir1;
        }
        else if (temp->dir2 != NULL && strcmp(split, temp->dir2->name) == 0)
        {
            temp = temp->dir2;
        }
        else if (temp->dir3 != NULL && strcmp(split, temp->dir3->name) == 0)
        {
            temp = temp->dir3;
        }

        split = strtok(NULL, "/");

        if (split == NULL)
        { //reached the required directory
            return temp;
        }
    }
    return NULL;
}

void createLink(char path[], char dir_name[])
{ //creating a link to existing file to another directory
    file *temp_file = getFilePointer(path);
    dir *temp_dir = getDirectoryPointer(dir_name);

    if (temp_file != NULL)
    {
        if (temp_dir->file1 == NULL)
        {
            temp_dir->file1 = temp_file;
        }
        else if (temp_dir->file2 == NULL)
        {
            temp_dir->file2 = temp_file;
        }
        else
        {
            printf("\nThe destination directory is full. Link cannot be created.\n");
        }
    }
}

/*
OUTPUT:
(base) vishakan@Legion:~/Desktop/Operating-Systems/Ex 12 File Organization Techniques$ gcc DAGraph.c -o d(base) vishakan@Legion:~/Desktop/Operating-Systems/Ex 12 File Organization Techniques$ ./d
                        Two Level Directory Structure
                Main Menu
        1. Create a New File
        2. Create a New Directory
        3. Create a Link to a File
        4. Display Existing Files
        0. Exit the Program
        Your choice -> 1
Enter "root" to create a file in the root directory.
Enter "root/directory" to create a file in the sub-directory.
Enter the Path: root
Enter the File Name: DAG.c
                Main Menu
        1. Create a New File
        2. Create a New Directory
        3. Create a Link to a File
        4. Display Existing Files
        0. Exit the Program
        Your choice -> 2
Enter "root" to create a file in the root directory.
Enter "root/directory" to create a file in the sub-directory.
Enter the Path: root
Enter the Directory Name: OS
                Main Menu
        1. Create a New File
        2. Create a New Directory
        3. Create a Link to a File
        4. Display Existing Files
        0. Exit the Program
        Your choice -> 3
Enter the Path of File (Including File Name): root/DAG.c
Enter the Path of Directory to Create Link in: root/OS
                Main Menu
        1. Create a New File
        2. Create a New Directory
        3. Create a Link to a File
        4. Display Existing Files
        0. Exit the Program
        Your choice -> 4
File                            Path
DAG.c                   root/
DAG.c                   root/OS/
                Main Menu
        1. Create a New File
        2. Create a New Directory
        3. Create a Link to a File
        4. Display Existing Files
        0. Exit the Program
        Your choice -> 0
                Thank You!
*/