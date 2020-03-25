#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *findMin(void *param);
void *findMax(void *param);
void *findAverage(void *param);

//common elements used by all threads
int avg, min, max, len, list[100];

int main(int argc, char *argv[])
{

    if (argc <= 1)
    { //we need a list of integer arguments to work with
        fprintf(stderr, "\n\tUsage Error: Enter a list of integer values.\n");
        return -1;
    }

    else
    {
        int i = 0;
        len = argc - 1; //since the "./<file>" is an argument

        for (i = 1; i < argc; i++)
        { //appending it to a global list
            list[i - 1] = atoi(argv[i]);
        }

        pthread_t min_thread, max_thread, avg_thread; //thread variables
        pthread_attr_t attr_min, attr_max, attr_avg;  //init. to default attributes

        pthread_attr_init(&attr_min); //initialise all threads
        pthread_attr_init(&attr_max);
        pthread_attr_init(&attr_avg);

        pthread_create(&min_thread, &attr_min, findMin, list); //creating the threads
        pthread_create(&max_thread, &attr_max, findMax, list);
        pthread_create(&avg_thread, &attr_avg, findAverage, list);

        pthread_join(min_thread, NULL); //waiting for termination of threads in sequence
        pthread_join(max_thread, NULL);
        pthread_join(avg_thread, NULL);

        printf("\n\tAverage is : %d\n", avg); //outputs
        printf("\n\tMinimum is : %d\n", min);
        printf("\n\tMaximum is : %d\n\n", max);

        return 0;
    }
}

void *findMin(void *param)
{ //to find minimum from list
    int i = 0;
    min = list[0];

    for (i = 1; i < len; i++)
    {
        if (min > list[i])
        {
            min = list[i];
        }
    }

    pthread_exit(0);
}

void *findMax(void *param)
{ //to find maximum from list
    int i = 0;
    max = list[0];

    for (i = 1; i < len; i++)
    {
        if (max < list[i])
        {
            max = list[i];
        }
    }

    pthread_exit(0);
}

void *findAverage(void *param)
{ //to find average of all elements in list
    int i = 0;
    avg = 0;

    for (i = 0; i < len; i++)
    {
        avg += list[i];
    }

    avg /= len;

    pthread_exit(0);
}

/*

OUTPUT:

(base) vishakan@Legion:~/Desktop/Operating-Systems/Ex 13 Threads$ ./stat 90 81 78 95 79 72 85

        Average is : 82

        Minimum is : 72

        Maximum is : 95

(base) vishakan@Legion:~/Desktop/Operating-Systems/Ex 13 Threads$ ./stat
        
        Usage Error: Enter a list of integer values.

*/