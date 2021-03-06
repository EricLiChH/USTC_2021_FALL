.ORIG x3000
LD R1 NUMB;
LD R2 NUMA;
JSR JUDGE;
HALT

JUDGE ST R7,ST7;
DECIDE JSR SQR;
NOT R4,R4;
ADD R4,R4,#1;
ADD R4,R4,R0;
BRn RETURN2;
JSR MOD;
ADD R4,R4,#0;
BRp PLUS;
AND R1,R1,#0;
BRnzp RETURN2;
PLUS ADD R2,R2,#1;
BRnzp DECIDE;
RETURN2 LD R7,ST7;
RET;

SQR AND R4,R4,#0;
ADD R3,R2,#0;
BRz RETURN1;
LOOP ADD R4,R2,R4;
ADD R3,R3,#-1;
BRp LOOP;
RETURN1 RET;

MOD AND R4,R4,#0;
AND R3,R3,#0;
NOT R5,R0;
ADD R5,R5,#1;
OPR ADD R3,R3,R2;
ADD R4,R3,R5;
BRn OPR;
RET;

NUMA .FILL #2;
NUMB .FILL #1;
ST7 .BLKW #1;
.END
