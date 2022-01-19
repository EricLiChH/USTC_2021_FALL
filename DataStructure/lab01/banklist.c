#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>


//设置全局状态参量
#define CLOSETIME 600 //闭店时间为600
#define initmoney 10000//初始银行账目有10000存留



//队列中成员的状态结构
typedef struct Member {
	int status;//status为1时，代表已离开，为0代表正在排队
	int no;//用户编号
	int arrivetime, duration, endtime;//arrive为到达时间,duration为办事所需时间，endtime为完成时间
	int waitingtime ;//等候时间
	int amount;//交易金额
	struct Member* next;
}member,*LinkMember;

//成员队列
typedef struct queue {
	int num;//队列中人数
	LinkMember front;//队首
	LinkMember rear;//队尾
}queue, *Queue;



//全局变量
int TIME;//现在时间，初始是0
int MAXPY;//最长处理时间
int MINPY;//最短处理时间
int AMOUNT;//交易金额
int MONEY;//现存款总额
int MAXMONEY;//最大存取款金额
int MAXLINE;//最大队列数
int NO;//客户编号，第一位客户为1
int MAXINTER;//最大间隔
int MININTER;//最小间隔
LinkMember History;//历史事件表
LinkMember ArriveList;//到达事件表
Queue* QueueLine;//常规业务队列
Queue Bigname;//大额交易者



//下面是队列操作，参考课本进行了一定修改

//预估时间
void TimePredict(Queue Q) {
	if (!Q)
		return;
	else if (Q->num&&Q->front->endtime==601)
		Q->front->endtime = TIME + Q->front->duration;
	return;
}

//判断空队
int EmptyLine(Queue L[]) {
	int flag=1;
	for (int i = 0; i < MAXLINE; i++) {
		if (L[i]->num) {
        flag = 0;
		return flag;
		}	
	}
	return flag;
}

//初始化队列
Queue InitQueue() {
	Queue Q;
	Q = (Queue)malloc(sizeof(queue));
	Q->front = Q->rear = (LinkMember)malloc(sizeof(member));
	if (!Q->front) return;//内存分配失败
	Q->num = 0;
	return Q;
}

//入队操作
void push(Queue q, LinkMember L) {
	if (!L)
		return;
	if (q ->num==0) {
		q->front = L;
		q->rear = L;
		q->front->next = NULL;
	}
	else {
		q->rear->next = L;
		q->rear = L;
		q->rear->next = NULL;
	}
	q->num++;
}

//出队操作
void pop(Queue Q) {
	LinkMember W = Q->front;

	if (Q->num==0)return;//空队列
	else if (Q->front == Q->rear) {
		Q->front = Q->rear = (LinkMember)malloc(sizeof(member));
		Q->num = 0;
		free(W);
		return;
	}//当队列中仅有一个成员或仅仅完成初始化时，置空队列，同时将成员数置为0
	else
		Q->front = Q->front->next;
	free(W);
	Q->num--;
}

//销毁操作，用于结束时清空存储空间
void destroy(Queue Q) {
	LinkMember p, q;
	q = Q->front;
	while (q) {
		p = q->next;
		free(q);
		q = p;
	}
}

//寻找最小队
Queue MinQueue(Queue Q[]) {
	Queue Min;
	int i;
	Min = Q[0];
	for (i = 0; i < MAXLINE; i++) {
		if (Q[i]->num == 0) {
            Min = Q[i];
			break;
		}
		if (Min->num > Q[i]->num)
			Min = Q[i];
	}
	return Min;
}

Queue mintime(Queue Q[]) {
	Queue Min = Q[0];
	for (int i = 0; i < MAXLINE; i++) {
		if (!Q[i]->num)
			continue;
		else{
			if (!Min->num||Min->front->endtime > Q[i]->front->endtime)
				Min = Q[i];
		}
	}
	return Min;
}

//选择各队列里下一时间发生的事件
Queue Timechoice(Queue L[],Queue Big) {
	int i;
	Queue Min=L[0];
	for (i = 0; i < MAXLINE; i++) {
		if (L[i]->num == 0) {
			Min = L[i];
			break;
		}
		if (Min->front->endtime > L[i]->front->endtime)
			Min = L[i];
	}
	if( (Big->front->endtime < Min->front->endtime&&Big->front->endtime))
		Min = Big;
	return Min;
}

//寻找可以解决的大额客户;
int search(Queue L) {
	LinkMember p, q, r;
	p = q = L->front;
	while ((p->amount + MONEY) < 0) {
		L->rear->next = p;
		L->rear = p;
		r = p->next;
		p->next = NULL;
		p = r;
		if (p == q)
			return 0;
	}
	L->front = p;
	return 1;
}

//队伍成员复制
LinkMember Qcopy(LinkMember m) {
	if (!m)
		return;
	LinkMember newM;
	newM = (LinkMember)malloc(sizeof(member));
	newM->status = m->status;
	newM->amount = m->amount;
	newM->arrivetime = m->arrivetime;
	newM->endtime = 601;
	newM->duration = m->duration;
	newM->no = m->no;
	return newM;
}

//以下为对成员表的操作

//取表尾操作
LinkMember GetRear(LinkMember L) {
	LinkMember p ;
	if (!L)
		return L;
	p = L;
	while (p->next)
		p = p->next;
	return p;
}

//成员表移动操作
LinkMember GoList(LinkMember L) {
	LinkMember p, q;
	p = L->next;
	q = L;
	q->next = NULL;
	free(q);
	return p;
}

//复制操作，将某成员结构复制到链表的尾部
void copy(LinkMember m, LinkMember L) {
    LinkMember p = GetRear(L);
	LinkMember newM;
	newM = (LinkMember)malloc(sizeof(member));
	newM->status = m->status;
	newM->amount = m->amount;
	newM->arrivetime = m->arrivetime;
	newM->endtime = m->endtime;
	newM->duration = m->duration;
	newM->no = m->no;
	if (p) {
		p->next = newM;
		p = p->next;
		p->next = NULL;
	}
	else {
        p = newM;
		p->next = NULL;
	}
}



//对于一般客户，我们用solvelist函数解决存取款问题
void solvelist(LinkMember L) {
	L->status = 1;
	MONEY += L->amount;
}//作用效果是改写链表的最新项为完成

//arrive函数是建立事件开始链表的关键函数
LinkMember arrive() {
	LinkMember p,q,r;
	int temptime;
	p = (LinkMember)malloc(sizeof(member));
	p->arrivetime = rand() % (MAXINTER - MININTER + 1);
	p->endtime = 601;
	p->duration = rand() % (MAXPY - MINPY + 1) + MINPY;
	p->no = NO++;
	p->status = 0;
	p->amount = (rand() % (2 * MAXMONEY) - MAXMONEY);//利用rand函数随机记录交易金额
	p->next = NULL;
	q = r = p;
	TIME = p->arrivetime;//记录time的初始时间

	while (r->arrivetime+r->duration < CLOSETIME) {
		temptime = r->arrivetime;
		p = (LinkMember)malloc(sizeof(member));
        p->amount = (rand() % (2 * MAXMONEY) - MAXMONEY);//利用rand函数随机记录交易金额
	    p->no = NO++;
        p->status = 0;
		p->arrivetime = rand() % (MAXINTER - MININTER +1) + temptime;
		p->duration= rand() % (MAXPY - MINPY + 1) + MINPY;
		p->next = NULL;
		r->next = p;
		r = r->next;
	}
	p->next = NULL;
	return q;
}


//这是结束时的打印函数
void PrintList(LinkMember H) {
	int S_Cus=0,IP=0;//分别为完成业务用户，进门用户
	double TotalTime=0;//所有人的用时
	double AverageTime,SPercent;//平均时长
	LinkMember Htemp=H->next;
	while (Htemp) {
		if (!Htemp->status) {
			IP++;
			printf("时间：%3d  用户编号：%3d  进入银行开始排队。\n",Htemp->arrivetime,Htemp->no);
		}
		else {
			S_Cus++;
			Htemp->waitingtime = Htemp->endtime - Htemp->arrivetime;
			TotalTime += Htemp->waitingtime;
			if (Htemp->amount > 0) 
				printf("时间：%3d  用户编号：%3d  完成存（还）款金额为%4d 等待时长为%3d 。\n", Htemp->endtime, Htemp->no, Htemp->amount,Htemp->waitingtime);
			else
				printf("时间：%3d  用户编号：%3d  完成取（借）款金额为%4d 等待时长为%3d 。\n", Htemp->endtime, Htemp->no, (-1)*Htemp->amount,Htemp->waitingtime );
		}
		Htemp = Htemp->next;
	}
	AverageTime = TotalTime / S_Cus;
	SPercent =(double) S_Cus / (double)IP;
	printf("\n时间为%3d  银行下班！！！", CLOSETIME);
	printf("\n\n\n\n\n");
	printf("今日营业共计有%d名客户进店，其中%d名完成业务，完成率为%.5f,平均用时为%2.3f，当日结余金额为%d\n", IP, S_Cus,SPercent,AverageTime,MONEY );
}


int main() {
	printf("请输入最大队列数目:\n");
	scanf("%d", &MAXLINE);
	printf("请设置客户的最大和最小到达间隔:(中间用空格隔开)\n");
	scanf("%d %d",&MAXINTER,&MININTER);
	printf("请设置客户的最大和最小交易时长:(中间用空格隔开)\n");
	scanf("%d %d",&MAXPY,&MINPY);
	printf("请设置交易最大金额:\n");
	scanf("%d",&MAXMONEY);
	

	NO = 1;

	LinkMember p;
	Queue q;
	MONEY=0;
	History = (LinkMember)malloc(sizeof(member));
	LinkMember pHistory = History;
	pHistory->next = NULL;
	QueueLine=(queue**)malloc(MAXLINE*sizeof(queue*));
	ArriveList = arrive();//初始化到达事件表

	for (int i = 0; i < MAXLINE; i++) 
		QueueLine[i]=InitQueue();
	Bigname=InitQueue();
	//初始化队列
	

	while ((ArriveList || !EmptyLine(QueueLine)||Bigname->num)&&TIME<CLOSETIME) {
		//预估时间
		if (!ArriveList&&EmptyLine(QueueLine))
			if (Bigname->front->endtime > CLOSETIME||Bigname->front->amount+MONEY<0||!MONEY)
				break;
		for (int i = 0; i < MAXLINE; i++)
			if(QueueLine[i]->num)
			    TimePredict(QueueLine[i]);
		if (Bigname->num&&search(Bigname)) {
            TimePredict(Bigname);
			q = Timechoice(QueueLine, Bigname);
		}

		else
		    q = mintime(QueueLine);
		//判断下一事件
		//下一时间点在等候队列里
		if ((q->num)&&(!ArriveList ||(q->front->endtime < ArriveList->arrivetime))) {
			p = q->front;
			TIME = p->endtime;
			if (TIME > CLOSETIME)
				break;
			//存款
			if (p->amount > 0) {
				solvelist(p);
				copy(p, History);
				pop(q);
				
			}
			//取款
			else {
				//大客户交易
				if (q == Bigname&&Bigname->num) {
					if (p->amount + MONEY >= 0) {
                        solvelist(p);
					    copy(p, History);
					    pop(q);
					}
						
				}
				//一般交易
				else {
					//是大客户
					if (p->amount + MONEY < 0) {
					    push(Bigname, Qcopy(p));
					    pop(q);
					}
					//一般客户
					else {
						solvelist(p);
						copy(p, History);
						pop(q);
					}
					
				}
			}
		}
		//下一事件发生在到达事件表中
		else {
			if (!ArriveList)
				break;
			TIME = ArriveList->arrivetime;
			if (TIME > CLOSETIME)
				break;
			copy(ArriveList, History);
			push(MinQueue(QueueLine), Qcopy(ArriveList));
			ArriveList=GoList(ArriveList);
		}
	}
	//打印结果
	PrintList(History);

	system("pause");

	return 0;
}