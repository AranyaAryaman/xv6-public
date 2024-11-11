#include "types.h"
#include "stat.h"
#include "user.h"

#define RESOURCE_ID 2

void print_timestamp(const char* message) {
  printf(1, "[%d ms] %s\n", uptime(), message);
}

void print_priority(int pid){
  int pr = getpr(pid);
  printf(1,"%d has priority %d\n", pid, pr);
}

void priority_inversion_low_task(void) {
  int pid1 = getpid();
  print_timestamp("Low priority task started");
  nice(pid1, 5); // Set to low priority
  print_priority(pid1);
  int x = getpr(pid1);
  print_timestamp("Low priority task acquiring lock");
  resourcelock_acquire(RESOURCE_ID);
  print_timestamp("Low priority task acquired lock");

  // Simulate work while holding the lock for 5 seconds
  for (int i = 0; i < 5; i++) {
    sleep(1000); // 1 second
    printf(1, "Low priority task working...\n");
    if(x!=getpr(pid1)|| i==0)
      printf(1,"[%d ms] Process %d has priority %d\n",uptime(),pid1,getpr(pid1));
  }

  print_timestamp("Low priority task releasing lock");
  resourcelock_release(RESOURCE_ID);
  print_timestamp("Low priority task finished");
  print_priority(pid1);
  exit();
}

void priority_inversion_high_task(void) {
  int pid2 = getpid();
  sleep(3000); // Delay start to create inversion scenario
  print_timestamp("High priority task started");
  nice(pid2, 1); // Set to high priority
  print_priority(pid2);

  print_timestamp("High priority task attempting to acquire lock");
  resourcelock_acquire(RESOURCE_ID);
  print_timestamp("High priority task acquired lock");

  // Release the lock immediately after acquisition
  resourcelock_release(RESOURCE_ID);
  print_timestamp("High priority task finished");
  print_priority(pid2);
  exit();
}

int main(void) {
  printf(1, "=== Test Case 5: Priority Inversion Detection ===\n");
  
  int low_pid = fork();
  if (low_pid == 0) {
    priority_inversion_low_task();
  } else {
    int high_pid = fork();
    if (high_pid == 0) {
      priority_inversion_high_task();
    } else {
      wait();
      wait();
      printf(1, "=== Test Case 5 Completed ===\n");
    }
  }
  
  exit();
}
