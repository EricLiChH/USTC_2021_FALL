#include<stdio.h>
#include<stdlib.h>

int SQR(int i){
    int count=i;
    int ans=0;
    while(count){
        ans+=i;
        count--;
    }
    return ans;
}

int MOD(int i,int j){
    int counter = 0;
    while (counter < i){
        counter+=j;
    }
    if(counter==i)
        return 0;
    else 
        return 1;
}

int JUDGE(int r0){
    int i = 2,r1 = 1;
    while (SQR(i)<=r0)
    {
        if (!MOD(r0,i))
        {
            r1=0;
            break;
        }
        i++;
        
    }
    return r1;
}

int main(){
    int r0;
    printf("请输入R0的值:");
    scanf("%d",&r0);
    printf("R1的值是%d\n",JUDGE(r0));
    system("pause");
    return 0;
}