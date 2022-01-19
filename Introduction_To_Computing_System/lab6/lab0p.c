#include<stdlib.h>
#include<stdio.h>

short mul(short i,short j){
    short answer = 0;
    short modder=1;
    short sqt;
    short judge=0;

    if (i<0&&j>0){
        judge = 1;
        i=-i;
    }
    if(j<0&&i>0){
        judge = 1;
        j = -j;
    }   
    if(j<0&&i<0){
        judge=0;
        i=(-1)*i;
        j =(-1)*j;
    }     
    if(!i||!j)
        return answer;

    for ( sqt=i; modder>0; modder=modder<<1)
    {
        if(j&modder)
            answer+=sqt;
        sqt+=sqt;
    }
    
    if(judge)
        return (-1)*answer;
    else 
        return answer;
}

int main(){
    int i, j;
    printf("请输入乘数与被乘数(使用空格间隔开)：");
    scanf("%d %d",&i,&j);
    printf("答案是：%d\n",mul(i,j));
    system("pause");
    return 0;
}