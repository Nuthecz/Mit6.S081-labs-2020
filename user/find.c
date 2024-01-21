#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
void find_file(char *target_path, char *target_file);
int main(int argc, char **argv)
{
    // 检测参数数量 -> find . b
    if (argc != 3)
    {
        fprintf(2, "find: Parameter error\n");
        exit(1);
    }
    char *target_path = argv[1];
    char *target_file = argv[2];
    find_file(target_path, target_file);
    exit(0);
}

void find_file(char *target_path, char *target_file)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(target_path, 0)) < 0) // 打开文件
    {
        printf("find: cannot open %s\n", target_path);
        return;
    }

    if (fstat(fd, &st) < 0) // 获取文件信息
    {
        printf("find: cannot stat %s\n", target_path);
        close(fd);
        return;
    }

    // 第二个参数必定为目录，即st.type==T_DIR
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
        // 修补完整路径
        strcpy(buf, target_path);
        p = buf + strlen(buf);
        *p++ = '/';
        if (de.inum == 0)
            continue;
        // 移动名称，这个时候p已经指向了/后面，所以直接赋值
        memmove(p, de.name, DIRSIZ);
        // 截断
        p[DIRSIZ] = 0;
        if (stat(buf, &st) < 0)
        {
            printf("find: cannot stat %s\n", buf);
        }

        switch (st.type){
        case T_FILE:
            if (!strcmp(de.name, target_file))
            {
                printf("%s\n", buf); // p是指针，buf已经是完整路径
            }
            break;
        case T_DIR:
            if (strcmp(de.name, ".") && strcmp(de.name, ".."))
            {
                find_file(buf, target_file);
            }
        }
    }
    close(fd);
    return;
}