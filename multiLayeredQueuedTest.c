#include "types.h"
#include "stat.h"
#include "user.h"

#define NUM_CHILDREN 60

int main(int argc, char *argv[])
{

    changePolicy(3);
    
    int pid = getpid();
    int child_number = -1;

    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        if (fork() == 0) 
        {
            setQueue((i / 10) + 1);
            child_number = i + 1;
            break;
        }
    }

    if (getpid() != pid)
    {
        for (int i = 1; i <= 200; i++)
            printf(1, "/%d/: /%d/\n", child_number, i);
    }

    else
    {
        int queues[NUM_CHILDREN] = {0};      // priorites
        int turnarounds[NUM_CHILDREN] = {0}; 
        int waitings[NUM_CHILDREN] = {0};    
        int CBTs[NUM_CHILDREN] = {0};        
        int *procTimes = malloc(4 * sizeof(int));
        int i = 0;
        while (customWait(procTimes) > 0)
        {
            int childQueue = procTimes[3];
            int childTurnaround = procTimes[0];
            int childWaiting = procTimes[1];
            int childCBT = procTimes[2];

            queues[i] = childQueue;
            turnarounds[i] = childTurnaround;
            waitings[i] = childWaiting;
            CBTs[i] = childCBT;
            i++;
        }
        for (int j = 0; j < NUM_CHILDREN; j++)
        {
            printf(1, "Child with priority %d :\n Turnaround time: %d, Waiting time: %d, CBT time: %d\n",
                   queues[j], turnarounds[j], waitings[j], CBTs[j]);
        }

        int turnaroundsPerQueue[6] = {0};
        int waitingsPerQueue[6] = {0};
        int CBTsPerQueue[6] = {0};
        for (int j = 0; j < NUM_CHILDREN; j++)
        {
            int childQueue = queues[j];
            turnaroundsPerQueue[childQueue - 1] += turnarounds[j];
            waitingsPerQueue[childQueue - 1] += waitings[j];
            CBTsPerQueue[childQueue - 1] += CBTs[j];
        }
        for (int j = 0; j < 6; j++)
        {
            printf(1, "Queue: %d :\n avarage Turnaround: %d, avarage Waiting: %d, avarage CBT: %d\n",
                   j + 1,
                   turnaroundsPerQueue[j] / (NUM_CHILDREN / 6),
                   waitingsPerQueue[j] / (NUM_CHILDREN / 6),
                   CBTsPerQueue[j] / (NUM_CHILDREN / 6));
        }

        int turnaroundsSum = 0;
        int waitingsSum = 0;
        int CBTsSum = 0;
        for (int j = 0; j < NUM_CHILDREN; j++)
        {
            turnaroundsSum += turnarounds[j];
            waitingsSum += waitings[j];
            CBTsSum += CBTs[j];
        }
        printf(1, "Total :\n avarage Turnaround: %d, avarage Waiting: %d, avarage CBT: %d\n",
               turnaroundsSum / NUM_CHILDREN,
               waitingsSum / NUM_CHILDREN,
               CBTsSum / NUM_CHILDREN);
    }

    while (wait() != -1)
        ;

    exit();
}