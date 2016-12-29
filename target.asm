DSEG    SEGMENT  
g       DW 0
a       DW 0
d       DW 0
i       DW 0
total   DW 0
q       DW 0
r       DW 0
s       DW 0
lo      DW 0
y       DB 0
k       DW 0
t:      DW 0
t9      DW 0
t8      DW 0
t7      DW 0
t6      DB 0
t5      DB 0
t4      DW 0
t3      DB 0
t2      DW 0
t1      DW 0
string  DB 'PLEASE INPUT:','$'
cnt     DW 1000 DUP(0)
float   DD 1000 DUP(0)
bool    DB 1000 DUP(0)
char    DB 1000 DUP(0)
DSEG    ENDS
SSEG    SEGMENT STACK
SSEG    ENDS
CSEG    SEGMENT
        ASSUME  CS:CSEG,DS:DSEG
        ASSUME  SS:SSEG
main:
        MOV AX,DSEG
        MOV DS,AX
        MOV AX,SSEG
        MOV SS,AX
        MOV a,000bH
        MOV d,0002H
        MOV total,0000H
        MOV y,'a'
        MOV k,0001H
        MOV DX,OFFSET string
        MOV AH,09H
        INT 21H
        MOV AH,01
        INT 21H
        SUB AL,30H
        AND AX,00FFH
        MOV i,AX
        MOV DL,0AH
        MOV AH,02H
        INT 21H
        MOV DL,0dH
        INT 21H
        MOV r,0002H
        MOV SI,OFFSET d
        MOV AL,BYTE PTR [SI]
        MOV SI,OFFSET a
        MUL BYTE PTR [SI]
        MOV t9,AX
        MOV AX,t9
        MOV s,AX
        MOV AX,a
        MOV SI,OFFSET d
        DIV BYTE PTR [SI]
        AND AH,00H
        MOV t8,AX
        MOV AX,t8
        MOV lo,AX
        MOV AX,t8
        MOV s,AX
compare0:   
        MOV AX,d
        MOV BX,a
        CMP AX,BX
        JBE else0
if0:
        MOV AX,a
        MOV q,AX
        MOV AX,d
        MOV a,AX
        MOV AX,q
        MOV d,AX
        JMP ifend0
else0:
        MOV AX,d
        MOV q,AX
        MOV AX,a
        MOV d,AX
        MOV AX,q
        MOV a,AX
ifend0:
compare1:   
        MOV AX,d
        MOV BX,a
        CMP AX,BX
        JA whileend0
do0:
        MOV AX,0001H
        ADD AX,a
        MOV t4,AX
        MOV AX,t4
        MOV d,AX
        JMP compare1
whileend0:
        MOV CL,16
show0:
        SUB CL,04
        MOV AX,total
        SHR AX,CL
        AND AX,000FH
        MOV DL,AL
        CMP DL,10
        JB  hex0
        ADD DL,07H
hex0:
        ADD DL,30H
        MOV AH,02H
        INT 21H
        CMP CL,0
        JNE show0
        MOV DL,'H'
        MOV AH,02H
        INT 21H
        MOV DL,0AH
        MOV AH,02H
        INT 21H
        MOV DL,0dH
        INT 21H
compare2:   
        MOV AX,0064H
        MOV BX,i
        CMP AX,BX
        JB whileend1
do1:
        MOV AX,i
        ADD AX,total
        MOV t2,AX
        MOV AX,t2
        MOV total,AX
        MOV AX,0001H
        ADD AX,i
        MOV t1,AX
        MOV AX,t1
        MOV i,AX
        JMP compare2
whileend1:
        MOV CL,16
show1:
        SUB CL,04
        MOV AX,total
        SHR AX,CL
        AND AX,000FH
        MOV DL,AL
        CMP DL,10
        JB  hex1
        ADD DL,07H
hex1:
        ADD DL,30H
        MOV AH,02H
        INT 21H
        CMP CL,0
        JNE show1
        MOV DL,'H'
        MOV AH,02H
        INT 21H
        MOV DL,0AH
        MOV AH,02H
        INT 21H
        MOV DL,0dH
        INT 21H
        MOV i,0000H
CSEG    ENDS
END     main
