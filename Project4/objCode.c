#include "objCode.h"
#define NOT_IN_STACK 0xffff
char *asmHead[] = { ".data\n",
"_prompt: .asciiz \"Enter an integer:\"\n",
"_ret: .asciiz \"\\n\"\n",
".globl main\n",
".text\n",
"read:\n",
"li $v0, 4\n",
"la $a0, _prompt\n",
"syscall\n",
"li $v0, 5\n",
"syscall\n",
"jr $ra\n\n",
"write:\n",
"li $v0, 1\n",
"syscall\n",
"li $v0, 4\n",
"la $a0, _ret\n",
"syscall\n",
"move $v0, $0\n",
"jr $ra\n", 
"\n"};
char *regs[] = {
	"$zero","$at","$v0","$v1",
	"$a0","$a1","$a2","$a3",
	"$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7",
	"$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7",
	"$t8","$t9","$k0","$k1","$gp","$sp","$s8","$ra"
};
REG r[REG_SIZE];
int reg(operand *op)
{
	for (int i = 4; i < 25; i++)
	{
		if (r[i].op == NULL)
			continue;
		if (op->kind == r[i].op->kind)
		{
			if (op->kind == VAR_OP && !strcmp(op->value, r[i].op->value))
			{
				return i;
			}
			else if (op->kind == TEMP_OP&&op->no == r[i].op->no)
			{
				return i;
			}
		}
	}
	return -1;
}
void labelop(int labelid, FILE *fp)
{
	char command[100];
	sprintf(command, "label%d:\n", labelid);
	fputs(command, fp);
}
void funcop(char *funcname, FILE *fp)
{
	char command[100];
	sprintf(command, "%s:\n", funcname);
	fputs(command, fp);
}

void li(int dstreg, int value, FILE *fp)
{
	char command[100];
	sprintf(command, "li %s, %d\n", regs[dstreg], value);
	fputs(command, fp);
}
void move(int dstreg, int opreg, FILE *fp)
{
	
	char command[100];
	sprintf(command,"move %s, %s\n", regs[dstreg], regs[opreg]);
	
	fputs(command, fp);
}

void addi(int dstreg, int op1reg, int imm, FILE *fp)
{
	char command[100];
	sprintf(command, "addi %s, %s, %d\n", regs[dstreg], regs[op1reg], imm);
	fputs(command, fp);

}
void add(int dstreg, int op1reg, int op2reg, FILE *fp)
{
	char command[100];
	sprintf(command, "add %s, %s, %s\n", regs[dstreg], regs[op1reg], regs[op2reg]);
	fputs(command, fp);
}
void sub(int dstreg, int op1reg, int op2reg, FILE *fp)
{
	char command[100];
	sprintf(command, "sub %s, %s, %s\n", regs[dstreg], regs[op1reg], regs[op2reg]);
	fputs(command, fp);
}
void mul(int dstreg, int op1reg, int op2reg, FILE *fp)
{
	char command[100];
	sprintf(command, "mul %s, %s, %s\n", regs[dstreg], regs[op1reg], regs[op2reg]);
	fputs(command, fp);
}
void mdiv(int dstreg, int op1reg, int op2reg, FILE *fp)
{
	char command1[100];
	sprintf(command1, "div %s, %s\n", regs[op1reg], regs[op2reg]);
	fputs(command1, fp);
	char command2[100];
	sprintf(command2, "mflo %s\n", regs[dstreg]);
	fputs(command2, fp);
}

void lw(int dstreg, int opreg, int addrbase, FILE *fp)
{
	char command[100];
	sprintf(command, "lw %s, %d(%s)\n", regs[dstreg], addrbase, regs[opreg]);
	fputs(command, fp);
}
void sw(int dstreg, int opreg, int addrbase, FILE *fp)
{
	char command[100];
	sprintf(command, "sw %s, %d(%s)\n", regs[opreg], addrbase, regs[dstreg]);
	fputs(command, fp);
}
void j(int labelid, FILE *fp)
{
	char command[100];
	sprintf(command, "j label%d\n", labelid);
	fputs(command, fp);
}
void callop(int dstreg, char *funcname, int argcount, FILE *fp)
{
	fputs("addi $sp, $sp, -4\n", fp);
	fputs("sw $ra, 0($sp)\n",fp);
	char command[100];
	sprintf(command, "jal %s\n", funcname);
	fputs(command, fp);
	fputs("lw $ra, 0($sp)\n", fp);
	fputs("addi $sp, $sp, 4\n", fp);
	addi(29, 29, 4 * argcount, fp);
	move(dstreg, 2, fp);
}
void retop(int dstreg, FILE *fp)
{
	move(2, dstreg, fp);
	fputs("jr $ra\n", fp);
}

void beq(int op1reg, int op2reg, int labelid, FILE *fp)
{
	char command[100];
	sprintf(command, "beq %s, %s, label%d\n", regs[op1reg], regs[op2reg], labelid);
	fputs(command, fp);
}

void bne(int op1reg, int op2reg, int labelid, FILE *fp)
{
	char command[100];
	sprintf(command, "bne %s, %s, label%d\n", regs[op1reg], regs[op2reg], labelid);
	fputs(command, fp);
}
void bgt(int op1reg, int op2reg, int labelid, FILE *fp)
{
	char command[100];
	sprintf(command, "bgt %s, %s, label%d\n", regs[op1reg], regs[op2reg], labelid);
	fputs(command, fp);
}
void blt(int op1reg, int op2reg, int labelid, FILE *fp)
{
	char command[100];
	sprintf(command, "blt %s, %s, label%d\n", regs[op1reg], regs[op2reg], labelid);
	fputs(command, fp);
}
void bge(int op1reg, int op2reg, int labelid, FILE *fp)
{
	char command[100];
	sprintf(command, "bge %s, %s, label%d\n", regs[op1reg], regs[op2reg], labelid);
	fputs(command, fp);
}
void ble(int op1reg, int op2reg, int labelid, FILE *fp)
{
	char command[100];
	sprintf(command, "ble %s, %s, label%d\n", regs[op1reg], regs[op2reg], labelid);
	fputs(command, fp);
}

void writeAsmHead(char *filename)
{
	FILE *fp = NULL;
	fp = fopen(filename, "w");
	if (fp == NULL)
	{
		printf("open file %s failed\n",filename);
		return;
	}
	for (int i = 0; asmHead[i][0] != '\n'; i++)
	{
		fputs(asmHead[i], fp);
	}
	fputs("\n", fp);
	fclose(fp);
}

void initreg()
{
	for (int i = 0; i < REG_SIZE; i++)
	{
		memset(&r[i], 0, sizeof(REG));
		r[i].id = i;
	}
}
int lastreg = 8;
#define NEXT_LAST_REG (lastreg+1)%18+8
int voidreg()
{
	for (int i = 8; i < 25; i++)
	{
		if (r[i].inusing == 0)
			return i;
	}
	return -1;
}
void printstack()
{
	stack *head = sshead->head;
	while (head != NULL)
	{
		printf("%s->", head->val);
		head = head->next;
	}
	printf("NULL\n");
}

int getreg(operand *op, int i,FILE *fp)
{
	printstack();
	if (op->kind == CONST_OP)//该操作数为常量
	{
		li(11, op->no, fp);
		return 11;
	}
	if (r[i].inusing == 0|| r[i].op == NULL||r[i].op->kind == CONST_OP)
	{
		r[i].op = op;
		r[i].inusing = 1;
		char varname2[20];
		if (op->kind == TEMP_OP)
		{
			sprintf(varname2, "t%d", op->no);
		}
		else
		{
			sprintf(varname2, "%s", op->value);
		}
		int retstack2 = stackfind(varname2);
		if (retstack2 != NOT_IN_STACK)
		{
			lw(i, 30, -4 * retstack2, fp);
		}
	}
	else
	{
		char varname[20];
		if (r[i].op->kind == TEMP_OP)
		{
			sprintf(varname, "t%d", r[i].op->no);
		}
		else
		{
			sprintf(varname, "%s", r[i].op->value);
		}
		int retstack = stackfind(varname);
		if (retstack == NOT_IN_STACK)
		{
			addi(29, 29, -4, fp);
			sw(29, i, 0, fp);
			stackpush(varname, 0);
		}
		else
		{
			sw(29, i, retstack * 4, fp);
		}
		r[i].op = op;
		char varname2[20];
		if (op->kind == TEMP_OP)
		{
			sprintf(varname2, "t%d", op->no);
		}
		else
		{
			sprintf(varname2, "%s", op->value);
		}
		int retstack2 = stackfind(varname2);
		if (retstack2 != NOT_IN_STACK)
		{
			lw(i, 30, -4 * retstack2, fp);
		}
	}
	return i;
}

stack *retaddress = NULL;
void inter2asm(char *filename)
{
	FILE *fp = NULL;
	fp = fopen(filename, "a");
	if (fp == NULL)
	{
		printf("open file %s failed\n", filename);
		return;
	}
	int argcount = 0;
	code *troot = codeRoot;
	for (; troot != NULL; troot = troot->next)
	{
		if (troot->kind == LABEL_CODE)
		{
			labelop(troot->detail.singleop.op->no, fp);
		}
		else if (troot->kind == FUNCTION_CODE)
		{
			deletestack();
			fputs("\n", fp);
			funcop(troot->detail.singleop.op->value, fp);
			sspush();
			stackpush("return", 0);
			retaddress = sshead->head;
			move(30, 29, fp);
			
		}
		else if (troot->kind == ASSIGN_CODE)
		{
			int dstregid = getreg(troot->detail.assign.left,8,fp);
			
			int srcregid = getreg(troot->detail.assign.right,9,fp);
			move(dstregid, srcregid, fp);
		}
		else if (troot->kind == ADD_CODE)
		{
			int dstregid = getreg(troot->detail.doubleop.result, 8,fp);
			int op1regid = getreg(troot->detail.doubleop.op1, 9,fp);
			int op2regid = getreg(troot->detail.doubleop.op2, 10,fp);
			add(dstregid, op1regid, op2regid, fp);
		}
		else if (troot->kind == MINUS_CODE)
		{
			int dstregid = getreg(troot->detail.doubleop.result,8, fp);
			int op1regid = getreg(troot->detail.doubleop.op1, 9,fp);
			int op2regid = getreg(troot->detail.doubleop.op2, 10,fp);
			sub(dstregid, op1regid, op2regid, fp);
		}
		else if (troot->kind == MUL_CODE)
		{
			int dstregid = getreg(troot->detail.doubleop.result,8, fp);
			int op1regid = getreg(troot->detail.doubleop.op1, 9,fp);
			int op2regid = getreg(troot->detail.doubleop.op2,10, fp);
			mul(dstregid, op1regid, op2regid, fp);
		}
		else if (troot->kind == DIV_CODE)
		{
			int dstregid = getreg(troot->detail.doubleop.result,8, fp);
			int op1regid = getreg(troot->detail.doubleop.op1,9, fp);
			int op2regid = getreg(troot->detail.doubleop.op2,10, fp);
			mdiv(dstregid, op1regid, op2regid, fp);
		}
		else if (troot->kind == GOTO_CODE)
		{
			j(troot->detail.singleop.op->no,fp);
		}
		else if (troot->kind == IF_GOTO_CODE)
		{
			int op1regid = getreg(troot->detail.tribleop.x, 8,fp);
			int op2regid = getreg(troot->detail.tribleop.y,9, fp);
			if (!strcmp(troot->relop, "=="))
			{
				beq(op1regid, op2regid, troot->detail.tribleop.gotoLabel->no, fp);
			}
			else if (!strcmp(troot->relop, "!="))
			{
				bne(op1regid, op2regid, troot->detail.tribleop.gotoLabel->no, fp);
			}
			else if (!strcmp(troot->relop, ">="))
			{
				bge(op1regid, op2regid, troot->detail.tribleop.gotoLabel->no, fp);
			}
			else if (!strcmp(troot->relop, "<="))
			{
				ble(op1regid, op2regid, troot->detail.tribleop.gotoLabel->no, fp);
			}
			else if (!strcmp(troot->relop, ">"))
			{
				bgt(op1regid, op2regid, troot->detail.tribleop.gotoLabel->no, fp);
			}
			else if (!strcmp(troot->relop, "<"))
			{
				blt(op1regid, op2regid, troot->detail.tribleop.gotoLabel->no, fp);
			}
		}
		else if(troot->kind == RETURN_CODE)
		{
			int dstreg = getreg(troot->detail.singleop.op, 8,fp);
			move(29, 30, fp);
			retop(dstreg, fp);
			argcount = 0;
		}
		else if (troot->kind == ARG_CODE)
		{
			int opregid = getreg(troot->detail.singleop.op,8,fp);
			addi(29, 29, -4, fp);
			sw(29, opregid, 0, fp);
			
			if (argcount < 4)
			{
				move(argcount + 4, opregid, fp);
				argcount++;
			}
		}
		else if (troot->kind == CALL_CODE)
		{
			int dstregid = getreg(troot->detail.assign.left,8, fp);
			callop(dstregid, troot->detail.assign.right->value, argcount, fp);
		}
		else if (troot->kind == PARAM_CODE)
		{
			stack *tempstack = (stack*)malloc(sizeof(stack));
			tempstack->next = NULL;
			tempstack->pre = retaddress;
			strcpy(tempstack->val, troot->detail.singleop.op->value);
			tempstack->ifarg = 1;
			retaddress->next = tempstack;
			retaddress = tempstack;
		}
		else if (troot->kind == READ_CODE)
		{
			
			int retreg = getreg(troot->detail.singleop.op, 8,fp);
			fputs("addi $sp, $sp, -4\n", fp);
			fputs("sw $ra, 0($sp)\n", fp);
			fputs("jal read\n",fp);
			char command[100];
			sprintf(command,"move %s, $v0\n", regs[retreg]);
			fputs(command, fp);
			fputs("lw $ra, 0($sp)\n", fp);
			fputs("addi $sp, $sp, 4\n", fp);
			
		}
		else if (troot->kind == WRITE_CODE)
		{
			int retreg = getreg(troot->detail.singleop.op, 8,fp);
			char command[100];
			sprintf(command, "move $a0, %s\n", regs[retreg]);
			fputs(command, fp);
			fputs("addi $sp, $sp, -4\n", fp);
			fputs("sw $ra, 0($sp)\n", fp);
			fputs("jal write\n", fp);
			fputs("lw $ra, 0($sp)\n", fp);
			fputs("addi $sp, $sp, 4\n", fp);
		}
	}
	fclose(fp);
}
ss *sshead = NULL;
void sspush()
{
	ss *temp = (ss*)malloc(sizeof(ss));
	temp->next = sshead;
	temp->head = NULL;
	sshead = temp;
}
void stackpush(char *val, int ifarg)
{
	stack *temp = (stack*)malloc(sizeof(stack));
	temp->ifarg = ifarg;
	temp->next = NULL;
	temp->pre = NULL;
	temp->next = sshead->head;
	strcpy(temp->val, val);
	if(sshead->head!=NULL)
		sshead->head->pre = temp;
	sshead->head = temp;
	
}

int stackfind(char *val)
{
	int count = 0;
	stack *head = sshead->head;
	if (head == NULL)
		return NOT_IN_STACK;
	for (; strcmp("return",head->val)!=0; head = head->next);
	for (; head != NULL; head = head->pre)
	{
		if (!strcmp(val, head->val))
		{
			return count - 1;
		}
		count++;
	}
	return NOT_IN_STACK;
}
void deletestack()
{
	if (sshead == NULL)
		return;
	stack *temp1 = sshead->head;
	while (temp1 != NULL)
	{
		
		stack *temp2 = temp1;
		temp1 = temp1->next;
		free(temp2);
		
	}
	
	sshead = sshead->next;
	free(temp1);
}