#include "types.h"
#include "stat.h"
#include "user.h"

int is_prime(int n) {
    if (n <= 1) return 0;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return 0;
    }
    return 1;
}

void print_primes() {
    for (int i = 2; i < 1000; i++) {
        if (is_prime(i)) {
            printf(1, "PID %d: %d\n", getpid(), i);
        }
    }
}

int main(void) {
    int pid1 = fork();
    if (pid1 == 0) {
        // Child process 1: lowest priority
        #ifdef SCHEDPOLICY_PRIORITY
            nice(getpid(),5);
            printf(1,"\nLowest Priority pid: %d\n",getpid());
        #endif
        print_primes();
        exit();
    }

    int pid2 = fork();
    if (pid2 == 0) {
        // Child process 2: medium priority
        #ifdef SCHEDPOLICY_PRIORITY
            nice(getpid(),3);
            printf(1,"\nMedium Priority: %d\n",getpid());
        #endif
        print_primes();
        exit();
    }

    int pid3 = fork();
    if (pid3 == 0) {
        // Child process 3: highest priority
        #ifdef SCHEDPOLICY_PRIORITY
            nice(getpid(),1);
            printf(1,"\nHighest Priority: %d\n",getpid());
        #endif
        print_primes();
        exit();
    }

    wait(); wait(); wait();
    exit();
}
