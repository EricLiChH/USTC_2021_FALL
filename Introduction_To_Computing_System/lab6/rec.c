#include<stdio.h>
#include<stdlib.h>

short R0, R1, R2, R3, R4, R5, R6, R7;
int mem[65536];

int rec(){
    int i=R2;
    do {
        R2+=1;
        R0+=1;
        R1=mem[0x3019];
        R1-=1;
        mem[0x3019]=R1;
    }while(R1!=0);
    R2-=1;
    return R0;
}

int main(){
    int i;
    R0 = 0;
    R2=0x300F;
    scanf("请输入存储值%d",&mem[0x3019]);
    //mem[0x3019]=5;
    rec();
    printf("\n%d %x\n",R0);
    system("pause");
    return 0;
}