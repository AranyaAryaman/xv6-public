#include "types.h"
#include "stat.h"
#include "user.h"

#define RESOURCE_ID 0

void print_priority(int pid) {
  int priority = getpr(pid);
  printf(1, "PID %d current priority: %d\n", pid, priority);
}

void low_priority_task(void) {
  printf(1, "Low priority task started\n");
  nice(getpid(), 5);  // Set to lowest priority
  print_priority(getpid());
  
  printf(1, "Low priority task acquiring lock\n");
  resourcelock_acquire(RESOURCE_ID);
  printf(1, "Low priority task acquired lock\n");
  
  // Simulate work
  for (int i = 0; i < 5; i++) {
    sleep(1000);  // Sleep for 1 second
    printf(1, "Low priority task working...\n");
    print_priority(getpid());
  }
  
  printf(1, "Low priority task releasing lock\n");
  resourcelock_release(RESOURCE_ID);
  printf(1, "Low priority task finished\n");
  print_priority(getpid());
  exit();
}

void high_priority_task(void) {
  sleep(3000);  // Wait for 3 seconds before starting
  printf(1, "High priority task started\n");
  nice(getpid(), 1);  // Set to highest priority
  print_priority(getpid());
  
  printf(1, "High priority task attempting to acquire lock\n");
  resourcelock_acquire(RESOURCE_ID);
  printf(1, "High priority task acquired lock\n");
  resourcelock_release(RESOURCE_ID);
  printf(1, "High priority task finished\n");
  print_priority(getpid());
  exit();
}

int main(void) {
  printf(1, "=== Test Case 6: Priority Inversion Demonstration ===\n");
  int low_pid, high_pid;

  low_pid = fork();
  if (low_pid == 0) {
    low_priority_task();
  } else {
    high_pid = fork();
    if (high_pid == 0) {
      high_priority_task();
    } else {
      wait();
      wait();
      printf(1, "=== Test Case 6 Completed ===\n");
    }
  }
  exit();
}