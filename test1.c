// test1.c - CPU Burst Test
#include "types.h"
#include "stat.h"
#include "user.h"

#define RUNTIME 800
#define BURST_SIZE 1000000

void cpu_burst(int pid, int priority) {
    int bursts_completed = 0;
    int start_time = uptime();
    
    while(1) {
        // Perform a CPU burst
        for(volatile int i = 0; i < BURST_SIZE; i++);
        
        bursts_completed++;
        
        // Check if runtime exceeded
        if(uptime() - start_time >= RUNTIME) {
            printf(1, "Process %d (Priority %d): Completed %d bursts\n", 
                   pid, priority, bursts_completed);
            break;
        }
    }
}

int main(void) {
    int pid1, pid2, pid3;
    printf(1, "=== Test Case 1: CPU Burst Test ===\n");
    printf(1, "\nCPU Burst Test - Running for %d ticks\n", RUNTIME);
    printf(1, "Each process performs fixed-size CPU bursts\n\n");
    
    pid1 = fork();
    if(pid1 == 0) {
        nice(getpid(), 1);  // Highest priority
        cpu_burst(getpid(), 1);
        exit();
    }
    
    pid2 = fork();
    if(pid2 == 0) {
        nice(getpid(), 3);  // Medium priority
        cpu_burst(getpid(), 3);
        exit();
    }
    
    pid3 = fork();
    if(pid3 == 0) {
        nice(getpid(), 5);  // Lowest priority
        cpu_burst(getpid(), 5);
        exit();
    }
    
    wait();
    wait();
    wait();
    printf(1, "=== Test Case 1 Completed ===\n");
    exit();
}