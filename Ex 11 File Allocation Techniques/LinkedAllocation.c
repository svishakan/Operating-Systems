#include <stdio.h>
#include <stdlib.h>

struct List
{ //structure for maintaining linked list of blocks
    int block;
    struct List *next;
};

typedef struct List list;

typedef struct
{ //structure for maintaining file details
    char name[50];
    int size;
    int blocks;
    list *head;
} file;

int free_blocks[100], mem_size;
int no_free, no_blocks, no_files, block_size;

file files[100];

void initMemory();
list *makeNode(int block);
void printFreeBlocks();
void allocateFiles();
void printDirectory();
void printList(list *head);

int main(void)
{
    int opt = 1;

    printf("\n\t\tLinked Allocation\n");
    while (opt != 0)
    {
        printf("\n\n\t\tMain Menu\n");
        printf("\n\t1. Initialise Memory\n\t2. Print Free Blocks\n\t3. Allocate Files\n\t4. Print Directory\n\t0. Exit\n\tYour Choice -> ");
        scanf("%d", &opt);
        switch (opt)
        {
        case 1:
            initMemory();
            break;
        case 2:
            printFreeBlocks();
            break;
        case 3:
            allocateFiles();
            break;
        case 4:
            printDirectory();
            break;
        case 0:
            printf("\n\tThank You!\n");
            break;
        default:
            printf("\nInvalid Option!");
            break;
        }
    }

    return 0;
}

list *makeNode(int block)
{ //making a new node for a block
    list *new_node = (list *)malloc(sizeof(list));
    new_node->block = block;
    new_node->next = NULL;
    return new_node;
}

void printList(list *head)
{ //printing list of blocks for a file
    list *temp;

    printf("\nBlock List: ");
    for (temp = head; temp != NULL; temp = temp->next)
    {
        printf("%d", temp->block);
        if (temp->next != NULL)
        {
            printf(" -> ");
        }
    }
    printf("\n");
}

void printFreeBlocks()
{ //printing free blocks
    int i = 0;

    printf("\nFree Blocks: ");
    for (i = 0; i < no_blocks; i++)
    {
        if (free_blocks[i] == 0)
        {
            printf("%d ", i);
        }
    }
    printf("\nFree Space: %d KB", no_free * block_size);
}

void printDirectory()
{ //printing the directory info
    int i = 0;

    printf("\nDirectory Structure: \n");
    for (i = 0; i < no_files; i++)
    {
        printf("\nFile : %s", files[i].name);
        printList(files[i].head);
        printf("\n");
    }
}

void initMemory()
{ //initialising memory space
    int i = 0, rand_block = 0;

    printf("\nEnter the size of memory in KB: ");
    scanf("%d", &mem_size);
    printf("\nEnter the size of a block in KB: ");
    scanf("%d", &block_size);

    no_blocks = mem_size / block_size;
    no_free = no_blocks;
    printf("\nNo. of blocks : %d", no_blocks);
}

void allocateFiles()
{ //allocating space for files
    int i = 0, j = 0, rand_block = 0, count = 0;

    if (no_free == 0)
    {
        printf("\nMemory space exhausted!\n");
        return;
    }

    printf("\nEnter the no. of Files: ");
    scanf("%d", &no_files);

    for (i = 0; i < no_files; i++)
    {
        printf("\nEnter the File Name: ");
        scanf("%s", files[count].name);
        printf("\nEnter the File Size in KB: ");
        scanf("%d", &files[count].size);

        files[count].blocks = files[count].size / block_size;

        if (files[count].size * 1.0 / block_size > files[count].blocks)
        {
            files[count].blocks++;
        }

        if (files[count].blocks > no_blocks)
        { //unavailability of blocks
            printf("\nCannot allocate this file due to insufficient memory.\n");
            i--;
        }

        else
        { //making a list of blocks for the file
            list *temp, *prev;
            for (j = 0; j < files[count].blocks; j++)
            {
                rand_block = random() % no_blocks;
                if (free_blocks[rand_block] == 0)
                {
                    free_blocks[rand_block] = 1;
                    no_free--;
                    temp = makeNode(rand_block);
                    if (j == 0)
                    { //init. the header node
                        files[count].head = temp;
                        prev = files[count].head;
                    }
                    else
                    { //enqueue the other nodes
                        prev->next = temp;
                        prev = temp;
                    }
                }
                else
                {
                    j--;
                }
            }
            count++;
        }

        if (no_free == 0)
        { //if memory space is exhausted
            printf("\nMemory space exhausted!\n");
            no_files = count;
            break;
        }
    }
}

/*
OUTPUT:

(base) vishakan@Legion:~/Desktop/Operating-Systems/Ex 11 File Allocation Techniques$ gcc LinkedAllocation.c -o l
(base) vishakan@Legion:~/Desktop/Operating-Systems/Ex 11 File Allocation Techniques$ ./l

                Linked Allocation


                Main Menu

        1. Initialise Memory
        2. Print Free Blocks
        3. Allocate Files
        4. Print Directory
        0. Exit
        Your Choice -> 1

Enter the size of memory in KB: 20

Enter the size of a block in KB: 2

No. of blocks : 10

                Main Menu

        1. Initialise Memory
        2. Print Free Blocks
        3. Allocate Files
        4. Print Directory
        0. Exit
        Your Choice -> 2

Free Blocks: 0 1 2 3 4 5 6 7 8 9 
Free Space: 20 KB

                Main Menu

        1. Initialise Memory
        2. Print Free Blocks
        3. Allocate Files
        4. Print Directory
        0. Exit
        Your Choice -> 3

Enter the no. of Files: 3

Enter the File Name: A

Enter the File Size in KB: 10

Enter the File Name: B

Enter the File Size in KB: 5

Enter the File Name: C

Enter the File Size in KB: 2


                Main Menu

        1. Initialise Memory
        2. Print Free Blocks
        3. Allocate Files
        4. Print Directory
        0. Exit
        Your Choice -> 4

Directory Structure: 

File : A
Block List: 3 -> 6 -> 7 -> 5 -> 2


File : B
Block List: 9 -> 1 -> 0


File : C
Block List: 8



                Main Menu

        1. Initialise Memory
        2. Print Free Blocks
        3. Allocate Files
        4. Print Directory
        0. Exit
        Your Choice -> 0

        Thank You!

*/