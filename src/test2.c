#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>


// void* routine() {
//     x += 5;
//     sleep(2);
// }

int main() {
    int x = 2;
    // child process
    int pid = fork();
    if (pid == -1) {
        return 1;
    }
    // Process id 2674408
    // Process id 2674409
    printf("Process id %d\n", getpid());
    //child process
    if (pid == 0){
        x++;
    }
    sleep(2);
    printf("value of x: %d\n", x);
    // parent process 0 waiting child
    if (pid != 0) {
        wait(NULL);
    }
    return 0;
}