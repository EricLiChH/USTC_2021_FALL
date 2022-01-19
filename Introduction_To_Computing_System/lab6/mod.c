#include<stdlib.h>
#include<stdio.h>

short push(short i){
    return i>>3;
}

short mod(short i){
    while (i>7){
        i=i&7+push(i);
    }
    if(i==7)
        return 0;
    else 
        return i;
}

int main() {
    short i;
    printf("请输入目标值；");
    scanf("%d",&i);
    printf("\n结果是%d\n",mod(i));
    system("pause");
    return 0;
}