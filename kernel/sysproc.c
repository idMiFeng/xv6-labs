#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;
  // 从用户空间获取第一个参数，即休眠的时钟 ticks 数
  argint(0, &n);
  // 获取时钟锁，防止在获取ticks值的时候被中断
  acquire(&tickslock);
  // 记录当前ticks的值
  ticks0 = ticks;
  // 当当前 ticks 的增加量小于 n 时，一直循环
  while (ticks - ticks0 < n) {
    // 如果进程被杀死，释放锁并返回-1表示错误
    if (killed(myproc())) {
      release(&tickslock);
      return -1;
    }
    // 使进程休眠，等待时钟中断唤醒
    sleep(&ticks, &tickslock);
  }
  // 释放时钟锁
  release(&tickslock);
  // 返回0表示休眠完成
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
