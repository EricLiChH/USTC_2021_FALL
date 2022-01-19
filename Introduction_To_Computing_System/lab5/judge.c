#include<stdio.h>
#include<stdlib.h>

int judge(int r0) {
    int i = 2;
    int r1 = 1;
    while (i * i <= r0) {
        if (r0 % i == 0) {
            r1 = 0;
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
    printf("R1的值是%d\n",judge(r0));
    system("pause");
    return 0;
}