.orig x3000
LEA R2,#14;
AND R0,R0,#0;
JSR #1;0或者1
TRAP x25;
STR R7,R2,#0;
ADD R2,R2,#1;1或者#9
ADD R0,R0,#1;
LD R1,#17;
ADD R1,R1,#-1;R5或者R1
ST R1,#15;
BRz #1;
JSR #-8;
ADD R2,R2,#-1;
LDR R7,R2,#0;LDR R7,R2,#0或者JSR x180
RET;
.FILL x0;
.FILL x0;
.FILL x0;
.FILL x0;
.FILL x0;
.FILL x0;
.FILL x0;
.FILL x0;
.FILL x0;
.FILL x0;
.FILL x5;
.end