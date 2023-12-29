// 如果用户忘记传递参数，sleep 应该打印一个错误消息。
// 命令行参数以字符串形式传递；可以使用 atoi 将其转换为整数（参见 user/ulib.c）。
// 使用系统调用 sleep。
// 查看 xv6 内核代码中实现 sleep 系统调用的部分（查找 sys_sleep）：kernel/sysproc.c。
// 查看 user/user.h 获取从用户程序中调用的 sleep 的 C 定义，以及 user/usys.S 中从用户代码跳转到内核进行 sleep 的汇编代码。
// 在 main 函数完成时调用 exit(0)。
// 将你的 sleep 程序添加到 Makefile 的 UPROGS 中，这样运行 make qemu 时就会编译你的程序，然后可以从 xv6 shell 中运行它。
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){
  if(argc!=2){
    //提醒要传入参数
    fprintf(2,"Usage: sleep <ticks>\n");
    exit(1);
  }
  int n = atoi(argv[1]);
 
  if(sleep(n)!=0){
    fprintf(2, "sleep: %s failed\n", argv[1]);
    exit(1);
  }
  exit(0);
}
