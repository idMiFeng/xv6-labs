#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include <stdbool.h>

bool is_prime(int num);

int main(int argc, char *argv[]){
    int int_nums[2];
    pipe(int_nums);

    //child用来把数字写入int_nums
    if(fork()==0){
        for(int i = 2;i < 36;i++){
            write(int_nums[1],&i,sizeof(i));
        }
        exit(0);
    }
    wait(0);
    close(int_nums[1]);
    int out_nums[2];
    pipe(out_nums);

    //child用来从int_nums取数字筛选后写入out_nums
    if(fork()==0){
        int num;
        while(read(int_nums[0],&num,sizeof(num))){
            if(is_prime(num)){
                write(out_nums[1],&num,sizeof(num));
            }
        }
        exit(0);     
    }
    wait(0);
    close(int_nums[0]);
    close(out_nums[1]);
    int prime;
    while(read(out_nums[0],&prime,sizeof(prime))){
        printf("prime %d\n", prime); 
    }
    exit(0);
}

bool is_prime(int num) {
    
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            return false;  
        }
    }

    return true;  
}