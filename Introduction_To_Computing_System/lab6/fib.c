#include<stdio.h>
#include<stdlib.h>

short fib(short i){
    short f1,f2,f3,f4,f5;
    short mod=1023;
    short answer;
    f1=1;f2=1;f3=2;
    if(i==1)
        return 1;
    if (i == 2) {
        return 2;
    }
    for (i-=2; i>0; i--)
    {
        f1=f1+f1;
        f4=f1+f3;
        f5=f4&mod;
        f1=f2;
        f2=f3;
        f3=f5;
        answer=f3;
    }
    return answer;
}

int main(){
    short i;
    printf("请输入R0中存储的值：");
    scanf("%d",&i);
    printf("\n得到值为%d\n",fib(i));
    system("pause");
    return 0;
}