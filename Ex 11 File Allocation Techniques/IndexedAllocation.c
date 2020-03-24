#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{ //struct for maintaining a file
    char name[50];
    int size;
    int blocks;
    int index;
} file;

typedef struct
{ //struct to maintain indexing
    int block_id;
    int blocks[100];
} index_block;

index_block indexer[100];
file files[100];

int free_blocks[100], mem_size;
int block_size, no_files, no_blocks, no_free;

void initMemory();
void printBlocks(int ind);
void allocateFiles();
void printFreeBlocks();
void printDirectory();
void printIndexBlock();

int main(void)
{
    int opt = 1;

    printf("\n\t\tIndexed Allocation\n");
    while (opt != 0)
    {
        printf("\n\n\t\tMain Menu\n");
        printf("\n\t1. Initialise Memory\n\t2. Print Free Blocks\n\t3. Allocate Files\n\t4. Print Directory\n\t5. Print Index Blocks\n\t0. Exit\n\tYour Choice -> ");
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
        case 5:
            printIndexBlock();
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

void printBlocks(int ind)
{ //printing block information
    int j = 0;

    printf("\n\tIndex Block No. : %d\n\tData Blocks : ", indexer[ind].block_id);
    for (j = 0; j < files[ind].blocks; j++)
    {
        printf("%d ", indexer[ind].blocks[j]);
    }
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

void initMemory()
{ //initialising memory
    int i = 0, rand_block = 0;

    printf("\nEnter the size of memory in KB: ");
    scanf("%d", &mem_size);
    printf("\nEnter the size of a block in KB: ");
    scanf("%d", &block_size);

    no_blocks = mem_size / block_size;
    no_free = no_blocks;
    printf("\nNo. of blocks : %d", no_blocks);

    /*for (i = 0; i <= no_blocks / 3; i++)
    {
        rand_block = random() % no_blocks;
        if (free_blocks[rand_block] == 1)
        {
            i--;
        }
        else
        {
            free_blocks[rand_block] = 1;
            no_free--;
        }
    }*/
}

void allocateFiles()
{ //allocating files to blocks
    int i, rand_block, count = 0, j = 0;

    if (no_free == 0)
    {
        printf("\nMemory space exhausted!\n");
        return;
    }

    printf("\nEnter the no. of files: ");
    scanf("%d", &no_files);

    for (i = 0; i < no_files; i++)
    {
        printf("\nEnter the Name of File %d: ", i + 1);
        scanf("%s", files[count].name);
        printf("\nEnter Size of File %d in KB: ", i + 1);
        scanf("%d", &files[count].size);

        files[count].blocks = files[count].size / block_size;
        if (files[count].size * 1.0 / block_size > files[count].blocks)
        {
            files[count].blocks++;
        } //finding appropriate no. of blocks for the file

        if (files[count].blocks + 1 > no_free)
        {
            printf("\nCannot allocate file %d\n", i + 1);
            i--;
        }
        else
        {
            do
            {
                rand_block = random() % no_blocks;
            } while (free_blocks[rand_block] == 1);

            indexer[count].block_id = rand_block; //choosing the index block
            files[count].index = rand_block;
            free_blocks[rand_block] = 1;
            no_free--;

            for (j = 0; j < files[count].blocks; j++)
            { //allocating the file's blocks
                rand_block = random() % no_blocks;
                if (free_blocks[rand_block] == 0)
                {
                    free_blocks[rand_block] = 1;
                    no_free--;
                    indexer[count].blocks[j] = rand_block;
                }
                else
                {
                    j--;
                }
            }
            count++;
        }

        if (no_free == 0)
        {
            printf("\nMemory space exhausted!\n");
            no_files = count;
            break;
        }
    }
}

void printDirectory()
{ //printing out directory info
    int i = 0;

    printf("\nFile Allocation: \n");
    for (i = 0; i < no_files; i++)
    {
        printf("\nFile: %s\n", files[i].name);
        printBlocks(i);
        printf("\n");
    }
}

void printIndexBlock()
{ //printing out index blocks
    int i = 0;
    printf("\n\tFile Indexing\n");
    printf("\n---------------------------------");
    printf("\n|\tFile\t|\tIndex\t|");
    printf("\n---------------------------------");
    for (i = 0; i < no_files; i++)
    {
        printf("\n|\t%s\t|\t%d\t|", files[i].name, files[i].index);
    }
    printf("\n---------------------------------\n");
}


/*
OUTPUT:

(base) vishakan@Legion:~/Desktop/Operating-Systems/Ex 11 File Allocation Techniques$ gcc IndexedAllocation.c -o i
(base) vishakan@Legion:~/Desktop/Operating-Systems/Ex 11 File Allocation Techniques$ ./i

                Indexed Allocation


                Main Menu

        1. Initialise Memory
        2. Print Free Blocks
        3. Allocate Files
        4. Print Directory
        5. Print Index Blocks
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
        5. Print Index Blocks
        0. Exit
        Your Choice -> 2

Free Blocks: 0 1 2 3 4 5 6 7 8 9 
Free Space: 20 KB

                Main Menu

        1. Initialise Memory
        2. Print Free Blocks
        3. Allocate Files
        4. Print Directory
        5. Print Index Blocks
        0. Exit
        Your Choice -> 3

Enter the no. of files: 3

Enter the Name of File 1: A      

Enter Size of File 1 in KB: 10

Enter the Name of File 2: B

Enter Size of File 2 in KB: 2

Enter the Name of File 3: C

Enter Size of File 3 in KB: 4

Cannot allocate file 3
Enter the Name of File 3: C

Enter Size of File 3 in KB: 3

Cannot allocate file 3
Enter the Name of File 3: C

Enter Size of File 3 in KB: 2

Memory space exhausted!


                Main Menu

        1. Initialise Memory
        2. Print Free Blocks
        3. Allocate Files
        4. Print Directory
        5. Print Index Blocks
        0. Exit
        Your Choice -> 2

Free Blocks: 
Free Space: 0 KB

                Main Menu

        1. Initialise Memory
        2. Print Free Blocks
        3. Allocate Files
        4. Print Directory
        5. Print Index Blocks
        0. Exit
        Your Choice -> 4

File Allocation: 

File: A

        Index Block No. : 3
        Data Blocks : 6 7 5 2 9 

File: B

        Index Block No. : 1
        Data Blocks : 0 

File: C

        Index Block No. : 8
        Data Blocks : 4 


                Main Menu

        1. Initialise Memory
        2. Print Free Blocks
        3. Allocate Files
        4. Print Directory
        5. Print Index Blocks
        0. Exit
        Your Choice -> 5

        File Indexing

---------------------------------
|       File    |       Index   |
---------------------------------
|       A       |       3       |
|       B       |       1       |
|       C       |       8       |
---------------------------------


                Main Menu

        1. Initialise Memory
        2. Print Free Blocks
        3. Allocate Files
        4. Print Directory
        5. Print Index Blocks
        0. Exit
        Your Choice -> 0    

        Thank You!

*/