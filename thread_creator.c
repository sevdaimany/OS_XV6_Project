#include "types.h"
#include "stat.h"
#include "user.h"

#define PAGESIZE  4096

int 
thread_creator(void (*fn) (void *), void *arg)
{
    void *fptr = malloc(2* (PAGESIZE));
    void *stack;

    if(fptr == 0)
    return -1;

    // the following if-else is for assigning page-aligned space to stack

    int mod = (uint)fptr % PAGESIZE;

    if (mod == 0)
        stack = fptr;
    else
        stack = fptr + (PAGESIZE - mod);

    // clone = thread_create system call
    int thread_id = clone((void*)  stack);

    // clone failed
    if(thread_id < 0)
        printf(1, "clone failed\n");
    
    // child
    else if (thread_id == 0){
        // call tthe function passed to thread create
        (fn)(arg);
        // free space when function is finished
        free(stack);
        exit();
    }
    return thread_id;
}