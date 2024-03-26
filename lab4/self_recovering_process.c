#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>


void sigusr1_handler(int signum) {
    printf("Received SIGTERM signal. Restarting...\n");
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(-1);
    }
    if (pid == 0) {
        printf("Child process is running. PID: %d\n", getpid());
    } else {
        exit(0);
    }
}


int main() {
    printf("Self-recovering process is running. PID: %d\n", getpid());

    signal(SIGINT, sigusr1_handler);
    
    int count = 0;
    while (count <= 15) {
        printf("Count: %d\n", count++);
        sleep(1);
    }

    return 0;
}