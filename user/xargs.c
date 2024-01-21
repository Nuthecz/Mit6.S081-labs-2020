#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"
#define MAX_NUM 100

int main(int argc, char **argv)
{
    char args[MAXARG][MAX_NUM];
    char buf;
    char *xargs[MAXARG];
    char *command = argv[1];
    // 遇到'\n'的情况进行下一轮循环，重新传递参数
    while (1)
    {
        memset(args, 0, MAXARG * MAX_NUM);
        int count = argc - 1; // 统计参数数量
        // 处理xargs的参数
        for (int i = 0; i < count; i++)
        {
            strcpy(args[i], argv[i + 1]);
        }

        // 从标准输入单字节读取数据，处理xargs之前的参数
        int flag = 0; // 标记argv的结束
        int num = 0;  // 统计每个argv的长度
        int result;   // 记录是否读取到'\n'
        while ((result = read(0, &buf, sizeof(char))) > 0 && buf != '\n')
        {
            if (buf == ' ' && flag)
            {
                count++;
                num = 0;
                flag = 0;
            }
            else if (buf != ' ')
            {
                args[count][num++] = buf;
                flag = 1;
            }
        }

        // 读到'\n'退出循环while(1)
        if (result <= 0)
            break;

        // 参数合并
        for (int i = 0; i < MAXARG - 1; i++)
        {
            xargs[i] = args[i];
        }
        xargs[MAXARG - 1] = 0;//在输入参数的时候最后一个是回车，改为截断

        int pid = fork();
        if (pid > 0)
        {
            wait(0);
        }
        else
        {
            exec(command, xargs);
            exit(0);
        }
    }
    exit(0);
}