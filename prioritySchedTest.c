#include "types.h"
#include "stat.h"
#include "user.h"

#define NUM_CHILDREN 30

int main(int argc, char *argv[])
{
    changePolicy(2);
    int original_pid = getpid();
    int child_num = -1;
    int priority = -1;

    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        if (fork() == 0)
        {
            child_num = i + 1;
            priority = (i / 5) + 1;
            printf(1, "i: %d, priority: %d\n", i, priority);
            setPriority(priority);
            break;
        }
    }

    if (getpid() != original_pid)
    {
        for (int i = 1; i <= 50; i++)
            printf(1, "/%d/: /%d/\n", child_num, i);
    }

    else
    {
        int priorities[NUM_CHILDREN] = {0};  
        int turnarounds[NUM_CHILDREN] = {0}; 
        int waitings[NUM_CHILDREN] = {0};    
        int CBTs[NUM_CHILDREN] = {0};        

        int *procTimes = malloc(4 * sizeof(int));
        int i = 0;
        while (wait_(procTimes) > 0)
        {
            int childPriority = procTimes[3];
            int childTurnaround = procTimes[0];
            int childWaiting = procTimes[1];
            int childCBT = procTimes[2];

            priorities[i] = childPriority;
            turnarounds[i] = childTurnaround;
            waitings[i] = childWaiting;
            CBTs[i] = childCBT;
            i++;
        }

        for (int j = 0; j < NUM_CHILDREN; j++)
        {
            printf(1, "Child with priority %d : Turnaround time: %d, Waiting time: %d, CBT time: %d\n",
                   priorities[j], turnarounds[j], waitings[j], CBTs[j]);
        }

        int turnaroundsPerClass[6] = {0};
        int waitingsPerClass[6] = {0};
        int CBTsPerClass[6] = {0};
        for (int j = 0; j < NUM_CHILDREN; j++)
        {
            int childPriority = priorities[j];
            turnaroundsPerClass[childPriority - 1] += turnarounds[j];
            waitingsPerClass[childPriority - 1] += waitings[j];
            CBTsPerClass[childPriority - 1] += CBTs[j];
        }
        for (int j = 0; j < 6; j++)
        {
            printf(1, "Priority class: %d :\n avarage Turnaround: %d, avarage Waiting: %d, avarage CBT: %d\n",
                   j + 1,
                   turnaroundsPerClass[j] / (NUM_CHILDREN / 6),
                   waitingsPerClass[j] / (NUM_CHILDREN / 6),
                   CBTsPerClass[j] / (NUM_CHILDREN / 6));
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
        printf(1, "Total :\n avarageTurnaround: %d, avarage Waiting: %d, avarage CBT: %d\n",
               turnaroundsSum / NUM_CHILDREN,
               waitingsSum / NUM_CHILDREN,
               CBTsSum / NUM_CHILDREN);
    }

    while (wait() != -1)
        ;

    exit();
}