#include "types.h"
#include "stat.h"
#include "user.h"

#define NUM_CHILDREN 10

int main(int argc, char *argv[])
{
    changePolicy(1);
    int main_pid = getpid();
    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        if (fork() > 0)
            break;
    }

    if (main_pid != getpid())
    {
        for (int i = 0; i < 100; i++)
            printf(1, "/%d/: %d\n", getpid(), i);

        int turnAroundTime = getTurnAroundTime(getpid());
        int waitingTime = getWaitingTime(getpid());
        int CBT = getCBT(getpid());

        wait();

        printf(1, "turnAroundTime for process /%d/ is :%d\n", getpid(), turnAroundTime);
        printf(1, "waitingTime for process /%d/ is :%d\n", getpid(), waitingTime);
        printf(1, "CBT for process /%d/ is :%d\n", getpid(), CBT);
        printf(1, "\n\n");
    }
    else{ 

        int turnarounds[NUM_CHILDREN] = {0}; 
        int waitings[NUM_CHILDREN] = {0};  
        int CBTs[NUM_CHILDREN] = {0};   

        int *procTimes = malloc(4 * sizeof(int));
        int i = 0;
        while (wait_(procTimes) > 0)
        {
            int childTurnaround = procTimes[0];
            int childWaiting = procTimes[1];
            int childCBT = procTimes[2];

            turnarounds[i] = childTurnaround;
            waitings[i] = childWaiting;
            CBTs[i] = childCBT;
            i++;
        }

           

     printf(1, "\n\n\n*****AVG Times in total*****\n");
        int turnaroundsSum = 0;
        int waitingsSum = 0;
        int CBTsSum = 0;
        for (int j = 0; j < NUM_CHILDREN; j++)
        {
            turnaroundsSum += turnarounds[j];
            waitingsSum += waitings[j];
            CBTsSum += CBTs[j];
        }
        printf(1, "Total -> AVG Turnaround: %d, AVG Waiting: %d, AVG CBT: %d\n",
               turnaroundsSum / NUM_CHILDREN,
               waitingsSum / NUM_CHILDREN,
               CBTsSum / NUM_CHILDREN);
    }

    while (wait() != -1);
    exit();
}
