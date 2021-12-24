#include "types.h"
#include "stat.h"
#include "user.h"


void childPrint(void *args){
    printf(1,"Child function executed properly with argument: %d\n", *(int*) args);
}
int main(void){

    int argument = 0x0F01;

    int thread_id = thread_creator(&childPrint , (void*) &argument);

    if (thread_id < 0)
        printf(1, "error!");
    
    join();
    printf(1, "Thread ID: %d\n", thread_id);
    exit();
}