#include "kernel/types.h"
#include "user/user.h"
#define MAX_NUM 36
void child(int *pl);
int main(int argc, char **argv)
{
    int pl[2];
    pipe(pl);
    int pid = fork();
    if (pid > 0)
    { // 父节点
        close(pl[0]);
        for (int i = 2; i < MAX_NUM; i++)
            write(pl[1], &i, sizeof(int)); // write的第二个参数为const void*
        close(pl[1]);
        wait(0);
    }
    else
    { // 子节点
        child(pl);
    }
    exit(0);
}
void child(int *pl)
{
    close(pl[1]);
    int num;

    // 结束条件
    int result = read(pl[0], &num, sizeof(int));
    if (!result)
    {
        exit(0);
    }

    // 处理素数
    int pr[2];
    pipe(pr);
    int pid = fork();
    if (pid > 0)
    {
        close(pr[0]);
        printf("prime %d\n", num);
        int prime = num;                       // 获得打印的素数
        while (read(pl[0], &num, sizeof(int))) // 去除素数的倍数
        {
            if (num % prime)
            { // 不是素数的倍数
                write(pr[1], &num, sizeof(int));
            }
        }
        close(pr[1]);
        wait(0);
        exit(0);
    }
    else
    {
        child(pr);
    }
}
