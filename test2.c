// test5.c - Mixed Workload Test
#include "types.h"
#include "stat.h"
#include "user.h"

#define RUNTIME 1000
#define COMPUTE_SIZE 100000

void mixed_workload(int pid, int priority) {
    int computations = 0;
    int start_time = uptime();
    
    while(1) {
        // CPU-bound work
        for(volatile int i = 0; i < COMPUTE_SIZE; i++);
        computations++;
        
        // I/O simulation
        if(computations % 10 == 0) {
            sleep(1);
        }
        
        // Print progress every 50 computations
        if(computations % 50 == 0) {
            printf(1, "PID %d (Pri %d): %d computations\n", 
                   pid, priority, computations);
        }
        
        // Check runtime
        if(uptime() - start_time >= RUNTIME) {
            printf(1, "\nFinal - PID %d (Priority %d): %d computations\n", 
                   pid, priority, computations);
            break;
        }
    }
}

int main(void) {
    int pid1, pid2, pid3;
    printf(1, "=== Test Case 2: Mixed Workload Test ===\n");
    printf(1, "\nMixed Workload Test - %d ticks\n", RUNTIME);
    printf(1, "Processes perform both CPU and I/O operations\n\n");
    
    pid1 = fork();
    if(pid1 == 0) {
        nice(getpid(), 1);  // Highest priority
        mixed_workload(getpid(), 1);
        exit();
    }
    
    pid2 = fork();
    if(pid2 == 0) {
        nice(getpid(), 3);  // Lowest priority
        mixed_workload(getpid(), 3);
        exit();
    }
    
    pid3 = fork();
    if(pid3 == 0) {
        nice(getpid(), 5);  // Lowest priority
        mixed_workload(getpid(), 5);
        exit();
    }
    
    wait();
    wait();
    wait();
    printf(1, "=== Test Case 2 Completed ===\n");
    exit();
}