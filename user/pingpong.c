#include "kernel/types.h"
#include "user/user.h"
#define SIZE 1
int main(int argc, char **argv)
{
    int p1[2], p2[2];
    // p1 is for parent write to child
    // p2 is for child write to parent
    int pid;
    pipe(p1);
    pipe(p2);
    char buf[SIZE];
    pid = fork();
    if (pid)
    { //  parent
        close(p1[0]);
        close(p2[1]);

        write(p1[1], " ", SIZE);
        read(p2[0], buf, SIZE);
        printf("%d: received pong\n", getpid());

        close(p1[1]);
        close(p2[0]);
    }
    else
    { // child
        close(p1[1]);
        close(p2[0]);

        read(p1[0], buf, SIZE);
        printf("%d: received ping\n", getpid());
        write(p2[1], " ", SIZE);

        close(p1[0]);
        close(p2[1]);
        exit(0);
    }

    exit(0);
}
