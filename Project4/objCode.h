#ifndef __OBJCODE__
#define __OBJCODE__

#include "interCode.h"
#define REG_SIZE 32
typedef struct reg REG;
typedef struct _stack stack;
typedef struct stackstack ss;

struct reg
{
	int id;
	int inusing;
	operand *op;
};
extern REG r[REG_SIZE];
struct _stack
{
	char val[64];
	int ifarg;
	stack *next;
	stack *pre;
};
struct stackstack
{
	stack *head;
	ss *next;
};
extern ss *sshead;
void sspush();
void stackpush(char *val, int ifarg);
int stackfind(char *val);
void deletestack();


int reg(operand *op);
void initreg();
void writeAsmHead(char *filename);
void inter2asm(char *filename);
void labelop(int labelid, FILE *fp);
void funcop(char *funcname, FILE *fp);
void li(int dstreg, int value, FILE *fp);
void move(int dstreg, int opreg, FILE *fp);
void addi(int dstreg, int op1reg, int imm, FILE *fp);
void add(int dstreg, int op1reg, int op2reg, FILE *fp);
void sub(int dstreg, int op1reg, int op2reg, FILE *fp);
void mul(int dstreg, int op1reg, int op2reg, FILE *fp);
void mdiv(int dstreg, int op1reg, int op2reg, FILE *fp);
void lw(int dstreg, int opreg, int addrbase, FILE *fp);
void sw(int dstreg, int opreg, int addrbase, FILE *fp);
void j(int labelid, FILE *fp);

void callop(int dstreg, char *funcname,int argcount, FILE *fp);
void retop(int dstreg, FILE *fp);
void beq(int op1reg, int op2reg, int labelid, FILE *fp);
void bne(int op1reg, int op2reg, int labelid, FILE *fp);
void bgt(int op1reg, int op2reg, int labelid, FILE *fp);
void blt(int op1reg, int op2reg, int labelid, FILE *fp);
void bge(int op1reg, int op2reg, int labelid, FILE *fp);
void ble(int op1reg, int op2reg, int labelid, FILE *fp);


#endif