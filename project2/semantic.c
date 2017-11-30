#include "newHashTable.h"
#include "Node.h"
#include "semantic.h"
#define DEBUG
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

void ExtDef(Node *root)
{
	Node *child = root->child;
	Type *type = NULL;
	type = Specifier(child);//获取类型
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
			if (tempfun->isDef == 1)//这里已经定义过，但接下来还要定义
			{
				printf("Error type 4 at Line %d: Redefine functiion \"%s\"\n", child->lineno, child->value);
				return;//不要继续往下执行了。
			}
			globalHashStack = stackPush(globalHashStack);//放入一个新的栈，进行函数内部处理
			//这里需要把函数的参数压入栈下挂的链表中
			//--------------填充----------------
			//==================================
			CompSt();
		}

#ifdef DEBUG
		printHashTable(globalHashTable);
#endif
    }
}

void CompSt()
{

}
FuncDec *FunDec(Node *root, Type *p)
{
    Node *child = root->child;
    //这里进行函数名检查是否重复以及注册HashNode
	HashNode *res = hashCheck(globalHashTable, child->name, FUNC_KIND, globalHashStack->depth + 1);
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
		newFuncDec->retType = p;//返回值类型确定。
		newFuncDec->isDef = 0;//现在先假设未定义。
		HashNode *newFuncNode = initHashNode(FUNC_KIND, newFuncDec->name, globalHashStack->depth);
		newFuncNode->func = newFuncDec;
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
    //这里进行变量名检查是否重复以及注册HashNode

    //====================================
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

        //==================
        type = (Type *)malloc(sizeof(Type));
		memset(type, 0, sizeof(Type));

        if(child->child!=NULL)
            type->name = child->child->value;
        else
            type->name = NULL;
        type->kind = STRUCT_KIND;
        FieldList *structure = NULL;
        structure = (FieldList *)malloc(sizeof(FieldList));
		memset(structure, 0, sizeof(FieldList));
        structure -> name = child->child->value;
        structure -> type = NULL;
        structure ->nextFieldList = NULL;
        type->detail.structure = structure;

        DefList(child->brother->brother,structure);
    }
    else if(!strcmp(child->name,"Tag"))
    {
        #ifdef DEBUG
        printf("StructSpecifier->STRUCT Tag");
        #endif
        //这里应该检查是否未定义

        //==================

        type = (Type *)malloc(sizeof(Type));
		memset(type, 0, sizeof(Type));

        type -> name = child->child->value;
        type -> kind = STRUCT_KIND;
        type -> detail.structure = NULL;
    }
    return type;
}

void DefList(Node *root,FieldList *structure)
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

    }
}

Type* Def(Node *root,FieldList* structure)
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

    }
}

HashNode *VarDec(Node *root,Type *type,FieldList *structure)
{
    Node *child = root->child;
    if(structure != NULL)
    {
        if(!strcmp(child->name,"ID"))
        {
			//需要检察是否已经在structure中定义过
			//-------------填充---------------
			if (checkInFieldList(child->value, structure) != NULL)//该变量名已经使用过
			{
				printf("Error type 15 at line %d: Redefined field \"%s\"", child->lineno, child->value);
				return NULL;//?是否可以直接返回未知
			}
			//================================
            #ifdef DEBUG
            printf("struct:VarDec->ID\n");
            #endif
            FieldList *newFieldList = (FieldList *)malloc(sizeof(FieldList));
			memset(newFieldList, 0, sizeof(FieldList));

            newFieldList->nextFieldList = structure->nextFieldList;
            newFieldList->type = type;
            newFieldList->name = child->value;
            structure->nextFieldList = newFieldList;

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
			//该变量仅仅是个基础类型
			HashNode *newHashNode = initHashNode(TYPE_KIND, child->value, globalHashStack->depth);
			newHashNode->type = type;
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
			newType->kind = type->detail.basic | 0x30;
			newType->detail.array.elem = type;
			newType->detail.array.size = atoi(child->brother->brother->value);
			VarDec(child, newType, structure);
			return NULL;
		}
    }
}
Type *checkInFieldList(char *name, FieldList *structure)
{
	FieldList *temp = structure;
	while (temp != NULL)
	{
		if (!strcmp(temp->name, name))
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
#ifdef DEBUG
#endif
	ParamDec(child, fun);
	if(child->brother!=NULL)
	{
		VarList(child->brother->brother, fun);
	}
}

void ParamDec(Node *root, FuncDec *fun)
{
	Node *child = root->child;
	Type *p = NULL;
	p = Specifier(child);
	HashNode *newHashNode = VarDec(child->brother,p,NULL);
	//判断该变量是否已经在参数列表中定义过
	if (checkInFunArgs(newHashNode->name, fun->argsDec))
	{
		printf("Error type 4 at line %d: Redefined variable \"%s\"\n", child->lineno, newHashNode->name);
		deleteHashNode(newHashNode);
		return;
	}
	//=================================
	//在参数列表中注册HashNode
	HashNode *tempHashNode = fun->argsDec;
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
					printf("Func:name=%s", p->name);
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
					printf("%s ", p->name);
				}
				p = p->nextInHash;
			}
		}
	}
}