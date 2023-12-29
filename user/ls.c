#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.  如果文件名长度超过 DIRSIZ，则直接返回原始文件名
  if(strlen(p) >= DIRSIZ)
    return p;

  //将文件名复制到 buf 中，并在后面填充空格，以确保总长度为 DIRSIZ+1
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void
ls(char *path)
{
  char buf[512], *p;
  int fd;
  struct dirent de;//struct dirent 结构体，用于存储读取到的目录项
  struct stat st;//struct stat 结构体，用于存储文件或目录的状态信息

  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }
  // 获取文件或目录的状态信息
  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_DEVICE:
  case T_FILE:
  // 如果是设备或文件，直接打印信息
    printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    //读取目录项,存储到de中
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      //如果 inum 为 0，说明当前目录项是 . 或 ..，跳过当前迭代，继续下一次循环
      if(de.inum == 0)
        continue;
      //DIRSIZ 是文件名的最大长度,p 指向 buf 中的当前位置dir/,把文件名复制上去
      memmove(p, de.name, DIRSIZ);
      //添加字符串结束符
      p[DIRSIZ] = 0;
      //获取当前目录项的状态信息
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      //打印目录项信息
      printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
    }//重复该循环

    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  int i;

  if(argc < 2){
    ls(".");
    exit(0);
  }
  for(i=1; i<argc; i++)
    ls(argv[i]);
  exit(0);
}
