#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{ //structure to maintain memory
    int block;
    int not_free;
    char file[50];
} memory;

struct Element
{ //structure to maintain linked list
    int block;
    int size;
    char file[50];
    struct Element *next;
};

typedef struct Element element;

memory mem[100];
element *table = NULL;
int mem_size, block_size, num_blocks;

void initMemory();
void printDirectory();
int checkFreeSpace(int size);
int checkContiguous(int index, int size);
void insertFile(char file[], int size, int block);
void allocateFiles();
int allocateFile(char name[], int size);
void printFreeBlocks();

int main(void)
{
    int opt = 1;

    printf("\n\t\tContiguous Allocation\n");
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

void initMemory()
{ //initialising memory space
    int i = 0;

    printf("\nEnter size of memory in KB: ");
    scanf("%d", &mem_size);
    printf("\nEnter the size of a block in KB: ");
    scanf("%d", &block_size);
    num_blocks = mem_size / block_size;
    printf("\nNo. of Blocks : %d\n", num_blocks);

    for (i = 0; i < num_blocks; i++)
    {
        mem[i].block = i;
        mem[i].not_free = 0;
        strcpy(mem[i].file, "---");
    }
}

void printFreeBlocks()
{ //printing free blocks
    int i = 0;
    printf("\nFree Blocks: ");
    for (i = 0; i < num_blocks; i++)
    {
        if (mem[i].not_free == 0)
        {
            printf("%d ", i);
        }
    }
    printf("\n");
}

void printDirectory()
{ //displaying the file allocation details
    element *temp = table;

    printf("\n\n-------------------------------------------------");
    printf("\n|\tFile\t|\tBlock\t|\tSize\t|\n");
    printf("-------------------------------------------------\n");

    while (temp != NULL)
    {
        printf("|\t%s\t|\t%d\t|\t%d\t|\n", temp->file, temp->block, temp->size);
        temp = temp->next;
    }

    printf("-------------------------------------------------\n");
}

int checkFreeSpace(int size)
{ //checking for contiguous free space
    int i = 0, j = 0;

    for (i = 0; i < num_blocks;)
    {
        if (mem[i].not_free == 0)
        {
            int j = i;
            while (mem[j].not_free == 0 && j < num_blocks)
            {
                if (j - i + 1 == size)
                {
                    return i;
                }
                j++;
            }
            i += (j + 1);
        }
        else
        {
            i++;
        }
    }

    return -1; //no free space
}

int checkContiguous(int ind, int size)
{ //checking for contiguous blocks from a given index point
    int i = 0;

    if (mem[ind].not_free == 0)
    {
        for (i = ind; i < ind + size && i < num_blocks; i++)
        {
            if (mem[i].not_free == 1)
            {
                return 0;
            }
        }
    }
    else
    {
        return 0;
    }

    if (ind + size - 1 < num_blocks)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void insertFile(char file[], int size, int block)
{ //inserting a new file into the linked list
    element *new_node = (element *)malloc(sizeof(element));
    new_node->next = NULL;
    new_node->block = block;
    new_node->size = size;
    strcpy(new_node->file, file);

    if (table == NULL)
    {
        table = new_node;
    }
    else
    {
        new_node->next = table;
        table = new_node;
    }
}

int allocateFile(char name[], int size)
{ //allocating memory space for a new file
    int flag = 0, blocks = size / block_size;
    int index = 0, i = 0;

    if (size * 1.0 / block_size != blocks)
    {
        blocks++;
    }

    size = blocks;

    if (checkFreeSpace(size) >= 0)
    {
        while (1)
        {
            index = random() % (num_blocks);
            if (checkContiguous(index, size) == 1)
            {
                flag = 1;
                i = index;

                do
                {
                    mem[i].not_free = 1;
                    strcpy(mem[i].file, name);
                    i++;
                } while (i < blocks + index);
                insertFile(name, blocks, index);
                break;
            }
        }
    }

    else
    { //insufficient memory
        printf("\nNot enough memory to allocate File %s.", name);
    }

    return flag;
}

void allocateFiles()
{ //allocating files
    int i = 0;
    char file[50];
    int size, num_files;

    printf("\nEnter the no. of files to allocate: ");
    scanf("%d", &num_files);

    for (i = 0; i < num_files; i++)
    {
        printf("\nEnter the File Name: ");
        scanf("%s", file);
        printf("\nEnter the Size of the File: ");
        scanf("%d", &size);
        allocateFile(file, size);
    }
}


/*
OUPTUT:

(base) vishakan@Legion:~/Desktop/Operating-Systems/Ex 11 File Allocation Techniques$ gcc ContiguousAllocation.c -o c
(base) vishakan@Legion:~/Desktop/Operating-Systems/Ex 11 File Allocation Techniques$ ./c

                Contiguous Allocation


                Main Menu

        1. Initialise Memory
        2. Print Free Blocks
        3. Allocate Files
        4. Print Directory
        0. Exit
        Your Choice -> 1

Enter size of memory in KB: 20

Enter the size of a block in KB: 2

No. of Blocks : 10


                Main Menu

        1. Initialise Memory
        2. Print Free Blocks
        3. Allocate Files
        4. Print Directory
        0. Exit
        Your Choice -> 2

Free Blocks: 0 1 2 3 4 5 6 7 8 9 


                Main Menu

        1. Initialise Memory
        2. Print Free Blocks
        3. Allocate Files
        4. Print Directory
        0. Exit
        Your Choice -> 3

Enter the no. of files to allocate: 3

Enter the File Name: A

Enter the Size of the File: 4

Enter the File Name: B

Enter the Size of the File: 4

Enter the File Name: C

Enter the Size of the File: 4


                Main Menu

        1. Initialise Memory
        2. Print Free Blocks
        3. Allocate Files
        4. Print Directory
        0. Exit
        Your Choice -> 2

Free Blocks: 0 5 8 9 


                Main Menu

        1. Initialise Memory
        2. Print Free Blocks
        3. Allocate Files
        4. Print Directory
        0. Exit
        Your Choice -> 4


-------------------------------------------------
|       File    |       Block   |       Size    |
-------------------------------------------------
|       C       |       1       |       2       |
|       B       |       6       |       2       |
|       A       |       3       |       2       |
-------------------------------------------------


                Main Menu

        1. Initialise Memory
        2. Print Free Blocks
        3. Allocate Files
        4. Print Directory
        0. Exit
        Your Choice -> 0

        Thank You!
        
*/