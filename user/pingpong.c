// 使用pipe创建一个管道。
// 使用fork创建一个子进程。
// 使用read从管道读取字节，使用write向管道写入字节。
// 使用getpid查找调用进程的进程ID。
// 在Makefile的UPROGS中添加程序。
// xv6中的用户程序有一组有限的库函数可供使用。您可以在user/user.h中查看列表；源代码（除了系统调用之外）位于user/ulib.c、user/printf.c和user/umalloc.
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
int main(int argc, char *argv[]){
    //读取（p[0]）和写入（p[1]）
    int p[2];
    pipe(p);
    char buf[128];
    if(fork()==0){
        //child
       read(p[0],buf,4);
       write(p[1],"pong",4);
       printf("%d: received %s\n", getpid(), buf);
       close(p[0]);
       close(p[1]);
       exit(0);
        
    }else{
        //parent
        write(p[1],"ping",4);
        read(p[0],buf,4);
        wait(0);
        printf("%d: received %s\n", getpid(), buf);
        close(p[0]);
        close(p[1]);
        exit(0);
    }
}