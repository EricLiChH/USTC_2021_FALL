#include<stdio.h>
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

int main() {
    int i;
    int fn;
    FILE* fp;
    fp=fopen("list.txt","w+");
    for (i = 1; i <16385;i++){
        for (i = 0; i <16385;i++){
            fprintf(fp, "%d,\n",f(1,1,2,i));
        }
        fprintf(fp, ".end\n");
    }
}

