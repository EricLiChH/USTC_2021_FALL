#include<stdio.h>
#include<stdlib.h>

short mul(short i,short j){
    short counter=j,ans=0;
    while (counter--)
        ans+=i;
    return ans;
}
int main(){
    int i, j;
    printf("请输入乘数与被乘数(使用空格间隔开)：");
    scanf("%d %d",&i,&j);
    printf("答案是：%d\n",mul(i,j));
    system("pause");
    return 0;
}