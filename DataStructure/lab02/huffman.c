#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<assert.h>

#define OK 1
#define ERROR 0
#define BLANK '\0'

#define EMPTY 0
#define FULL -1
#define UNEMPTY 1
#define MAXSIZE 200
#define SIZE 256

typedef unsigned char ELEMTYPE;//包含中文所以要用unsigned char

typedef struct 
{
    ELEMTYPE ch;//字符
    long weight;//权值
    int parent,lchild,rchild;//双亲和孩子节点
}HMTNode,*HMTree;//哈夫曼树结构

typedef struct 
{
    int status;//队伍状态
    int front,rear;
    ELEMTYPE lenth;//队长
    char elem[MAXSIZE];//存入字符
}*queue,QNode;

//以下为(循环)队列操作

int InitQueue(queue q){
    if(!q)
        return 0;
    q->status=EMPTY;
    q->front=q->rear=0;
    q->lenth=0;
    return OK;
}//创建空队

int EnQueue(queue q,char a){
    if(q->status==FULL)
        return ERROR;
    q->elem[q->rear]=a;//插入队尾，同时队尾后移
    q->rear=(q->rear+1)%MAXSIZE;
    q->lenth++;
    if(q->status==EMPTY)
        q->status=UNEMPTY;
    else if (q->front == q->rear && q->status == UNEMPTY)
        q->status=FULL;
    return OK;
}//入队操作

int DeQueue(queue q,char*a){
    if(q->status==EMPTY)
        return ERROR;
    *a=q->elem[q->front];
    q->front=(q->front+1)%MAXSIZE;
    q->lenth--;
    if(q->lenth==0)
        q->status=EMPTY;
    else
        q->status=UNEMPTY;
    return OK;
}//出队操作,返回队首字符


//以下是对HUFFMAN树的操作

int FindMinTree(HMTree T,int n){
    int temp,i;
    long weight;
    
    for ( i = 0; i <= n; i++)
    {
        if (T[i].parent ==0){
            weight=T[i].weight;
            temp=i;
            break;
        }//找到未选择节点
    }
    for (i++; i <= n; i++) {
		if (0 == T[i].parent && T[i].weight < weight) {
			weight = T[i].weight;
			temp = i;
		}
    }
    return temp;
}//找到（在0至n）权值最小的HUFFMAN树，返回值是权值最小的节点（这要求HuffmanTree是有序的）

int SortHMT(HMTree T){
    int i,j,k;
    HMTNode temp;
    for (i = SIZE-1; i >= 0; i--) {
        for (j = 0; j < i; j++)
			if (T[j].weight < T[j + 1].weight) {
				temp = T[j];
				T[j] = T[j + 1];
				T[j + 1] = temp;
			}
	}
    for (i = 0; i < SIZE; i++)
		if (0 == T[i].weight)
			return i;//有权值代表它是叶子节点，无权值代表它所对应的字符并没有出现
	return i;
}//从大到小排序,返回值是叶子节点的数量

char **HuffmanNode(HMTree T,int leaf){
    char** temp;//temp作为返回值，返回编码表
    char* ctemp;//ctemp存储当前所选中的叶子节点所对应的huffman编码
    int i,papa,start,end;//papa是双亲节点,start是编码首位，end是上一位编码，按照二进制存储进入ctemp

    temp=(char**)malloc(leaf*sizeof(char*));//temp共有叶子节点数量个元素，一个叶子对应一个字符

    if (leaf==1)
    {   temp[0]=(char*)malloc(2 *sizeof(char));
        strcpy(temp[0],"0");
        return temp;
    }//只有一个叶子节点（空文件）
    
    ctemp = (char* )malloc((leaf+1)*sizeof(char));//分配空间，最后一个元素空间分配给结束区域
    ctemp[leaf]='\0';//末尾

    for (i = 0; i < leaf; i++) {
        start=leaf;end=i;
        //开始构建该字符所对应的哈夫曼编码
        for(papa=T[i].parent;papa!=0;papa=T[papa].parent){
            if (T[papa].lchild==end)
            {
                ctemp[--start]='0';//左孩子标记为0
            }
            else {
                ctemp[--start] = '1';//右孩子标记为1
            }
            end=papa;//更新上一节点
        }
        //开始构建编码表
        temp[i]=(char*)malloc((leaf-start)*sizeof(char));//有几位分配多少个空间
        strcpy(temp[i],&ctemp[start]);//从第start开始复制（因为前面都是空的）
        printf("%3d号 %3c 编码长度为:%2d;编码:%s\n", T[i].ch, T[i].ch,leaf - start, &ctemp[start]);
    }
    
    free(ctemp);
    return temp;
}//leaf是叶子节点的个数，也就是sorthuffman的值

HMTree CreateHuffManTree(FILE* fp,long* FileLenth,int*leaf){
    HMTree TempTree=NULL;
    TempTree=(HMTree)malloc(2*SIZE*sizeof(HMTNode));//一棵树的极限节点数为2*size-1
    int i,j,k;
    int n;//n是huffman的节点总数
    ELEMTYPE word;//所读取的字符

    for (i = 0; i < SIZE; i++) {
        TempTree[i].weight=0;
        TempTree[i].ch=(ELEMTYPE)i;//强制类型转换，按照ASCII码转换存储字符
    }//初始化树

    for(*FileLenth=0;!feof(fp);(*FileLenth)++){
        word=fgetc(fp);
        TempTree[word].weight++;
    }//圈定各节点的权值（字符数）

    (*FileLenth)--;//消除最后一次增加对文件字符数造成的影响
    if (*FileLenth==0){
        printf("THERE IS NO NEED FOR YOU TO COMPRESS IT FOR THE LENTH OF THE FILE\n");
        return NULL;
    }

    *leaf=SortHMT(TempTree);//出现的字符种类,也就是huffmantree的叶子节点数
    n=2*(*leaf)-1;//节点总数

    if(n-1==0){
        TempTree[0].parent=1;
        return TempTree;
    }//只存在根节点

    else if(!*leaf)
        return NULL;//空树

    for ( i = n-1; i >=0; i--)
    {
        TempTree[i].parent=0;
        TempTree[i].lchild=TempTree[i].rchild=0;
    }//此时Temptree已经排序，数组前n个元素都是含有字符的非空节点。我们将它初始化为双亲和左右孩子都为0的节点。

    for (i = *leaf; i < n; i++) {

        //找到最小节点
        j=FindMinTree(TempTree,i-1);
        TempTree[j].parent = i;
        TempTree[i].lchild = j;

        //找到次小节点
        k=FindMinTree(TempTree, i-1);
        TempTree[k].parent = i;
        TempTree[i].rchild = k;

        TempTree[i].weight = TempTree[j].weight+TempTree[k].weight;
    }//构建huffman树
    return TempTree;
}//fp是目标文件

ELEMTYPE GetCode(queue Q){
    char c;
    ELEMTYPE code=0;

    for (int i = 0; i < 8; i++){
        if(DeQueue(Q,&c)!=EMPTY){
            if (c=='0')
                code=code<<1;//左移一位
            else
                code=(code << 1) | 1;//左移一位然后补1
        }
        else break;
    }
    return code;
}

int EnCode(FILE *fp,queue Q,int leaf,char** text,ELEMTYPE *lenth){
    char* WTemp;
    int i;
    int count=0;
    ELEMTYPE code,j;

    for ( i = 0; i < leaf; i++)
    {
        for(WTemp=text[i];*WTemp!='\0';WTemp++)
            EnQueue(Q,*WTemp);
        
        while (Q->lenth>8){
            code=GetCode(Q);//读字符
            fputc(code,fp);
            count++;
        }
    }//编码

        i=8-Q->lenth;
        code = GetCode(Q);
        *lenth=Q->lenth;//lenth是余下的编码长
        for(j=0;j<i; j++)
            code=code<<1;
        //printf(" %u ",code);
        fputc(code, fp);
        count++;
        InitQueue(Q);
        //printf("\n");
    return count;
}//text[]是每个叶子节点对应的编码，fp是对应文件，Q是存储队列，leaf是叶子节点数目

int ReadCode(queue Q,ELEMTYPE ch){
    int i;
    ELEMTYPE Ctemp;

    for (i = 0; i < 8; i++) {
        Ctemp = ch << i;
		Ctemp = Ctemp >> 7;
        if (Ctemp==1)
            EnQueue(Q,'1');
        else
            EnQueue(Q,'0');
    }

    return OK;
}//译码

int GetLength(HMTree T,ELEMTYPE* MaxLenth,char**List,ELEMTYPE* MinLenth,int leaf,FILE *fp){
    int i;
    ELEMTYPE temp;

    *MaxLenth=*MinLenth=strlen(List[0]);//初始化最大最小的编码

    for (i = 0; i <leaf;i++){
        temp=strlen(List[i]);
        fwrite(&T[i].ch, sizeof(ELEMTYPE), 1, fp);
		fwrite(&temp, sizeof(ELEMTYPE), 1, fp);	//字符和对应的编码长度写进文件
        if (temp>*MaxLenth)
            *MaxLenth = temp;
        else if(temp<*MinLenth)
            *MinLenth = temp;
    }
    return OK;
}//求编码的上下限长度

void compress(){
    int i;
    int codes,leaf;//codes是编码，leaf是叶子数
    ELEMTYPE MaxLenth,MinLenth,lastlenth,bit,ch;//maxlenth和minlenth是最大最小编码长度,bit是字符转换成的字节
    queue Q;
    HMTree T;//哈夫曼树T
    char* cp;
    char name[20];//name是文件名,ch是字符变量
    char newname[20];//newname是压缩后的文件名
    char **List;//编码表List
    char **Dictionary;//包含所有字符编码的字典
    FILE* originfile;//原文件originfile
    FILE* newfile;//新文件newfile
    long FileLenth;//filelenth是文件长（字符数）
    long count=0;//字符数统计
    long Lenth=0;//文件长度

    printf("please tell me the name of target file\n");
	scanf("%s", name);
    printf("please tell me the name after the operation\n");
	scanf("%s",newname);

    originfile=fopen(name,"rb");//只读形式打开二进制文件
    if (!originfile){
        printf("\nOH...CANNOT OPEN IT!!!!\n");
        return;
    }
    newfile=fopen(newname,"wb");//只写形式建立新二进制文件
    if (!newfile) {
        printf("\nOH...CANNOT CREATE IT!!!\n");
        return;
    }

    T=CreateHuffManTree(originfile,&FileLenth,&leaf);//建立哈夫曼树
    Q=(queue)malloc(sizeof(QNode));//建立存储队列
    InitQueue(Q);
    List=HuffmanNode(T,leaf);   
    Dictionary=(char**)malloc(SIZE*sizeof(char*));//字典，存储每一种编码

    for ( i = 0; i < SIZE; i++)
        Dictionary[i]=NULL;

    for (i = 0; i < leaf; i++) 
        Dictionary[(int)(T[i].ch)]=List[i];

    fseek(newfile,  2*sizeof(int) + 4 * sizeof(ELEMTYPE)+sizeof(long), SEEK_SET);//在文件起始位置预留空间给叶子数目和编码长度等数据 
    GetLength(T,&MaxLenth,List,&MinLenth,leaf,newfile);
    codes=EnCode(newfile,Q,leaf,List,&lastlenth);//初步压缩，将字符转为huffman编码
    
    rewind(newfile);
    fseek(newfile, sizeof(long)+sizeof(ELEMTYPE), SEEK_SET);//开头留出空间
    fwrite(&leaf, sizeof(int), 1, newfile);//叶子数
    fwrite(&MaxLenth, sizeof(ELEMTYPE), 1, newfile);	//最长码串长度
	fwrite(&MinLenth, sizeof(ELEMTYPE), 1, newfile);	//最短码串长度
	fwrite(&codes, sizeof(int), 1, newfile);	//填写叶子编码压入多少个
    fwrite(&lastlenth, sizeof(ELEMTYPE), 1, newfile);  //剩下的字长
    //printf("%d %u %u %d %u\n",leaf,MaxLenth,MinLenth,codes,lastlenth);

    fseek(newfile, 2 * leaf * sizeof(ELEMTYPE) + codes, SEEK_CUR);//移位至空白部分
    fseek(originfile, 0, SEEK_SET);//回到旧有文件

    while (count<FileLenth){
        ch=fgetc(originfile);
        count++;
        for (cp=Dictionary[ch]; *cp!='\0'; cp++)
            EnQueue(Q,*cp);
        while (Q->lenth > 8)	//  printf("OutQueue: ");
		{
			bit = GetCode(Q);	//出队8个元素,合成一个字节
            //printf(" %u ",bit);
			fputc(bit, newfile);	//fwrite(&bits,1,1,compressFile);   
            Lenth++;
		}
    }

    lastlenth = Q->lenth;//还剩多少字节

    bit = GetCode(Q);
	for (i = 0; i < 8-Q->lenth; i++)
		bit = bit << 1;
    //printf(" %u ",bit);
    fwrite(&bit, sizeof(ELEMTYPE), 1, newfile);//把最后的字符写上去
    Lenth++;

    rewind(newfile);
    fwrite(&Lenth, sizeof(long), 1, newfile);
    fwrite(&lastlenth, sizeof(ELEMTYPE), 1, newfile);
    //printf("%d %u",Lenth,lastlenth);

//压缩率
    if ((Lenth+16+codes) >= FileLenth)
		puts("\nCompression rate: 0.0%");
	else
		printf("\nCompression rate: %.2lf%c\n",
		    (double)((FileLenth -(Lenth+16+codes)) / (double)FileLenth) * 100.0, '%');

    fclose(originfile);
	fclose(newfile);
	free(Q);
	free(T);
	free(Dictionary);
    //完成压缩

}//压缩操作

void uncompress(){
    long lenth=0,filelenth=0,curr_lenth=0;
    int i,j=0;
    char name[100],newname[100];
    char** List;
    char* text,*content;
    char c;
    ELEMTYPE maxlenth,minlenth,lastlenth=0,origlenth=0;
    ELEMTYPE *words,*wordslenth;
    ELEMTYPE ch;
    FILE *originfile,*newfile;
    queue Q;
    int leaf,codes;

    fflush(stdin);
    printf("please tell me the name of target file\n");
	scanf("%s", name);
    fflush(stdin);
    printf("please tell me the name after the operation\n");
	scanf("%s",newname);

    originfile=fopen(name,"rb");
    newfile=fopen(newname,"wb");
    if(!originfile||!newfile){
        puts("Cannot open files.");
		return;
    }

    fread(&lenth,sizeof(long),1,originfile);
    fread(&lastlenth, sizeof(ELEMTYPE), 1,originfile);	
	fread(&leaf, sizeof(int), 1,originfile );	
	fread(&maxlenth, sizeof(ELEMTYPE), 1,originfile);
	fread(&minlenth, sizeof(ELEMTYPE), 1,originfile);
    fread(&codes,sizeof(int),1,originfile);
    fread(&origlenth,sizeof(ELEMTYPE),1,originfile);
    //printf("%d %u ",lenth,lastlenth);
    //printf("%d %u %u %d %u\n",leaf,maxlenth,minlenth,codes,origlenth);

    Q=(queue)malloc(sizeof(QNode));//初始化
    InitQueue(Q);

    words=(ELEMTYPE*)malloc(leaf*sizeof(ELEMTYPE));
    wordslenth=(ELEMTYPE*)malloc(leaf*sizeof(ELEMTYPE));
    content=(char*)malloc(2+maxlenth*sizeof(char));

    for ( i = 0; i < leaf; i++)
    {
        fread(&words[i],sizeof(ELEMTYPE),1,originfile);
        fread(&wordslenth[i],sizeof(ELEMTYPE),1,originfile);
        //printf(" %u %u \n",words[i],wordslenth[i]);
    }//读出每个字符以及它们对应的编码长度
    List=(char**)malloc((leaf) *sizeof(char*));

    codes--;//最后一个编码可能是补齐的，特殊处理
    for (i =j= 0; i < codes;i++){
        ch=fgetc(originfile);
        //printf(" %u ",ch);
        ReadCode(Q,ch);
        while (Q->lenth>maxlenth){
            text=List[j]=(char*)malloc(1+wordslenth[j]);
            for (ch = 0; ch < wordslenth[j]; ch++) {
                DeQueue(Q,&c);
                *text++ = c;
            }
            *text = '\0';
            j++;
        }
    }

    ch=fgetc(originfile);
    //printf(" %u ",ch);
    ReadCode(Q,ch);

    while (Q->lenth>(8-origlenth))
    {
        text=List[j]=(char*)malloc(1+wordslenth[j]);
        for (ch = 0; ch < wordslenth[j]; ch++) {
                DeQueue(Q,&c);
                *text++ = c;
            }
        *text = '\0';
        j++;
    }
    
    if(codes==255)
		List[SIZE-1] = "00000000";
    InitQueue(Q);
    lenth--;//减去最后的字符
    minlenth--;
    while (curr_lenth<lenth){
        ch=fgetc(originfile);//printf(" %u ",ch);
        ReadCode(Q,ch);
        curr_lenth++;
        while (Q->lenth>maxlenth) {
            for ( i = 0; i < minlenth; i++)
            {
                DeQueue(Q,&c);
                content[i]=c;
            }//出来一个不完整字符编码，minlenth减1保证下一个for循环可以正常进行
			for (; i < maxlenth;i++) {
                DeQueue(Q,&c);
                content[i] = c;
                content[i+1] = '\0';
                for(j=0;j<leaf;j++){
                    if (i+1 == wordslenth[j] && 0 == strcmp(List[j],content)){
                        ch = words[j];
                        //printf(" %u ",ch);
                        fputc(ch,newfile);//判断是否是字符
                        break;
                    }
                }
                if(j<leaf)
                    break;
            }
        }
    }

    ch=fgetc(originfile);
    //printf(" %u ",ch);
    ReadCode(Q,ch);

    while (Q->lenth>(8-lastlenth))
    {
        for ( i = 0; i < minlenth; i++)
            {
                DeQueue(Q,&c);
                content[i]=c;
            }
		for (; i < maxlenth; i++) {
                DeQueue(Q,&c);
                content[i] = c;
                content[i+1] = '\0';
                for(j=0;j<leaf;j++){
                    if (i+1 == wordslenth[j] && 0 == strcmp(List[j],content)){
                        //printf(" %u ",ch);
                        ch=words[j];
						fputc(ch, newfile);
                        break;
                    }
                }
                if(j<leaf)
                    break;
            }
    }//特殊处理
    
    free(Q);
    free(words);
    free(wordslenth);
    free(List);
    fclose(originfile);
    fclose(newfile);

} //解压缩操作

int main() {
    int sw;

    printf("         WELCOME TO USING MY PROGRAM            \n");
    while (1)
    {
        printf("         PLEACE TELL ME ABOUT YOUR CHOICE             \n");
        printf("         1:              COMPRASS                     \n");
        printf("         2:            UNCOMPRASS                     \n");
        printf("         else:                EXIT                       \n");
        printf("          NOW YOU COULD MAKE YOUR CHOICE              \n");

        scanf("%d",&sw);

        switch (sw)
        {
        case 1:
            compress();
            break;
        
        case 2:
            uncompress();
            break;

        default:
            return 0;
        }
    }

    return 0;
}