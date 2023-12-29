#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"  // MAXARG 命令行参数的最大数量

void strncpy(char *dest, char *src, int n);


int main(int argc, char *argv[]){
	if(argc < 2){
		printf("Usage: xargs <command> [argv...]\n");
		exit(1);
	}
    char buf[512], *p,ch; // p用来指示当前指针在buf中的位置
    char commands[MAXARG][MAXARG];  //commands 是最终要传入exec命令的参数
    // 首先把xargs自己的参数赋值给commands ，在之后的执行过程中会把从标准输入获得的参数也赋值给commands
    int i;
    for(i=0;i<argc-1;i++){
        strncpy(commands[i],argv[i+1],sizeof(argv[i+1]));
    }
    int k =i; // 记录xargs 自己的参数的数量（i是commands的下标，接下来 i 会变）
   
    
    // 读标准输入，先把标准输入的全部内容放在buf里
    int j = 0;
    while (read(0, &ch, 1) > 0){
        buf[j++]=ch;
    }
    
    p = buf;

    // 定义word用来从buf中复制相应的字符串
    char word[MAXARG];
    // j 用来迭代 word 
    j = 0;
    //从buf中读数据复制到 word
    for(;*p!='\0';p++){
        //当*p == '\n'时，说明此时已经读完了标准输入一行数据，那么就执行一次，然后再继续读下一行
        while(*p != '\n'){
            //如果为空格就说明此时word已经获得了一个参数，把word的内容复制到commands里
            if(*p == ' '){
                word[j++]='\0';
                strncpy(commands[i++], word, sizeof(word));
                //清空现在word，重新存储下一个参数
                memset(word, 0, sizeof(word));
                p++;
                //清空word的下标
                j=0;
            }else{
                word[j++]=*p;
                p++;
            }
        }
        //此时p遇到'\n'退出while循环，把最后一次读到的word复制到commands中
        word[j++]='\0';
        strncpy(commands[i++], word, sizeof(word));
        memset(word, 0, sizeof(word));
        //用args来存储commands的参数放进exec执行，因为commands是二维数组，需要转换一下
        char *args[MAXARG];
        for (j = 0; j < i; j++) {
            args[j] = commands[j];
        }
        
        if (fork()==0) {
            exec(args[0], args);
            exit(0);
        }
        wait(0);
        // j 用来记录
        j=0;
        // 把从标准输入得到的这一行赋值给commands的参数清空, 这样commands就只剩下xargs的参数，k是之前记录的xargs自己的参数的数量
        for(;k<i;k++){
             memset(commands[k], 0, sizeof(commands[k]));
        }
        i=k;
        
}
   
    exit(0);
}

//不能用C库，自己写一个strncpy
void strncpy(char *dest,char *src, int n) {
    int i;
    for (i = 0; i < n && src[i] != '\0'; i++) {
        *(dest+i) = src[i];
    }
    dest[i] = '\0';  
}

