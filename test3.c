// test3.c
#include "types.h"
#include "stat.h"
#include "user.h"

#define RUNTIME 1000  // Runtime limit

void compute_work(int pid, int priority) {
    int count = 0;
    int start_time = uptime();
    int current_time;
    
    while(1) {
        count++;
        if(count % 1000000 == 0) {  // Print progress periodically
            current_time = uptime();
            if(current_time - start_time >= RUNTIME) {
                printf(1, "PID: %d (Priority: %d) - Work done: %d\n", 
                       pid, priority, count/1000000);
                break;
            }
        }
    }
}

int main(void) {
    int pid1, pid2, pid3;
    printf(1, "=== Test Case 3: Priority Scheduling Test ===\n");
    printf(1, "\nStarting Priority Scheduling Test\n");
    printf(1, "Each process will run for %d ticks\n", RUNTIME);
    printf(1, "Higher work count indicates more CPU time\n\n");
    
    pid1 = fork();
    if(pid1 < 0) {
        printf(2, "Fork failed\n");
        exit();
    }
    
    if(pid1 == 0) {  // First child - High priority
        nice(getpid(), 1);
        compute_work(getpid(), 1);
        exit();
    }
    
    pid2 = fork();
    if(pid2 < 0) {
        printf(2, "Fork failed\n");
        exit();
    }
    
    if(pid2 == 0) {  // Second child - Medium priority
        nice(getpid(), 3);
        compute_work(getpid(), 3);
        exit();
    }
    
    pid3 = fork();
    if(pid3 < 0) {
        printf(2, "Fork failed\n");
        exit();
    }
    
    if(pid3 == 0) {  // Third child - Low priority
        nice(getpid(), 5);
        compute_work(getpid(), 5);
        exit();
    }
    
    // Parent waits for all children
    wait();
    wait();
    wait();
    printf(1, "=== Test Case 3 Completed ===\n");
    exit();
}