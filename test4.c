#include "types.h"
#include "stat.h"
#include "user.h"

#define RESOURCE_ID 1

void basic_lock_unlock_task(int priority) {
  nice(getpid(), priority);
  printf(1, "Process %d with priority %d attempting to acquire lock\n", getpid(), priority);
  
  int lock_status = resourcelock_acquire(RESOURCE_ID);
  if (lock_status == 0) {
    printf(1, "Process %d acquired lock\n", getpid());
    sleep(3000); // Simulate work with the lock held for 3 seconds
    printf(1, "Process %d releasing lock\n", getpid());
    resourcelock_release(RESOURCE_ID);
  } else {
    printf(1, "Process %d failed to acquire lock\n", getpid());
  }
  
  printf(1, "Process %d finished\n", getpid());
  exit();
}

int main(void) {
  printf(1, "=== Test Case 4: Basic Lock/Unlock ===\n");
  
  int pid1 = fork();
  if (pid1 == 0) {
    basic_lock_unlock_task(3);
  } else {
    int pid2 = fork();
    if (pid2 == 0) {
      sleep(1000); // Ensure the first process starts acquiring the lock first
      basic_lock_unlock_task(3);
    } else {
      wait();
      wait();
      printf(1, "=== Test Case 4 Completed ===\n");
    }
  }
  
  exit();
}
