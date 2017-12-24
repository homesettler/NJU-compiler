#include "interCode.h"
#include "Node.h"
#include "newHashTable.h"
#define INSERTCODEDEBUG
code *codeRoot = NULL;
code *codeTail = NULL;
int temp = 0;
int labeln = 0;
void printcode(char *outfilename)
{
	
	FILE *fp = NULL;
	fp = fopen(outfilename, "wt");
	code *troot = codeRoot;
	while (troot != NULL)
	{
		//printf("troot->value = 0x%x\n", troot->kind);
		if (troot->kind == FUNCTION_CODE)
		{
			fprintf(fp, "FUNCTION %s :\n", troot->detail.singleop.op->value);
#ifdef INSERTCODEDEBUG
			printf("FUNCTION %s :\n", troot->detail.singleop.op->value);
#endif
		}
		else if (troot->kind == PARAM_CODE)
		{
			fprintf(fp, "PARAM %s\n", troot->detail.singleop.op->value);
#ifdef INSERTCODEDEBUG
			printf("PARAM %s\n", troot->detail.singleop.op->value);
#endif
		}
		else if (troot->kind == LABEL_CODE)
		{
			fprintf(fp, "LABEL label%d :\n", troot->detail.singleop.op->no);
#ifdef INSERTCODEDEBUG
			printf("LABEL label%d :\n", troot->detail.singleop.op->no);
#endif
		}
		else if (troot->kind == ASSIGN_CODE)
		{
			if (troot->detail.assign.left == NULL)
			{
				troot = troot->next;
				continue;
			}
			if (troot->detail.assign.left->kind == TEMP_OP)
			{
				fprintf(fp, "t%d := ", troot->detail.assign.left->no);
#ifdef INSERTCODEDEBUG
				printf("t%d := ", troot->detail.assign.left->no);
#endif
			}
			else
			{
				fprintf(fp, "%s := ", troot->detail.assign.left->value);
#ifdef INSERTCODEDEBUG
				printf("%s := ", troot->detail.assign.left->value);
#endif
			}
			if (troot->detail.assign.right->kind == TEMP_OP)
			{
				fprintf(fp, "t%d\n", troot->detail.assign.right->no);
#ifdef INSERTCODEDEBUG
				printf("t%d\n", troot->detail.assign.right->no);
#endif
			}
			else if(troot->detail.assign.right->kind == CONST_OP)
			{
				fprintf(fp, "#%d\n", troot->detail.assign.right->no);
#ifdef INSERTCODEDEBUG
				printf("#%d\n", troot->detail.assign.right->no);
#endif
			}
			else
			{
				fprintf(fp, "%s\n", troot->detail.assign.right->value);
#ifdef INSERTCODEDEBUG
				printf("%s\n", troot->detail.assign.right->value);
#endif
			}
			
		}
		else if (troot->kind == ADD_CODE)
		{
			if (troot->detail.doubleop.result->kind == TEMP_OP)
			{
				fprintf(fp, "t%d := ", troot->detail.doubleop.result->no);
#ifdef INSERTCODEDEBUG
				printf("t%d := ", troot->detail.doubleop.result->no);
#endif
			}
			else
			{
				fprintf(fp, "%s := ", troot->detail.doubleop.result->value);
#ifdef INSERTCODEDEBUG
				printf("%s := ", troot->detail.doubleop.result->value);
#endif
			}
			if (troot->detail.doubleop.op1->kind == TEMP_OP)
			{
				fprintf(fp, "t%d + ", troot->detail.doubleop.op1->no);
#ifdef INSERTCODEDEBUG
				printf("t%d + ", troot->detail.doubleop.op1->no);
#endif
			}
			else if (troot->detail.doubleop.op1->kind == CONST_OP)
			{
				fprintf(fp, "#%d + ", troot->detail.doubleop.op1->no);
#ifdef INSERTCODEDEBUG
				printf("#%d + ", troot->detail.doubleop.op1->no);
#endif
			}
			else
			{
				fprintf(fp, "%s + ", troot->detail.doubleop.op1->value);
#ifdef INSERTCODEDEBUG
				printf("%s + ", troot->detail.doubleop.op1->value);
#endif
			}
			if (troot->detail.doubleop.op2->kind == TEMP_OP)
			{
				fprintf(fp, "t%d\n", troot->detail.doubleop.op2->no);
#ifdef INSERTCODEDEBUG
				printf("t%d\n", troot->detail.doubleop.op2->no);
#endif
			}
			else if (troot->detail.doubleop.op2->kind == CONST_OP)
			{
				fprintf(fp, "#%d\n", troot->detail.doubleop.op2->no);
#ifdef INSERTCODEDEBUG
				printf("#%d\n", troot->detail.doubleop.op2->no);
#endif
			}
			else
			{
				fprintf(fp, "%s\n", troot->detail.doubleop.op2->value);
#ifdef INSERTCODEDEBUG
				printf("%s\n", troot->detail.doubleop.op2->value);
#endif
			}
		}
		else if (troot->kind == MINUS_CODE)
		{
			if (troot->detail.doubleop.result->kind == TEMP_OP)
			{
				fprintf(fp, "t%d := ", troot->detail.doubleop.result->no);
#ifdef INSERTCODEDEBUG
				printf("t%d := ", troot->detail.doubleop.result->no);
#endif
			}
			else
			{
				fprintf(fp, "%s := ", troot->detail.doubleop.result->value);
#ifdef INSERTCODEDEBUG
				printf("%s := ", troot->detail.doubleop.result->value);
#endif
			}
			if (troot->detail.doubleop.op1->kind == TEMP_OP)
			{
				fprintf(fp, "t%d - ", troot->detail.doubleop.op1->no);
#ifdef INSERTCODEDEBUG
				printf("t%d - ", troot->detail.doubleop.op1->no);
#endif
			}
			else if (troot->detail.doubleop.op1->kind == CONST_OP)
			{
				fprintf(fp, "#%d - ", troot->detail.doubleop.op1->no);
#ifdef INSERTCODEDEBUG
				printf("#%d - ", troot->detail.doubleop.op1->no);
#endif
			}
			else
			{
				fprintf(fp, "%s - ", troot->detail.doubleop.op1->value);
#ifdef INSERTCODEDEBUG
				printf("%s - ", troot->detail.doubleop.op1->value);
#endif
			}
			if (troot->detail.doubleop.op2->kind == TEMP_OP)
			{
				fprintf(fp, "t%d\n", troot->detail.doubleop.op2->no);
#ifdef INSERTCODEDEBUG
				printf("t%d\n", troot->detail.doubleop.op2->no);
#endif
			}
			else if (troot->detail.doubleop.op2->kind == CONST_OP)
			{
				fprintf(fp, "#%d\n", troot->detail.doubleop.op2->no);
#ifdef INSERTCODEDEBUG
				printf("#%d\n", troot->detail.doubleop.op2->no);
#endif
			}
			else
			{
				fprintf(fp, "%s\n", troot->detail.doubleop.op2->value);
#ifdef INSERTCODEDEBUG
				printf("%s\n", troot->detail.doubleop.op2->value);
#endif
			}
		}
		else if (troot->kind == MUL_CODE)
		{
			if (troot->detail.doubleop.result->kind == TEMP_OP)
			{
				fprintf(fp, "t%d := ", troot->detail.doubleop.result->no);
			}
			else
			{
				fprintf(fp, "%s := ", troot->detail.doubleop.result->value);
			}
			if (troot->detail.doubleop.op1->kind == TEMP_OP)
			{
				fprintf(fp, "t%d * ", troot->detail.doubleop.op1->no);
			}
			else if (troot->detail.doubleop.op1->kind == CONST_OP)
			{
				fprintf(fp, "#%d * ", troot->detail.doubleop.op1->no);
			}
			else
			{
				fprintf(fp, "%s * ", troot->detail.doubleop.op1->value);
			}
			if (troot->detail.doubleop.op2->kind == TEMP_OP)
			{
				fprintf(fp, "t%d\n", troot->detail.doubleop.op2->no);
			}
			else if (troot->detail.doubleop.op2->kind == CONST_OP)
			{
				fprintf(fp, "#%d\n", troot->detail.doubleop.op2->no);
			}
			else
			{
				fprintf(fp, "%s\n", troot->detail.doubleop.op2->value);
			}
		}
		else if (troot->kind == DIV_CODE)
		{
			if (troot->detail.doubleop.result->kind == TEMP_OP)
			{
				fprintf(fp, "t%d := ", troot->detail.doubleop.result->no);
			}
			else
			{
				fprintf(fp, "%s := ", troot->detail.doubleop.result->value);
			}
			if (troot->detail.doubleop.op1->kind == TEMP_OP)
			{
				fprintf(fp, "t%d / ", troot->detail.doubleop.op1->no);
			}
			else if (troot->detail.doubleop.op1->kind == CONST_OP)
			{
				fprintf(fp, "#%d / ", troot->detail.doubleop.op1->no);
			}
			else
			{
				fprintf(fp, "%s / ", troot->detail.doubleop.op1->value);
			}
			if (troot->detail.doubleop.op2->kind == TEMP_OP)
			{
				fprintf(fp, "t%d\n", troot->detail.doubleop.op2->no);
			}
			else if (troot->detail.doubleop.op2->kind == CONST_OP)
			{
				fprintf(fp, "#%d\n", troot->detail.doubleop.op2->no);
			}
			else
			{
				fprintf(fp, "%s\n", troot->detail.doubleop.op2->value);
			}
		}
		else if (troot->kind == GOTO_CODE)
		{
			fprintf(fp,"GOTO label%d\n", troot->detail.singleop.op->no);
#ifdef INSERTCODEDEBUG
			printf("GOTO label%d\n", troot->detail.singleop.op->no);
#endif
		}
		else if (troot->kind == IF_GOTO_CODE)
		{
			fprintf(fp,"IF ");
#ifdef INSERTCODEDEBUG
			printf("IF ");
#endif
			if (troot->detail.tribleop.x->kind == TEMP_OP)
			{
				fprintf(fp, "t%d ", troot->detail.tribleop.x->no);
#ifdef INSERTCODEDEBUG
				printf("t%d ", troot->detail.tribleop.x->no);
#endif
			}
			else if (troot->detail.tribleop.x->kind == CONST_OP)
			{
				fprintf(fp, "#%d ", troot->detail.tribleop.x->no);
#ifdef INSERTCODEDEBUG
				printf("#%d ", troot->detail.tribleop.x->no);
#endif
			}
			else
			{
				fprintf(fp, "%s ", troot->detail.tribleop.x->value);
#ifdef INSERTCODEDEBUG
				printf("%s ", troot->detail.tribleop.x->value);
#endif
			}
			fprintf(fp, "%s ", troot->relop);
#ifdef INSERTCODEDEBUG
			printf("%s ", troot->relop);
#endif
			if (troot->detail.tribleop.y->kind == TEMP_OP)
			{
				fprintf(fp, "t%d GOTO label%d\n", troot->detail.tribleop.y->no,troot->detail.tribleop.gotoLabel->no);
#ifdef INSERTCODEDEBUG
				printf("t%d GOTO label%d\n", troot->detail.tribleop.y->no, troot->detail.tribleop.gotoLabel->no);
#endif
			}
			else if (troot->detail.tribleop.y->kind == CONST_OP)
			{
				fprintf(fp, "#%d GOTO label%d\n", troot->detail.tribleop.y->no, troot->detail.tribleop.gotoLabel->no);
#ifdef INSERTCODEDEBUG
				printf("#%d GOTO label%d\n", troot->detail.tribleop.y->no, troot->detail.tribleop.gotoLabel->no);
#endif
			}
			else
			{
				fprintf(fp, "%s GOTO label%d\n", troot->detail.tribleop.y->value, troot->detail.tribleop.gotoLabel->no);
#ifdef INSERTCODEDEBUG
				printf("%s GOTO label%d\n", troot->detail.tribleop.y->no, troot->detail.tribleop.gotoLabel->no);
#endif
			}
		}
		else if (troot->kind == RETURN_CODE)
		{
			fprintf(fp, "RETURN ");
#ifdef INSERTCODEDEBUG
			printf("RETURN ");
#endif
			if (troot->detail.singleop.op->kind == TEMP_OP)
			{
				fprintf(fp, "t%d\n", troot->detail.singleop.op->no);
#ifdef INSERTCODEDEBUG
				printf("t%d\n", troot->detail.singleop.op->no);
#endif
			}
			else if (troot->detail.singleop.op->kind == CONST_OP)
			{
				fprintf(fp, "#%d\n", troot->detail.singleop.op->no);
#ifdef INSERTCODEDEBUG
				printf("#%d\n", troot->detail.singleop.op->no);
#endif
			}
			else
			{
				fprintf(fp, "%s\n", troot->detail.singleop.op->value);
#ifdef INSERTCODEDEBUG
				printf("%s\n", troot->detail.singleop.op->value);
#endif
			}
		}
		else if (troot->kind == ARG_CODE)
		{
			fprintf(fp, "ARG t%d\n", troot->detail.singleop.op->no);
#ifdef INSERTCODEDEBUG
			printf("ARG t%d\n", troot->detail.singleop.op->no);
#endif
		}
		else if (troot->kind == CALL_CODE)
		{
			if (troot->detail.assign.left->kind == TEMP_OP)
			{
				fprintf(fp, "t%d := ", troot->detail.assign.left->no);
#ifdef INSERTCODEDEBUG
				printf("t%d := ", troot->detail.assign.left->no);
#endif
			}
			else
			{
				fprintf(fp, "%s := ", troot->detail.assign.left->value);
#ifdef INSERTCODEDEBUG
				printf("%s := ", troot->detail.assign.left->value);
#endif
			}
			fprintf(fp, "CALL %s\n", troot->detail.assign.right->value);
#ifdef INSERTCODEDEBUG
			printf("CALL %s\n", troot->detail.assign.right->value);
#endif
		}
		else if (troot->kind == READ_CODE)
		{
			if (troot->detail.singleop.op->kind == TEMP_OP)
			{
				fprintf(fp, "READ t%d\n", troot->detail.singleop.op->no);
#ifdef INSERTCODEDEBUG
				printf("READ t%d\n", troot->detail.singleop.op->no);
#endif
			}
			else
			{
				fprintf(fp, "READ %s\n", troot->detail.singleop.op->value);
#ifdef INSERTCODEDEBUG
				printf("WRITE t%d\n", troot->detail.singleop.op->value);
#endif
			}
		}
		else if (troot->kind == WRITE_CODE)
		{
			if (troot->detail.singleop.op->kind == TEMP_OP)
			{
				fprintf(fp, "WRITE t%d\n", troot->detail.singleop.op->no);
#ifdef INSERTCODEDEBUG
				printf("WRITE t%d\n", troot->detail.singleop.op->no);
#endif
			}
			else if (troot->detail.singleop.op->kind == CONST_OP)
			{
				fprintf(fp, "WRITE #%d\n", troot->detail.singleop.op->no);
#ifdef INSERTCODEDEBUG
				printf("WRITE #%d\n", troot->detail.singleop.op->no);
#endif
			}
			else
			{
				fprintf(fp, "WRITE %s\n", troot->detail.singleop.op->value);
#ifdef INSERTCODEDEBUG
				printf("WRITE %s\n", troot->detail.singleop.op->value);
#endif
			}
		}
		troot = troot->next;
	}
	fclose(fp);
}

void insertCode(code *p)
{
	if (codeRoot == NULL)
	{
		codeRoot = p;
	}
	else
	{
		codeTail->next = p;
		p->pre = codeTail;
	}
	codeTail = p;
}
operand *new_temp()
{
	operand *temp1 = (operand *)malloc(sizeof(operand));
	memset(temp1, 0, sizeof(operand));
	temp1->kind = TEMP_OP;
	temp1->no = temp++;
	return temp1;
}
operand *new_label()
{
	operand *temp = (operand *)malloc(sizeof(operand));
	memset(temp, 0, sizeof(operand));
	temp->kind = LABEL_OP;
	temp->no = labeln++;
	return temp;
}
void translate_Basic_Exp(Node *root, operand *place)
{
	Node *child = root->child;
	if (!strcmp(child->name, "LP"))
	{
		translate_Basic_Exp(child->brother, place);
	}
	else if (!strcmp(child->name,"INT"))
	{
		int value = atoi(child->value);
		code *ret = (code *)malloc(sizeof(code));
		memset(ret, 0, sizeof(code));
		ret->kind = ASSIGN_CODE;
		ret->detail.assign.left = place;
		operand *cot = (operand *)malloc(sizeof(operand));
		memset(cot, 0, sizeof(operand));
		cot->kind = CONST_OP;
		cot->no = value;
		ret->detail.assign.right = cot;
		insertCode(ret);
	}
	else if (!strcmp(child->name, "ID")&&child->brother==NULL)
	{
		operand *variable = (operand *)malloc(sizeof(operand));
		memset(variable, 0, sizeof(operand));
		variable->kind = VAR_OP;
		variable->value = child->value;
		code *ret = (code *)malloc(sizeof(code));
		memset(ret, 0, sizeof(code));
		ret->kind = ASSIGN_CODE;
		ret->detail.assign.left = place;
		ret->detail.assign.right = variable;
		insertCode(ret);
	}
	else if (!strcmp(child->name, "Exp"))
	{
		if (!strcmp(child->brother->name, "ASSIGNOP"))
		{
			operand *variable = (operand *)malloc(sizeof(operand));
			memset(variable, 0, sizeof(operand));
			variable->kind = VAR_OP;
			variable->value = child->child->value;
			operand *t1 = new_temp();
			translate_Basic_Exp(child->brother->brother, t1);
			code *ret1 = (code *)malloc(sizeof(code));
			memset(ret1, 0, sizeof(code));
			ret1->kind = ASSIGN_CODE;
			ret1->detail.assign.left = variable;
			ret1->detail.assign.right = t1;
			insertCode(ret1);
			code *ret2 = (code *)malloc(sizeof(code));
			memset(ret2, 0, sizeof(code));
			ret1->kind = ASSIGN_CODE;
			ret2->detail.assign.left = place;
			ret2->detail.assign.right = variable;
			insertCode(ret2);
		}
		else if (!strcmp(child->brother->name, "PLUS"))
		{
			operand *t1 = new_temp();
			operand *t2 = new_temp();
			translate_Basic_Exp(child, t1);
			translate_Basic_Exp(child->brother->brother, t2);
			code *ret = (code *)malloc(sizeof(code));
			memset(ret, 0, sizeof(code));
			ret->kind = ADD_CODE;
			ret->detail.doubleop.result = place;
			ret->detail.doubleop.op1 = t1;
			ret->detail.doubleop.op2 = t2;
			insertCode(ret);
		}
		else if (!strcmp(child->brother->name, "MINUS"))
		{
			operand *t1 = new_temp();
			operand *t2 = new_temp();
			translate_Basic_Exp(child, t1);
			translate_Basic_Exp(child->brother->brother, t2);
			code *ret = (code *)malloc(sizeof(code));
			memset(ret, 0, sizeof(code));
			ret->kind = MINUS_CODE;
			ret->detail.doubleop.result = place;
			ret->detail.doubleop.op1 = t1;
			ret->detail.doubleop.op2 = t2;
			insertCode(ret);
		}
		else if (!strcmp(child->brother->name, "STAR"))
		{
			operand *t1 = new_temp();
			operand *t2 = new_temp();
			translate_Basic_Exp(child, t1);
			translate_Basic_Exp(child->brother->brother, t2);
			code *ret = (code *)malloc(sizeof(code));
			memset(ret, 0, sizeof(code));
			ret->kind = MUL_CODE;
			ret->detail.doubleop.result = place;
			ret->detail.doubleop.op1 = t1;
			ret->detail.doubleop.op2 = t2;
			insertCode(ret);
		}
		else if (!strcmp(child->brother->name, "DIV"))
		{
			operand *t1 = new_temp();
			operand *t2 = new_temp();
			translate_Basic_Exp(child, t1);
			translate_Basic_Exp(child->brother->brother, t2);
			code *ret = (code *)malloc(sizeof(code));
			memset(ret, 0, sizeof(code));
			ret->kind = DIV_CODE;
			ret->detail.doubleop.result = place;
			ret->detail.doubleop.op1 = t1;
			ret->detail.doubleop.op2 = t2;
			insertCode(ret);
		}
		else if (!strcmp(child->brother,"AND")|| !strcmp(child->brother, "OR") || !strcmp(child->brother->name, "RELOP"))
		{
			operand *label1 = new_label();
			operand *label2 = new_label();
			code *ret = (code *)malloc(sizeof(code));
			memset(ret, 0, sizeof(code));
			ret->kind = ASSIGN_CODE;
			ret->detail.assign.left = place;
			operand *cot = (operand *)malloc(sizeof(operand));
			memset(cot, 0, sizeof(operand));
			cot->kind = CONST_OP;
			cot->no = 0;
			ret->detail.assign.right = cot;
			insertCode(ret);
			translate_Cond(root, label1, label2);
			code *ret2 = (code *)malloc(sizeof(code));
			memset(ret2, 0, sizeof(code));
			ret2->kind = LABEL_CODE;
			ret2->detail.singleop.op = label1;
			insertCode(ret2);
			code *ret3 = (code *)malloc(sizeof(code));
			memset(ret3, 0, sizeof(code));
			ret3->kind = ASSIGN_CODE;
			operand *cot2 = (operand *)malloc(sizeof(operand));
			memset(cot2, 0, sizeof(operand));
			cot2->kind = CONST_OP;
			cot2->no = 1;

			ret3->detail.assign.left = place;
			ret3->detail.assign.right = cot2;
			insertCode(ret3);

			code *ret4 = (code *)malloc(sizeof(code));
			memset(ret4, 0, sizeof(code));
			ret4->kind = LABEL_CODE;
			ret4->detail.singleop.op = label2;
			insertCode(ret4);
		}
	}
	else if (!strcmp(child->name, "NOT"))
	{
		operand *label1 = new_label();
		operand *label2 = new_label();
		code *ret = (code *)malloc(sizeof(code));
		memset(ret, 0, sizeof(code));
		ret->kind = ASSIGN_CODE;
		ret->detail.assign.left = place;
		operand *cot = (operand *)malloc(sizeof(operand));
		memset(cot, 0, sizeof(operand));
		cot->kind = CONST_OP;
		cot->no = 0;
		ret->detail.assign.right = cot;
		insertCode(ret);
		translate_Cond(root, label1, label2);
		code *ret2 = (code *)malloc(sizeof(code));
		memset(ret2, 0, sizeof(code));
		ret2->kind = LABEL_CODE;
		ret2->detail.singleop.op = label1;
		insertCode(ret2);
		code *ret3 = (code *)malloc(sizeof(code));
		memset(ret3, 0, sizeof(code));
		ret3->kind = ASSIGN_CODE;
		operand *cot2 = (operand *)malloc(sizeof(operand));
		memset(cot2, 0, sizeof(operand));
		cot2->kind = CONST_OP;
		cot2->no = 1;

		ret3->detail.assign.left = place;
		ret3->detail.assign.right = cot2;
		insertCode(ret3);

		code *ret4 = (code *)malloc(sizeof(code));
		memset(ret4, 0, sizeof(code));
		ret4->kind = LABEL_CODE;
		ret4->detail.singleop.op = label2;
		insertCode(ret4);
	}
	else if (!strcmp(child->name, "MINUS"))
	{
		operand *t1 = new_temp();
		translate_Basic_Exp(child->brother,t1);
		code *ret = (code *)malloc(sizeof(code));
		memset(ret, 0, sizeof(ret));
		ret->kind = MINUS_CODE;
		operand *cot = (operand *)malloc(sizeof(operand));
		memset(cot, 0, sizeof(operand));
		cot->kind = CONST_OP;
		cot->no = 0;
		ret->detail.doubleop.result = place;
		ret->detail.doubleop.op1 = cot;
		ret->detail.doubleop.op2 = t1;
		insertCode(ret);
	}
	else if (!strcmp(child->name, "ID")&&child->brother!=NULL)
	{
		if (!strcmp(child->brother->brother, "Args"))
		{
			operand *arglist = NULL;

			if (!strcmp(child->value, "write"))
			{
				operand *arglist = NULL;
				translate_Args(child->brother->brother, &arglist);
				code *ret = (code *)malloc(sizeof(code));
				memset(ret, 0, sizeof(ret));
				ret->kind = WRITE_CODE;
				ret->detail.singleop.op = arglist;
				insertCode(ret);
			}
			else
			{
				translate_Args(child->brother->brother, &arglist);
				operand *func = (operand *)malloc(sizeof(operand));
				memset(func, 0, sizeof(operand));
				func->kind = FUNC_OP;
				func->value = child->value;
				operand *tem = arglist;
				code *codelistroot = NULL;
				code *codelisttail = NULL;
				while (tem != NULL)
				{
					code *reti = (code *)malloc(sizeof(code));
					memset(reti, 0, sizeof(code));
					reti->kind = ARG_CODE;
					reti->detail.singleop.op = tem;
					if (codelisttail == NULL)
					{
						codelisttail = reti;
					}
					else
					{
						codelistroot->pre = reti;
						reti->next = codelistroot;
					}
					codelistroot = reti;
					
					tem = tem->nextArg;
				}
				codeTail->next = codelistroot;
				codeTail = codelisttail;
				code *ret2 = (code *)malloc(sizeof(code));
				memset(ret2, 0, sizeof(code));
				ret2->kind = CALL_CODE;
				ret2->detail.assign.left = place;
				ret2->detail.assign.right = func;
				insertCode(ret2);
			}
		}
		else
		{
			if (!strcmp(child->value, "read"))
			{
				code *ret = (code *)malloc(sizeof(code));
				memset(ret, 0, sizeof(code));
				ret->kind = READ_CODE;
				ret->detail.singleop.op = place;
				insertCode(ret);
			}
			else
			{
				operand *func = (operand *)malloc(sizeof(operand));
				memset(func, 0, sizeof(operand));
				func->kind = FUNC_OP;
				func->value = child->value;
				code *ret = (code *)malloc(sizeof(code));
				memset(ret, 0, sizeof(code));
				ret->kind = CALL_CODE;
				ret->detail.assign.left = place;
				ret->detail.assign.right = func;
				insertCode(ret);
			}
		}
	}
}
void translate_Args(Node *root,operand **arglist)
{
	Node *child = root->child;
	if (child->brother == NULL)
	{
		operand *t1 = new_temp();
		translate_Basic_Exp(child, t1);
		t1->nextArg = (*arglist);
		(*arglist) = t1;
	}
	else
	{
		operand *t1 = new_temp();
		
		translate_Basic_Exp(child, t1);
		t1->nextArg = (*arglist);
		(*arglist) = t1;
		translate_Args(child->brother->brother, arglist);
	}
}

void translate_Stmt(Node *root)
{
	Node *child = root->child;
	if (!strcmp(child->name, "Exp"))
	{
		translate_Basic_Exp(child, NULL);
	}
	else if (!strcmp(child->name, "RETURN"))
	{
		operand *t1 = new_temp();
		translate_Basic_Exp(child->brother, t1);
		code *ret = (code *)malloc(sizeof(code));
		memset(ret, 0, sizeof(code));
		ret->kind = RETURN_CODE;
		ret->detail.singleop.op = t1;
		insertCode(ret);
	}
	else if(!strcmp(child->name,"IF"))
	{
		if (child->brother->brother->brother->brother->brother == NULL)
		{
			operand *label1 = new_label();
			operand *label2 = new_label();
			translate_Cond(child->brother->brother, label1, label2);
			code *ret1 = (code *)malloc(sizeof(code));
			memset(ret1, 0, sizeof(code));
			ret1->kind = LABEL_CODE;
			ret1->detail.singleop.op = label1;
			insertCode(ret1);
			translate_Stmt(child->brother->brother->brother->brother);
			code *ret2 = (code *)malloc(sizeof(code));
			memset(ret2, 0, sizeof(code));
			ret2->kind = LABEL_CODE;
			ret2->detail.singleop.op = label2;
			insertCode(ret2);
		}

		else if (child->brother->brother->brother->brother->brother != NULL)
		{
			operand *label1 = new_label();
			operand *label2 = new_label();
			//code1
			translate_Cond(child->brother->brother, label1, label2);
			//label1
			code *ret1 = (code *)malloc(sizeof(code));
			memset(ret1, 0, sizeof(code));
			ret1->kind = LABEL_CODE;
			ret1->detail.singleop.op = label1;
			insertCode(ret1);
			//code2
			translate_Stmt(child->brother->brother->brother->brother);


			code *ret2 = (code *)malloc(sizeof(code));
			memset(ret2, 0, sizeof(code));
			ret2->kind = LABEL_CODE;
			ret2->detail.singleop.op = label2;
			operand *label3 = new_label();
			code *ret3 = (code *)malloc(sizeof(code));
			memset(ret3, 0, sizeof(code));
			ret3->kind = GOTO_CODE;
			ret3->detail.singleop.op = label3;
			//gotoLabel3
			insertCode(ret3);

			//label2
			insertCode(ret2);
			//code3
			translate_Stmt(child->brother->brother->brother->brother->brother->brother);
			//label3
			code *ret4 = (code *)malloc(sizeof(code));
			memset(ret4, 0, sizeof(code));
			ret4->kind = LABEL_CODE;
			ret4->detail.singleop.op = label3;
			insertCode(ret4);
			
		}
	}
	else if (!strcmp(child->name, "WHILE"))
	{
		operand *label1 = new_label();
		operand *label2 = new_label();
		operand *label3 = new_label();
		code *ret1 = (code *)malloc(sizeof(code));
		memset(ret1, 0, sizeof(code));
		ret1->kind = LABEL_CODE;
		ret1->detail.singleop.op = label1;
		insertCode(ret1);
		translate_Cond(child->brother->brother, label2, label3);
		code *ret2 = (code *)malloc(sizeof(code));
		memset(ret2, 0, sizeof(code));
		ret2->kind = LABEL_CODE;
		ret2->detail.singleop.op = label2;
		insertCode(ret2);
		translate_Stmt(child->brother->brother->brother->brother);
		code *ret3 = (code *)malloc(sizeof(code));
		memset(ret3, 0, sizeof(code));
		ret3->kind = GOTO_CODE;
		ret3->detail.singleop.op = label1;
		insertCode(ret3);
		code *ret4 = (code *)malloc(sizeof(code));
		memset(ret4, 0, sizeof(code));
		ret4->kind = LABEL_CODE;
		ret4->detail.singleop.op = label3;
		insertCode(ret4);
	}
}

void translate_Cond(Node *root, operand *label_true, operand *label_false)
{
	Node *child = root->child;
	if (!strcmp(child->name, "Exp"))
	{
		if (!strcmp(child->brother->name, "RELOP"))
		{
			operand *t1 = new_temp();
			operand *t2 = new_temp();
			translate_Basic_Exp(child, t1);
			translate_Basic_Exp(child->brother->brother, t2);
			code *ret1 = (code *)malloc(sizeof(code));
			memset(ret1, 0, sizeof(code));
			ret1->kind = IF_GOTO_CODE;
			ret1->relop = child->brother->value;
			ret1->detail.tribleop.gotoLabel = label_true;
			ret1->detail.tribleop.x = t1;
			ret1->detail.tribleop.y = t2;
			insertCode(ret1);
			code *ret2 = (code *)malloc(sizeof(code));
			memset(ret2, 0, sizeof(code));
			ret2->kind = GOTO_CODE;
			ret2->detail.singleop.op = label_false;
			insertCode(ret2);
		}
		else if (!strcmp(child->brother->name, "AND"))
		{
			operand *label1 = new_label();
			translate_Cond(child, label1, label_false);
			code *ret = (code *)malloc(sizeof(code));
			memset(ret, 0, sizeof(code));
			ret->kind = LABEL_CODE;
			ret->detail.singleop.op = label1;
			insertCode(ret);
			translate_Cond(child->brother->brother, label_true, label_false);
		}
		else if (!strcmp(child->brother->name, "OR"))
		{
			operand *label1 = new_label();
			translate_Cond(child, label_true, label1);
			code *ret = (code *)malloc(sizeof(code));
			memset(ret, 0, sizeof(code));
			ret->kind = LABEL_CODE;
			ret->detail.singleop.op = label1;
			insertCode(ret);
			translate_Cond(child->brother->brother, label_true, label_false);
		}
		else if (!strcmp(child->name, "NOT"))
		{
			translate_Cond(child->brother, label_false, label_true);
		}
		else
		{
			operand *t1 = new_temp();
			translate_Basic_Exp(root, t1);
			code *ret1 = (code *)malloc(sizeof(code));
			memset(ret1, 0, sizeof(code));
			ret1->kind = IF_GOTO_CODE;
			operand *cot = (operand *)malloc(sizeof(operand));
			memset(cot, 0, sizeof(operand));
			cot->kind = CONST_OP;
			cot->no = 0;
			ret1->detail.tribleop.gotoLabel = label_true;
			ret1->detail.tribleop.x = t1;
			ret1->detail.tribleop.y = cot;
			insertCode(ret1);

			code *ret2 = (code *)malloc(sizeof(code));
			memset(ret2, 0, sizeof(code));
			ret2->kind = GOTO_CODE;
			ret2->detail.singleop.op = label_false;
			insertCode(ret2);
		}
	}
}

int checkOutStructSize(char *name)
{
	HashNode *structNode = hashCheck(globalHashTable, name, TYPE_KIND, 0);

}