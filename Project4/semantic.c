#include "newHashTable.h"
#include "Node.h"
#include "semantic.h"
#include "interCode.h"
//#define DEBUG
#define INSERTCODE
void Program(Node* root)
{
	//=============构造hash表与stack============
	//------------------填充--------------------
	//=========================================
#ifdef DEBUG
	printf("InitHashTable\n");
#endif
	globalHashTable = initHashTable(HASH_TABLE_SIZE);
#ifdef DEBUG
	printf("init HashTable successfully\n");
#endif
#ifdef DEBUG
	printf("InitHashStack\n");
#endif
	globalHashStack = stackPush(globalHashStack);
#ifdef DEBUG
	printf("init HashStack successfully\n");
#endif


	if (root == NULL)
		return;
	ExtDefList(root->child);//声明部分

	//释放HashTable和HashStack
	//---------填充-----------
	//=======================
}

void ExtDefList(Node *root)
{
	if (root == NULL)
		return;
	ExtDef(root->child);
	ExtDefList(root->child->brother);
	
}
#define MAINFUN 0x1000
#define MAINFUN_WITHRETURN 0x1002
#define MAINFUN_WITHOUTRETURN 0x1003

#define FUN 0x2000
#define FUN_WITHRETURN 0x2002
#define FUN_WITHOUTRETURN 0x2003

#define INFUN 0x0
void ExtDef(Node *root)
{
	Node *child = root->child;
	Type *type = NULL;

	type = Specifier(child);//获取类型
	if (type == NULL)
		return;
	if (!strcmp(child->brother->name, "ExtDecList"))
	{
#ifdef DEBUG
		printf("ExtDef->Specifier ExtDecList SEMI\n");
#endif
		ExtDecList(child->brother, type);
	}
	else if (!strcmp(child->brother->name, "SEMI"))
	{
#ifdef DEBUG
		printf("ExtDef->Specifier SEMI\n");
#endif

	}
	else if (!strcmp(child->brother->name, "FunDec"))
	{

		FuncDec * tempfun = FunDec(child->brother, type);
		if (!strcmp(child->brother->brother->name, "SEMI"))
		{
			//这里只是声明了函数,什么也不干就好，记得最后对最底层栈进行遍历，看是否有函数未定义。
#ifdef DEBUG
			printf("ExtDef->Specifier FunDec SEMI\n");
#endif
		}
		//接下来将参数放入栈中，并在Hash表中进行注册
		else
		{
#ifdef DEBUG
			printf("ExtDef->Specifier FunDec CompSt\n");
#endif
			if (tempfun == NULL)
			{
				return;
			}
			if (tempfun->isDef == 1)//这里已经定义过，但接下来还要定义
			{
				printf("Error type 4 at Line %d: Redefine function \"%s\".\n", child->lineno, tempfun->name);
				return;//不要继续往下执行了。
			}
			globalHashStack = stackPush(globalHashStack);//放入一个新的栈，进行函数内部处理
			//这里需要把函数的参数压入栈下挂的链表中
			//--------------填充----------------
			HashNode *argList = tempfun->argsDec;
			while (argList != NULL)
			{
				HashNode *newArgList = initHashNode(TYPE_KIND, argList->name, argList->depth + 1);
				newArgList->type = argList->type;

				//往hash表中注册
				hashPush(globalHashTable, newArgList);
				//往链表里挂
				newArgList->nextInStack = globalHashStack->start;
				globalHashStack->start = newArgList;
				
				argList = argList->nextInStack;
			}
			//==================================
			tempfun->isDef = 1;//将定义状态改成已定义
			translate_CompSt(child->brother->brother);
			CompSt(child->brother->brother, tempfun->retType, globalHashStack);
			//这里需要弹出栈
			globalHashStack = stackPop(globalHashStack);
			
			//-----填充--------
			//==================


		}

#ifdef DEBUG
		//printHashTable(globalHashTable);
#endif
    }
}

void CompSt(Node *root, Type *retType, Stack *globalstack)
{
	
#ifdef DEBUG
	printf("CompSt->LC DefList StmtList RC\n");
#endif
	if (globalstack == NULL)
	{
		globalHashStack = stackPush(globalHashStack);
	}
	Node *child = root->child->brother;//DefList;
	if (!strcmp(child->name, "DefList"))
	{
		DefList(child, NULL);
		if (!strcmp(child->brother->name, "StmtList"))
		{
			StmtList(child->brother, retType);
		}
	}
	else if(!strcmp(child->name,"StmtList"))
	{
		StmtList(child, retType);
		
	}
	if (globalstack == NULL)
	{
		globalHashStack = stackPop(globalHashStack);
	}
	
}

void StmtList(Node *root, Type *retType)
{
	if (root == NULL)
	{
		
#ifdef DEBUG
		printf("StmtList->NULL\n");
#endif
		return;
	}
	int retflag1 = 0;
	int retflag2 = 0;
#ifdef DEBUG
	printf("StmtList->Stmt StmtList\n");
#endif

	Stmt(root->child, retType);

	StmtList(root->child->brother, retType);
}
void Stmt(Node *root, Type *retType)
{
	if (root == NULL)
	{
		return;
	}



	Node *child = root->child;
	if (!strcmp(child->name, "Exp"))
	{
#ifdef DEBUG
		printf("Stmt->Exp SEMI\n");
#endif
		Exp(child);
	}
	else if (!strcmp(child->name, "CompSt"))
	{
#ifdef DEBUG
		printf("Stmt->CompSt\n");
#endif
		CompSt(root->child, retType, NULL);//传入参数为NULL表示需要在这个函数里进行入栈出栈操作
	}
	else if (!strcmp(child->name, "RETURN"))
	{
#ifdef DEBUG
		printf("Stmt->RETURN EXP SEMI\n");
#endif
		Type *ret = NULL;
		ret = Exp(child->brother);
		//这里要写个比较两个类型的函数
		if (ret == NULL)
			return;
		if (!typeCmp(ret, retType))
		{
			//类型不相同
			printf("Error type 8 at line %d: Type mismatched for return.\n",child->lineno);
		}
		return;
	}
	else if (!strcmp(child->name, "IF"))
	{
		if (child->brother->brother->brother->brother->brother == NULL)
		{
#ifdef DEBUG
			printf("Stmt->IF LP Exp RP Stmt\n");
#endif
			Exp(child->brother->brother);
			Stmt(child->brother->brother->brother->brother,retType);
		}
		else
		{
#ifdef DEBUG
			printf("Stmt->IF LP Exp RP Stmt ELSE Stmt\n");
#endif
			Exp(child->brother->brother);
			int flag1;
			int flag2;
			Stmt(child->brother->brother->brother->brother, retType);
			Stmt(child->brother->brother->brother->brother->brother->brother, retType);
		}
	}
	else if (!strcmp(child->name, "WHILE"))
	{
#ifdef DEBUG
		printf("Stmt->WHILE LP Exp RP Stmt\n");
#endif
		Exp(child->brother->brother);
		Stmt(child->brother->brother->brother->brother,retType);
	}
}


FuncDec *FunDec(Node *root, Type *p)
{
	
    Node *child = root->child;
    //这里进行函数名检查是否重复以及注册HashNode
	HashNode *res = hashCheck(globalHashTable, child->value, FUNC_KIND, -1);
	if (res != NULL)//已经声明或定义过
	{
		return res->func;//直接交给上层进行处理
	}
    //====================================
	else//未定义且未声明
	{
		//这里检查后无重复名称
		FuncDec *newFuncDec = (FuncDec *)malloc(sizeof(FuncDec));
		newFuncDec->name = child->value;
		p->ifCom = 1;
		newFuncDec->retType = p;//返回值类型确定。
		newFuncDec->isDef = 0;//现在先假设未定义。
		HashNode *newFuncNode = initHashNode(FUNC_KIND, newFuncDec->name, globalHashStack->depth);
		newFuncNode->func = newFuncDec;


#ifdef INSERTCODE
		code *ret = (code *)malloc(sizeof(code));
		memset(ret, 0, sizeof(ret));
		ret->kind = FUNCTION_CODE;
		ret->detail.singleop.op = (operand *)malloc(sizeof(operand));
		memset(ret->detail.singleop.op, 0, sizeof(operand));
		ret->detail.singleop.op->kind = FUNC_OP;
		ret->detail.singleop.op->value = child->value;
		insertCode(ret);
#endif

		if (!strcmp(child->brother->brother->name, "VarList"))
		{
			//这里有参数列表
#ifdef DEBUG
			printf("FunDec->ID LP VarList RP\n");
#endif
			VarList(child->brother->brother, newFuncDec);
		}
		else if (!strcmp(child->brother->brother->name, "RP"))
		{
			//无参数列表时
#ifdef DEBUG
			printf("FunDec->ID LP RP\n");
#endif
		}
		//在Hash表中进行注册
		hashPush(globalHashTable, newFuncNode);
		//放入Stack里
		newFuncNode->nextInStack = globalHashStack->start;
		globalHashStack->start = newFuncNode;
		return newFuncDec;
	}
}
void ExtDecList(Node *root,Type *p)
{

	Node *child = root->child;
	HashNode *newHashNode = NULL;
	newHashNode = VarDec(child,p,NULL);
	if (newHashNode == NULL)
	{
#ifdef DEBUG
		printf("ExtDecList中的VarDec返回值为空\n");
#endif
		return;
	}
	//这里检查是否已经注册过，未注册则进行注册
	if (hashCheck(globalHashTable, newHashNode->name, TYPE_KIND, globalHashStack->depth)!=NULL)
	{
		//已经注册过了
		printf("Error type 3 at line %d: Redefined variable \"%s\".\n", child->lineno, newHashNode->name);
		return;
	}



	//进行注册
	hashPush(globalHashTable, newHashNode);
	newHashNode->nextInStack = globalHashStack->start;
	globalHashStack->start = newHashNode;
	
	//=============================
	if (child->brother != NULL)
	{
#ifdef DEBUG
		printf("ExtDecList->VarDec COMMA ExtDecList\n");
#endif
		ExtDecList(child->brother->brother, p);
	}
	else
	{
#ifdef DEBUG
		printf("ExtDecList->VarDec\n");
#endif
	}
}

Type *Specifier(Node *root)
{
    Type *type = NULL;
    Node *child = root->child;

    if(!strcmp(child->name,"TYPE"))
    {
        #ifdef DEBUG
        printf("Specifier->TYPE\n");
        #endif
        type = (Type *)malloc(sizeof(Type));
		memset(type, 0, sizeof(Type));
        type->kind = BASIC_KIND;
        if(!strcmp(child->value,"int"))
        {
            type->detail.basic = BASIC_INT;
        }
        else if(!strcmp(child->value,"float"))
        {
            type->detail.basic = BASIC_FLOAT;
        }
    }
    else if(!strcmp(child->name,"StructSpecifier"))
    {
        #ifdef DEBUG
        printf("Specifier->StructSpecifier\n");
        #endif
        type = StructSpecifier(child);
    }
    return type;
}
Type *StructSpecifier(Node *root)
{
    if(root==NULL)
        return NULL;
    Type *type = NULL;
    Node *child = root->child->brother;
    if(!strcmp(child->name,"OptTag"))
    {
        #ifdef DEBUG
        printf("SructSpecifier->STRUCT OptTag LC DefList RC\n");
        #endif
        //这里应该执行是否重定义
		if (hashCheck(globalHashTable, child->child->value, TYPE_KIND, -1)!=NULL)
		{
			//已经定义过该结构体
			printf("Error type 16 at line %d: Duplicated name \"%s\".\n",child->lineno,child->child->value);
			return NULL;
		}
        //==================
        type = (Type *)malloc(sizeof(Type));
		memset(type, 0, sizeof(Type));

        if(child->child!=NULL)
            type->name = child->child->value;
        else
            type->name = NULL;
        type->kind = STRUCT_DEC;
        FieldList *structure = NULL;
		structure = (FieldList *)malloc(sizeof(FieldList));
		memset(structure, 0, sizeof(FieldList));
		structure->name = child->child->value;
		structure->type = NULL;
		structure->nextFieldList = NULL;
		type->detail.structure = structure;
		type->name = structure->name;
		//这里构造结构体
		//这里进行检察结构体是否已定义以及注册
		if (hashCheck(globalHashTable, type->name, TYPE_KIND, -1) != NULL)
		{
			//已经注册过
			printf("Error type 16 at Line %d: Duplicated name \"%s\".\n", child->lineno, type->name);
			return NULL;
		}
		//未注册,进行注册
		HashNode *newHashNode = initHashNode(TYPE_KIND, type->name, globalHashStack->depth);
		newHashNode->type = type;
		hashPush(globalHashTable, newHashNode);
		newHashNode->nextInStack = globalHashStack->start;
		globalHashStack->start = newHashNode;
		if (strcmp(child->brother->brother->name, "DefList"))
		{
			//printf("Got It\n");
			;
		}
		else
		{
			DefList(child->brother->brother, structure);
		}


    }
    else if(!strcmp(child->name,"Tag"))
    {
		
#ifdef DEBUG
        printf("StructSpecifier->STRUCT Tag\n");
#endif
        //这里应该检查是否未定义
		HashNode *retNode = NULL;
		retNode = hashCheck(globalHashTable, child->child->value, TYPE_KIND, 0);
		if (retNode == NULL)
		{
			//这里未找到定义的节点
			printf("Error type 17 at line %d: Undefined Structure \"%s\".\n",child->lineno,child->child->value);
			return NULL;
		}
        //==================
		if (retNode->type->kind != STRUCT_DEC && retNode->type->kind != STRUCT_KIND)
		{
			printf("Error type 17 at line %d: Undefined Structure \"%s\".\n", child->lineno, child->child->value);
			return NULL;
		}
	
        type = (Type *)malloc(sizeof(Type));
		memset(type, 0, sizeof(Type));
        type -> name = child->child->value;
        type -> kind = STRUCT_KIND;
        type -> detail.structure = retNode->type->detail.structure;
    }
    return type;
}

void DefList(Node *root, FieldList *structure)
{
    if(root==NULL)
        return;
    Node *child = root->child;
    if(structure!=NULL)
    {
        #ifdef DEBUG
        printf("struct:DefList->Def DefList\n");
        #endif
        Def(child,structure);
		
        DefList(child->brother,structure);
    }
    else
    {
#ifdef DEBUG
		printf("DefList->Def DefList\n");
#endif
		Def(child, NULL);
		DefList(child->brother, NULL);
    }
}

void Def(Node *root,FieldList* structure)
{
    Node *child = root->child;
    Type *p = NULL;
    if(structure!=NULL)
    {
#ifdef DEBUG
        printf("struct:Def->Specifier DecList SEMI\n");
#endif
        p = Specifier(child);
        DecList(child->brother,p,structure);
    }
    else
    {
#ifdef DEBUG
		printf("Def->Specifier DecList SEMI\n");
#endif
		p = Specifier(child);
		if (p == NULL)
		{
			return;
		}
		DecList(child->brother, p, NULL);
    }
}

void DecList(Node *root,Type *type,FieldList *structure)
{ 
    Node *child = root->child;
    if(structure!=NULL)
    {
        Dec(child,type,structure);
        if(child->brother!=NULL)
        {
            #ifdef DEBUG
            printf("struct:DecList->Dec COMMA DecList\n");
            #endif
            DecList(child->brother->brother,type,structure);
        }
    }
    else
    {
		Dec(child, type, NULL);
		if (child->brother != NULL)
		{
#ifdef DEBUG
			printf("DecList->Dec COMMA DecList\n");
#endif
			DecList(child->brother->brother, type, NULL);
		}
		else
		{
#ifdef DEBUG
			printf("DecList->Dec\n");
#endif
		}
    }
}

void Dec(Node *root,Type *type,FieldList *structure)
{
    Node *child = root->child;


    if(structure!=NULL)
    {
        VarDec(child,type,structure);
        if(child->brother!=NULL)
        {
            printf("Error type 15 at Line %d: Can't initial struct Field.\n",child->lineno);
        }
        else
        {
            #ifdef DEBUG
            printf("struct:Dec->VarDec\n");
            #endif
        }
    }
    else
    {
		HashNode *newHashNode = VarDec(child, type, NULL);

		
		//检察是否已经定义以及注册
		if (hashCheck(globalHashTable, newHashNode->name, TYPE_KIND, -1)!=NULL)
		{
			printf("Error type 3 at Line %d: Redefined variable \"%s\".\n", child->lineno, newHashNode->name);
			return;
		}
		//注册变量
		hashPush(globalHashTable, newHashNode);
		newHashNode->nextInStack = globalHashStack->start;
		globalHashStack->start = newHashNode;
		if (child->brother == NULL)
		{
			//这里没有进行初始化
#ifdef DEBUG
			printf("Dec->VarDec\n");
#endif
		}
		else
		{



			//这里进行初始化
			//需要判断左右量表类型是否相同
			Type *newType = Exp(child->brother->brother);

//#ifdef INSERTCODE
//			operand *o = (operand *)malloc(sizeof(operand));
//			memset(o, 0, sizeof(operand));
//			o->kind = VAR_OP;
//			o->value = newHashNode->name;
//			translate_Basic_Exp(child->brother->brother, o);
//#endif
			
			if (!typeCmp(newType, type))
			{
				printf("Error type 5 at line %d: Type mismatched for assignment.\n", child->lineno);
				return;
			}
		}
    }
}
int compArgList(HashNode *list1, HashNode *list2)
{
	if ((list1 == NULL&&list2 != NULL) || (list1 != NULL&&list2 == NULL))
	{
		return 0;
	}
	else if (list1 == NULL&&list2 == NULL)
	{
		return 1;
	}
	else
	{
		if (typeCmp(list1->type, list2->type))
		{
			return compArgList(list1->nextInStack, list2->nextInStack);
		}
		else
			return 0;
	}
}
void fillArgChar(HashNode *List1, char *arg2)
{
	if (List1 == NULL)
	{
		return;
	}
	if (List1->type->kind == BASIC_KIND)
	{
		if (List1->type->detail.basic == BASIC_INT)
		{
			if(arg2[0]=='\0')
			{
				sprintf(arg2, "%s", "int");
			}
			else
			{
				char *temp2 = "int,";
				char temp1[256];
				strcpy(temp1, arg2);
				sprintf(arg2, "%s%s", temp2, temp1);
			}
			
		}
		else
		{
			if (arg2[0] == '\0')
			{
				sprintf(arg2, "%s", "float");
			}
			else
			{
				char *temp2 = "float,";
				char temp1[256];
				strcpy(temp1, arg2);
				sprintf(arg2,"%s%s",temp2, temp1);
			}
		}
	}
	else if (List1->type->kind == INTARRAY_KIND)
	{
		int i = 0;
		Type *temptype = List1->type;
		while (temptype != NULL&&temptype->kind==INTARRAY_KIND)
		{
			temptype = temptype->detail.array.elem;
			i++;
		}
		char temp3[256] = "int";
		int c = 0;
		for (; c < i; c++)
		{
			strcat(temp3, "*");
		}
		if (arg2[0] != '\0')
		{
			strcat(temp3, ",");
		}
		char temp1[256];
		strcpy(temp1, arg2);
		sprintf(arg2, "%s%s", temp3, temp1);
	}
	else if (List1->type->kind == FLOATARRAY_KIND)
	{
		int i = 0;
		Type *temptype = List1->type;
		while (temptype != NULL&&temptype->kind == FLOATARRAY_KIND)
		{
			temptype = temptype->detail.array.elem;
			i++;
		}
		char temp3[256] = "float";
		int c = 0;
		for (; c < i; c++)
		{
			strcat(temp3, "*");
		}
		if (arg2[0] != '\0')
		{
			strcat(temp3, ",");
		}
		char temp1[256];
		strcpy(temp1, arg2);
		sprintf(arg2, "%s%s", temp3, temp1);
	}
	else if (List1->type->kind == STRUCT_KIND)
	{
		char temp2[256] = "struct ";
		strcat(temp2, List1->type->detail.structure->name);
		if (arg2[0] != '\0')
			strcat(temp2, ",");
		char temp1[256];
		strcpy(temp1, arg2);
		sprintf(arg2, "%s%s", temp2, temp1);
	}
	fillArgChar(List1->nextInStack, arg2);
}

Type *Assignop(Type *t1, Type *t2, int number) 
{
	if (t1 == NULL || t2 == NULL)
		return NULL;
	else if (typeCmp(t1, t2) == 1)
	{
		Type *newType = (Type *)malloc(sizeof(Type));
		memset(newType, 0, sizeof(Type));
		newType->ifCom = 1;
		newType->kind = BASIC_KIND;
		newType->detail.basic = BASIC_INT;
		return newType;
	}
	else {
		printf("Error type 5 at line %d: Type mismatched for assignment.\n", number);
		return NULL;
	}
}
Type *AND(Type *t1,Type *t2, int number)
{
	if (t1 == NULL || t2 == NULL)
		return NULL;
	else if (typeCmp(t1, t2) == 1)
	{
		Type *newType = (Type *)malloc(sizeof(Type));
		memset(newType, 0, sizeof(Type));
		newType->ifCom = 1;
		newType->kind = BASIC_KIND;
		newType->detail.basic = BASIC_INT;
		return newType;
	}
	else {
		printf("Error type 7 at line %d: Type mismatched for operand.\n", number);
		return NULL;
	}
}
Type *OR(Type *t1, Type *t2, int number)
{
	if (t1 == NULL || t2 == NULL)
		return NULL;
	else if (typeCmp(t1, t2) == 1)
	{
		Type *newType = (Type *)malloc(sizeof(Type));
		memset(newType, 0, sizeof(Type));
		newType->ifCom = 1;
		newType->kind = BASIC_KIND;
		newType->detail.basic = BASIC_INT;
		return newType;
	}
	else {
		printf("Error type 7 at line %d: Type mismatched for operand.\n", number);
		return NULL;
	}
}
Type *RELOP(Type *t1, Type *t2, int number)
{
	if (t1 == NULL || t2 == NULL)
		return NULL;
	else if (typeCmp(t1, t2) == 1)
	{
		Type *newType = (Type *)malloc(sizeof(Type));
		memset(newType, 0, sizeof(Type));
		newType->ifCom = 1;
		newType->kind = BASIC_KIND;
		newType->detail.basic = BASIC_INT;
		return newType;
	}
	else {
		printf("Error type 7 at line %d: Type mismatched for operand.\n", number);
		return NULL;
	}
}
Type *PLUS(Type *t1, Type *t2,int lineno)
{
	if (t1 == NULL || t2 == NULL)
		return NULL;
	else
	{
		if (typeCmp(t1, t2))
		{
			Type *newType = (Type *)malloc(sizeof(Type));
			memset(newType, 0, sizeof(Type));
			newType->ifCom = 1;
			newType->kind = t1->kind;
			newType->detail.basic = t1->detail.basic;
			return newType;
		}
		else
		{
			printf("Error type 7 at line %d: Type mismatched for operand.\n", lineno);
			return NULL;
		}
	}
	return NULL;
}
Type *MINUS(Type *t1, Type *t2,int number)
{
	if (t1 == NULL || t2 == NULL)
		return NULL;
	else if (typeCmp(t1, t2) == 1)
	{
		Type *newType = (Type *)malloc(sizeof(Type));
		memset(newType, 0, sizeof(Type));
		newType->ifCom = 1;
		newType->kind = t1->kind;
		newType->detail.basic = t1->detail.basic;
		return newType;
	}
	else {
		printf("Error type 7 at line %d: Type mismatched for operand.\n", number);
		return NULL;
	}
}
Type *STAR(Type *t1, Type *t2, int number)
{
	if (t1 == NULL || t2 == NULL)
		return NULL;
	else if (typeCmp(t1, t2) == 1)
	{
		Type *newType = (Type *)malloc(sizeof(Type));
		memset(newType, 0, sizeof(Type));
		newType->ifCom = 1;
		newType->kind = t1->kind;
		newType->detail.basic = t1->detail.basic;
		return newType;
	}
	else {
		printf("Error type 7 at line %d: Type mismatched for operand.\n", number);
		return NULL;
	}
}
Type *DIV(Type *t1, Type *t2, int number)
{
	if (t1 == NULL || t2 == NULL)
		return NULL;
	else if (typeCmp(t1, t2) == 1)
	{
		Type *newType = (Type *)malloc(sizeof(Type));
		memset(newType, 0, sizeof(Type));
		newType->ifCom = 1;
		newType->kind = t1->detail.basic;
		newType->detail.basic = t1->detail.basic;
		return newType;
	}
	else {
		printf("Error type 7 at line %d: Type mismatched for operands.\n", number);
		return NULL;
	}
}
Type *NOT(Type *t,int number)
{
	if(t==NULL)
		return NULL;
	else
	{
		Type *newType = (Type *)malloc(sizeof(Type));
		memset(newType, 0, sizeof(Type));
		newType->ifCom = 1;
		newType->kind = BASIC_KIND;
		newType->detail.basic = BASIC_INT;
		return newType;
	}
}
Type *Exp(Node *root)
{
	Node *child = root->child;
	//ID
	if (!strcmp(child->name, "ID")&&child->brother == NULL)
	{
#ifdef DEBUG
		printf("Exp->ID\n");
#endif
		//在Hash表中寻找
		HashNode *res = NULL;
		res = hashCheck(globalHashTable, child->value, TYPE_KIND, -1);
		if (res == NULL)
		{
			//在Hash表中没有定义
			printf("Error type 1 at line %d: Undefined variable \"%s\".\n", child->lineno, child->value);
			return NULL;
		}
		else
		{
			if (res->type->kind == FUNC_KIND)
				return NULL;
			else
				return res->type;
		}
	}
	else if (!strcmp(child->name, "INT"))//INT
	{
#ifdef DEBUG
		printf("Exp->INT\n");
#endif
		Type *newType = (Type *)malloc(sizeof(Type));
		newType->kind = BASIC_KIND;
		newType->name = child->value;
		newType->detail.basic = BASIC_INT;
		newType->ifCom = 1;
		return newType;
	}
	else if (!strcmp(child->name, "FLOAT"))//FLOAT
	{
#ifdef DEBUG
		printf("Exp->FLOAT\n");
#endif
		Type *newType = (Type *)malloc(sizeof(Type));
		newType->kind = BASIC_KIND;
		newType->name = child->value;
		newType->detail.basic = BASIC_FLOAT;
		newType->ifCom = 1;
		return newType;
	}
	//ID LP Args RP
	else if (!strcmp(child->name, "ID") && !strcmp(child->brother->brother->name, "Args"))
	{
#ifdef DEBUG
		printf("Exp->ID LP Args RP\n");
#endif
		if (!strcmp(child->value, "write"))
		{
			return NULL;
		}
		//这里需要检查ID是否声明过以及检查参数类型是否匹配，返回函数的返回值类型
		//检察ID是否定义过
		HashNode *tempfunc = NULL; 
		tempfunc = hashCheck(globalHashTable, child->value, FUNC_KIND, -1);
		HashNode *tempfunc2 = hashCheck(globalHashTable, child->value, TYPE_KIND, -1);
		if (tempfunc == NULL)//函数不存在
		{
			if (tempfunc2 == NULL)
			{

				printf("Error type 2 at line %d: Undefined function \"%s\".\n", child->lineno, child->value);
				return NULL;//返回一个空类型
			}
			else
			{
				printf("Error type 11 at Line %d: \"%s\" is not a function.\n", child->lineno, tempfunc2->name);
				return NULL;
			}
		}
		else
		{
			//函数存在
			//检察参数是否匹配
			HashNode *list = NULL;
			list = Args(child->brother->brother,list);
			
			HashNode *orgList = tempfunc->func->argsDec;

			if (!compArgList(list, orgList))
			{
				
				char str1[256] = "\0";
				char str2[256] = "\0";
				fillArgChar(orgList, str1);
				fillArgChar(list, str2);
				printf("Error type 9 at Line %d: Function \"%s(%s)\" is not applicable for arguments \"(%s)\".\n",child->lineno,tempfunc->name,str1,str2);
			}

			return tempfunc->func->retType;
		}
	}
	else if (!strcmp(child->name, "ID") && !strcmp(child->brother->brother->name, "RP"))
	{
#ifdef DEBUG
		printf("Exp->ID LP RP\n");
#endif
		if (!strcmp(child->value, "read"))
		{
			return NULL;
		}
		HashNode *tempfunc = NULL;
		HashNode *tempfunc2 = hashCheck(globalHashTable, child->value, TYPE_KIND, -1);
		tempfunc = hashCheck(globalHashTable, child->value, FUNC_KIND, -1);
		if (tempfunc == NULL)//函数不存在
		{
			if (tempfunc2 == NULL)
			{

				printf("Error type 2 at line %d: Undefined function \"%s\".\n", child->lineno, child->value);
				return NULL;//返回一个空类型
			}
			else
			{
				printf("Error type 8 at Line %d: \"%s\" is not a function.\n", child->lineno, tempfunc2->name);
				return NULL;
			}
		}
		else
		{


			return tempfunc->type;
		}
	}
	else if (!strcmp(child->name, "Exp"))
	{
		if (!strcmp(child->brother->name, "ASSIGNOP"))
		{
#ifdef DEBUG
			printf("Exp->Exp ASSIGNOP Exp\n");
#endif
			Type *t1 = Exp(child);
			Type *t2 = Exp(child->brother->brother);
			if (t1 == NULL)
				return NULL;
			if (t1->ifCom == 1)
			{
				printf("Error type 6 at line %d: The left-hand side of an assignment must be a variable.\n",child->lineno);
				return NULL;
			}
			return Assignop(t1, t2,child->lineno);
		}
		else if (!strcmp(child->brother->name, "PLUS"))
		{
#ifdef DEBUG
			printf("Exp->Exp PLUS Exp\n");
#endif
			Type *t1 = Exp(child);
			Type *t2 = Exp(child->brother->brother);
			return PLUS(t1, t2, child->lineno);
		}
		else if (!strcmp(child->brother->name, "AND"))
		{
#ifdef DEBUG
			printf("Exp->Exp AND Exp\n");
#endif
			Type *t1 = Exp(child);
			Type *t2 = Exp(child->brother->brother);
			return AND(t1, t2, child->lineno);
		}
		else if (!strcmp(child->brother->name, "OR"))
		{
#ifdef DEBUG
			printf("Exp->Exp OR Exp\n");
#endif
			Type *t1 = Exp(child);
			Type *t2 = Exp(child->brother->brother);
			return OR(t1, t2, child->lineno);
		}
		else if (!strcmp(child->brother->name, "RELOP"))
		{
#ifdef DEBUG
			printf("Exp->Exp RELOP Exp\n");
#endif
			Type *t1 = Exp(child);
			Type *t2 = Exp(child->brother->brother);
			return RELOP(t1, t2, child->lineno);
		}
		else if (!strcmp(child->brother->name, "MINUS"))
		{
#ifdef DEBUG
			printf("Exp->Exp MINUS Exp\n");
#endif
			Type *t1 = Exp(child);
			Type *t2 = Exp(child->brother->brother);
			return MINUS(t1, t2, child->lineno);
		}
		else if (!strcmp(child->brother->name, "STAR"))
		{
#ifdef DEBUG
			printf("Exp->Exp STAR Exp\n");
#endif
			Type *t1 = Exp(child);
			Type *t2 = Exp(child->brother->brother);
			return STAR(t1, t2, child->lineno);
		}
		else if (!strcmp(child->brother->name, "DIV"))
		{
#ifdef DEBUG
			printf("Exp->Exp DIV Exp\n");
#endif
			Type *t1 = Exp(child);
			Type *t2 = Exp(child->brother->brother);
			return DIV(t1, t2, child->lineno);
		}
		else if (!strcmp(child->brother->name, "LB"))
		{
#ifdef DEBUG
			printf("Exp->Exp LB Exp RB\n");
#endif
			Type *t1 = Exp(child);
			if (t1 == NULL)
			{
				return NULL;
			}
			if (t1->kind != INTARRAY_KIND && t1->kind != FLOATARRAY_KIND)
			{
				//这里不是一个数组
				printf("Error type 10 at Line %d: \"%s\" is not an array.\n", child->lineno, t1->name);
				return NULL;
			}
			Type *t2 = Exp(child->brother->brother);
			if (t2->kind != BASIC_KIND)
			{
				printf("Error type 12 at line %d: \"%s\" is not an integer.\n", child->lineno, t2->name);
				return NULL;
			}
			else if (t2->detail.basic != BASIC_INT)
			{
				printf("Error type 12 at line %d: \"%s\" is not an integer.\n", child->lineno , t2->name);
				return NULL;
			}
			return t1->detail.array.elem;
		}
		else if (!strcmp(child->brother->name, "DOT"))
		{
#ifdef DEBUG
			printf("Exp->Exp DOT ID\n");
#endif
			Type *t1 = Exp(child);
			if (t1 == NULL)
			{
				return NULL;
			}
			
			if (t1->kind != STRUCT_KIND)
			{
				printf("Error type 13 at Line %d: Ille1gal use of \".\".\n", child->lineno);
				return NULL;
			}
			FieldList *structure = t1->detail.structure;
			
			while (structure != NULL)
			{
				if (!strcmp(child->brother->brother->value, structure->type->name))
				{
					return structure->type;
				}
				structure = structure->nextFieldList;
			}
			printf("Error type 14 at Line %d: Non-existent field \"%s\".\n", child->lineno, child->brother->brother->value);
			return NULL;
		}
	}
	else if (!strcmp(child->name, "LP"))
	{
#ifdef DEBUG
		printf("Exp->LP Exp RP\n");
#endif
		return Exp(child->brother);
	}
	else if (!strcmp(child->name, "MINUS"))
	{
#ifdef DEBUG
		printf("Exp->MINUS Exp\n");
#endif
		return Exp(child->brother);
	}
	else if (!strcmp(child->name, "NOT"))
	{
#ifdef DEBUG
		printf("Exp->NOT Exp\n");
#endif
		Type *t0 = Exp(child->brother);
		return NOT(t0, child->lineno);
	}
}


HashNode *Args(Node *root,HashNode *List)
{
	//构造一个参数表？
	Node *child = root->child;
	Type *p = Exp(child);
	if (p == NULL)
	{
		return List;
	}
	HashNode *newNode = initHashNode(TYPE_KIND, NULL, 0);
	newNode->type = p;
	newNode->nextInStack = List;
	List = newNode;
	if (child->brother == NULL)
	{
#ifdef DEBUG
		printf("Args->Exp\n");
#endif
	}
	else
	{
#ifdef DEBUG
		printf("Args->Exp COMMA Args\n");
#endif
		List = Args(child->brother->brother,List);
	}
	return List;
}


HashNode *VarDec(Node *root,Type *type,FieldList *structure)
{
    Node *child = root->child;
	if (type == NULL)
	{
		return NULL;
	}
	Type *temp = (Type *)malloc(sizeof(Type));
	memcpy(temp, type, sizeof(Type));
    if(structure != NULL)
    {
        if(!strcmp(child->name,"ID"))
        {
			//需要检察是否已经在structure中定义过
			//-------------填充---------------
			if (checkInFieldList(child->value, structure) != NULL)//该变量名已经使用过
			{
				printf("Error type 15 at line %d: Redefined field \"%s\".\n", child->lineno, child->value);
				return NULL;//?是否可以直接返回未知
			}
			//================================
            #ifdef DEBUG
            printf("struct:VarDec->ID\n");
            #endif
			temp->name = child->value;
			if (structure->type == NULL)
			{
				structure->type = temp;
			}
			else
			{
				FieldList *newFieldList = (FieldList *)malloc(sizeof(FieldList));
				memset(newFieldList, 0, sizeof(FieldList));
				if (type->kind == STRUCT_DEC)
				{
					type->kind = STRUCT_KIND;
				}
				newFieldList->nextFieldList = structure->nextFieldList;
				newFieldList->type = temp;
				structure->nextFieldList = newFieldList;
			}
        }
        else if(!strcmp(child->name,"VarDec"))
        {
            #ifdef DEBUG
            printf("struct:VarDec->VarDec LB INT RB\n");
            #endif
            Type *newType = (Type *)malloc(sizeof(Type));
			memset(newType, 0, sizeof(Type));
            newType -> name = type->name;
            newType -> detail = type->detail;
            newType -> kind = type->detail.basic | 0x30;
            newType->detail.array.elem = type;
            newType->detail.array.size = atoi(child->brother->brother->value);
            VarDec(child,newType,structure);
        }
		return NULL;
    }
    else
    {
		if (!strcmp(child->name, "ID"))
		{
#ifdef DEBUG
			printf("VarDec->ID\n");
#endif
			if (type->kind == STRUCT_DEC)
			{
				type->kind = STRUCT_KIND;
			}
			//该变量仅仅是个基础类型
			HashNode *newHashNode = NULL;
			newHashNode = initHashNode(TYPE_KIND, child->value, globalHashStack->depth);
			newHashNode->type = type;
			type->name = child->value;
			return newHashNode;
		}
		else if (!strcmp(child->name, "VarDec"))
		{
			//该变量是个数组类型，递归构造类型,返回空
#ifdef DEBUG
			printf("VarDec->VarDec LB INT RB\n");
#endif

			Type *newType = (Type *)malloc(sizeof(Type));
			memset(newType, 0, sizeof(Type));
			newType->name = type->name;
			newType->detail = type->detail;
			if (type->kind == BASIC_KIND)
				newType->kind = type->detail.basic | 0x30;
			else
				newType->kind = type->kind;
			newType->detail.array.elem = type;
			newType->detail.array.size = atoi(child->brother->brother->value);
			return VarDec(child, newType, structure);
		}
    }
}
Type *checkInFieldList(char *name, FieldList *structure)
{
	FieldList *temp = structure;
	if (temp->type == NULL)
	{
		return NULL;
	}
	while (temp != NULL)
	{
		if (!strcmp(temp->type->name, name))
		{
			return temp->type;//该变量名在structure中存在
		}
		temp = temp->nextFieldList;
	}
	return NULL;//该变量名在structure中不存在
}

void VarList(Node *root, FuncDec *fun)
{
	Node *child = root->child;

	ParamDec(child, fun);
	if(child->brother!=NULL)
	{
#ifdef DEBUG
		printf("VarList->ParamDec VarList\n");
#endif
		VarList(child->brother->brother, fun);
	}
	else
	{
#ifdef DEBUG
		printf("VarList->ParamDec\n");
#endif
	}

}

void ParamDec(Node *root, FuncDec *fun)
{
	if (fun == NULL)
		return;
#ifdef DEBUG
	printf("ParamDec->Specifier VarDec\n");
#endif
	Node *child = root->child;
	Type *p = NULL;
	p = Specifier(child);
	HashNode *newHashNode = NULL;
	newHashNode = VarDec(child->brother, p, NULL);
	if (newHashNode == NULL)
	{
		return;
	}
	//判断该变量是否已经在参数列表中定义过
	if (checkInFunArgs(newHashNode->name, fun->argsDec))
	{
		printf("Error type 4 at line %d: Redefined variable \"%s\".\n", child->lineno, newHashNode->name);
		deleteHashNode(newHashNode);
		fun = NULL;
		return;
	}


#ifdef INSERTCODE
	code *ret = (code *)malloc(sizeof(code));
	memset(ret, 0, sizeof(code));
	ret->kind = PARAM_CODE;
	operand *o = (operand *)malloc(sizeof(operand));
	memset(o, 0, sizeof(operand));
	o->kind = VAR_OP;
	o->value = newHashNode->name;
	ret->detail.singleop.op = o;
	insertCode(ret);
#endif

	//=================================
	//在参数列表中注册HashNode
	newHashNode->nextInStack = fun->argsDec;
	fun->argsDec = newHashNode;
}


int checkInFunArgs(char *name,HashNode *funArgs)
{
	HashNode *p = funArgs;
	while (p != NULL)
	{
		if (!strcmp(p->name, name))
		{
			return 1;
		}
		p = p->nextInStack;
	}
	return 0;
}


void printHashTable(HashTable *hashtable)
{
	int i = 0;
	for (; i < hashtable->size; i++)
	{
		if (hashtable->table[i] != NULL)
		{
			printf("table[%d]\n", i);
			HashNode *p = hashtable->table[i];
			while (p != NULL)
			{
				if (p->kind == FUNC_KIND)
				{
					printf("Func:name=%s\n", p->name);
				}
				else
				{
					printf("Var:");
					switch (p->type->kind)
					{
					case BASIC_KIND:
						if (p->type->detail.basic == BASIC_INT)
							printf("int ");
						else
							printf("float");
						break;
					case FLOATARRAY_KIND:
						printf("floatarray ");
						break;
					case INTARRAY_KIND:
						printf("intarray ");
						break;
					case STRUCT_KIND:
						printf("Struct ");
						printf("%s ", p->type->detail.structure->name);
						break;
					}
					printf("%s\n", p->name);
				}
				p = p->nextInHash;
			}
		}
	}
}