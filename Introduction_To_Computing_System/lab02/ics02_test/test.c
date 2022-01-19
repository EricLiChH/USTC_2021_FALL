#include <stdio.h>
#include<stdlib.h>

int f(int n0, int n1, int n2,int n){
        int fn;
        fn=(2*n0+n2)%1024;
        if (n==0)
            return 1;
        else if (n == 1) 
            return 1;
        else if (n == 2) 
            return 2;
        else if(n==3)
            return fn;
        else if(n>3)
            return f(n1,n2,fn,n-1);
        else
            return 0;
}

int main(){
    int n;

    scanf("%d",&n);
    for ( int i = 200; i < 1000; i++)
    {
        for(int j = 0; j < n; j++)
            if (f(1,1,2,i)==f(1,1,2,i+j)&&f(1,1,2,i+1)==f(1,1,2,i+j+1)&&f(1,1,2,i+2)==f(1,1,2,i+j+2)){
                printf("%d\n",j);
        }
    }
    system("pause");
    return 0;
}