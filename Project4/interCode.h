#ifndef __INTERCODE__
#define __INTERCODE__

#include "stdio.h"
#include "Node.h"

#define VAR_OP 0x101
#define FUNC_OP 0x102
#define LABEL_OP 0x103
#define CONST_OP 0x104
#define TEMP_OP 0x105
#define ADDR_OP 0x106

#define LABEL_CODE 0x1
#define FUNCTION_CODE 0x2
#define ASSIGN_CODE 0x3
#define ADD_CODE 0x4
#define MINUS_CODE 0x5
#define MUL_CODE 0x6
#define DIV_CODE 0x7

#define ADDRESS_CODE 0x8

#define GOTO_CODE 0xb
#define IF_GOTO_CODE 0xc
#define RETURN_CODE 0xd
#define DEC_CODE 0xe
#define ARG_CODE 0xf
#define CALL_CODE 0x10
#define PARAM_CODE 0x11
#define READ_CODE 0x12
#define WRITE_CODE 0x13

typedef struct _operand operand;
typedef struct _code code;

struct _operand
{
	int kind;
	union
	{
		//LABEL_OP CONST_OP
		int no;
		//VAR_OP FUNC_OP
		char *value;
		//ADDR_OP
		operand *addr;
	};
	//参数列表中的下一个参数
	operand *nextArg;
};

struct _code
{
	int kind;
	union 
	{
		struct 
		{
			operand *left;
			operand *right;
			operand *useless;
		}assign;

		struct
		{
			operand *useless1;
			operand *op;
			operand *useless2;
		}singleop;

		struct
		{
			operand *result;
			operand *op1;
			operand *op2;
		}doubleop;

		struct
		{
			operand *gotoLabel;
			operand *x;
			operand *y;
		}tribleop;

		struct
		{
			operand *op;
			int size;
		}dec;
	}detail;
	char *relop;
	code *pre;
	code *next;
};

extern code *codeRoot;
extern code *codeTail;

void printcode(char *outfilename);
void insertCode(code *p);
void translate_Basic_Exp(Node *root, operand *place);
void translate_Args(Node *root, operand **arglist);
void translate_Stmt(Node *root);
void translate_Cond(Node *root, operand *label_true, operand *label_false);
operand *new_temp();
void translate_CompSt(Node *root);
void translate_DefList(Node *root);
void translate_Def(Node *root);
void translate_DecList(Node *root);
void translate_StmtList(Node *root);
void translate_Dec(Node *root);

#endif

