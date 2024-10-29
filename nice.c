#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        printf(2, "Usage: nice <pid> [<niceness_value>] or nice [<niceness_value>]\n");
        exit();
    }

    int pid;
    int niceness_value;

    if(argc==2){
        niceness_value = atoi(argv[1]);
        pid = getpid();
    }

    if(argc==3){
        pid = atoi(argv[1]);
        niceness_value = atoi(argv[2]);
    }
    if(niceness_value<1)
        niceness_value=1;
    else if(niceness_value>5)
        niceness_value=5;

    int x = nice(pid,niceness_value);
    
    if (x < 0) {
        printf(2, "Error setting nice value for PID %d\n", pid);
    }
    else{
        printf(1,"%d %d\n",pid,x);
    }

    exit();
}
