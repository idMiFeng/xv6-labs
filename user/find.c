#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char*
fmtname(char *path);

void find(char *path, char *name);

int
main(int argc, char *argv[])
{
  if(argc < 3){
		printf("Usage: find path filename\n");
		exit(1);
	}

	find(argv[1], argv[2]);

	exit(0);
}

char*
fmtname(char *path)
{
    char *p;
	for(p=path+strlen(path); p >= path && *p != '/'; p--)
		;
	p++;

	return p;
}

void find(char *path, char *name) {
   
    if(strcmp(fmtname(path), name)==0){
        printf("%s\n", path);
    }
    
    char buf[512], *p;
    int fd;
    struct dirent de;//struct dirent 结构体，用于存储读取到的目录项
    struct stat st;//struct stat 结构体，用于存储文件或目录的状态信息
   
    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
   
   // 获取文件或目录的状态信息
    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    if (st.type != T_DIR) {
		close(fd);
		return;
	}

    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
        printf("find: path too long\n");
        return;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
        //读取目录项,存储到de中
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
    //如果 inum 为 0，说明当前目录项是 . 或 ..，跳过当前迭代，继续下一次循环
    if (!strcmp(de.name, ".") || !strcmp(de.name, "..") || de.inum == 0)
		continue;
    //DIRSIZ 是文件名的最大长度,p 指向 buf 中”dir/“的下一个字符,把文件名复制上去
    memmove(p, de.name, DIRSIZ);
    //添加字符串结束符
    p[DIRSIZ] = 0;
    //递归调用find查找子目录
    find(buf,name);
    }//重复该循环
    close(fd);
}