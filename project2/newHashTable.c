#include "newHashTable.h"

HashTable *globalHashTable = NULL;
Stack *globalHashStack = NULL;

//Hash值计算
unsigned int hash(int size,char* name)
{
	int val = 0, i;
	for (; *name; ++name)
	{
		val = (val << 2) + *name;
		if (i = val & ~size)
			val = (val ^ (i >> 12)) & size;
	}
	return val;
}
//构建HashTable
HashTable *initHashTable(int size)
{
	HashTable *newHashTable = NULL;
	newHashTable = (HashTable *)malloc(sizeof(HashTable));
	newHashTable->size = size;
	newHashTable->table = (HashNode **)malloc(sizeof(HashNode *)*size);
	int i = 0;
	for (; i < size; i++)
	{
		newHashTable->table[i] = NULL;
	}
	return newHashTable;
}

//初始化HashNode，其内部指向Type的指针还需自行指定
HashNode *initHashNode(int kind, char *name, int depth)
{
	HashNode *newHashNode = NULL;
	newHashNode = (HashNode *)malloc(sizeof(HashNode));
	newHashNode->depth = depth;
	newHashNode->kind = kind;
	newHashNode->name = name;
	newHashNode->nextInHash = NULL;
	newHashNode->preInHash = NULL;
	newHashNode->nextInStack = NULL;
	return newHashNode;
}


//将一个HashNode放入到HashTable里，要现检察是否在表中已经存在
int hashPush(HashTable *hashTable, HashNode *node)
{
	int address = hash(hashTable->size, node->name);
	if (!hashCheck(hashTable, node->name, node->kind, node->depth))
	{
		HashNode *temp = hashTable->table[address];
		if(temp == NULL)
		{
			hashTable->table[address] = node;
		}
		else
		{
			while (temp->nextInHash != NULL)
			{
				temp = temp->nextInHash;
			}
			temp->nextInHash = node;
			node->preInHash = temp;
		}
		return 1;
	}
	return 0;
}

//检察一个元素是否在表中（depth也应相同），当是FUNC_KIND时检察其是否只是声明
HashNode *hashCheck(HashTable *hashTable, char *name, int kind, int depth)
{
	int address = hash(hashTable->size, name);
	HashNode *temp = hashTable->table[address];
	while (temp != NULL)
	{
		if (!strcmp(name, temp->name) && temp->depth == depth&&kind == temp->kind)
		{
			return temp;//找到了对应Hash节点
		}
	}
	return NULL;
}

//新建一个Stack节点,输入参数为当前栈顶指针，返回值为新的栈顶指针，
Stack *stackPush(Stack *top)
{
	Stack *newStack = NULL;
	newStack = (Stack *)malloc(sizeof(Stack));
	newStack->nextStack = NULL;
	newStack->start = NULL;
	if (top == NULL)
	{
		newStack->depth = 0;
		top = newStack;
	}
	else
	{
		newStack->depth = top->depth + 1;
		newStack ->nextStack = top;
		top = newStack;
	}
	return top;
}

//从栈中弹出一条链，依照指针依次删除这条链上所有的HashNode(包含的Type或fundec也需一并删除，Type中也可能有数据。。)，返回新的栈顶指针
Stack *stackPop(Stack *top)
{
	HashNode *t = top->start;
	while (t != NULL)
	{
		HashNode *dtemp = t;
		t = t->nextInStack;
		deleteHashNode(dtemp);
	}
}
void deleteFieldList(FieldList *structure)
{
	if (structure == NULL)
	{
		return;
	}
	deleteType(structure->type);
	deleteFieldList(structure->nextFieldList);
	free(structure);
}
void deleteType(Type *type)
{
	Type *temp = type;
	if (temp->kind == BASIC_KIND)
	{
		free(temp);
		temp = NULL;
	}
	else if (temp->kind & 0x30 == ARRAY_KIND)
	{
		Type *arraychild = temp->detail.array.elem;
		while (arraychild != NULL)
		{
			Type *tempChild = arraychild;
			arraychild = tempChild->detail.array.elem;
			free(tempChild);
		}
		temp = NULL;
	}
	else if (temp->kind == STRUCT_KIND)
	{
		deleteFieldList(type->detail.structure);
	}	
}

void deleteFunc(FuncDec *fundec)
{
	FuncDec *tempfundec = fundec;
	//该函数不为空
	if (fundec != NULL)
	{
		//删除返回值类型
		if (tempfundec->retType != NULL)
		{
			deleteType(tempfundec->retType);
		}
		//删除参数表
		if (tempfundec->argsDec != NULL)
		{
			while (tempfundec->argsDec != NULL)
			{
				HashNode *node = tempfundec->argsDec;
				tempfundec->argsDec = node->nextInStack;
				deleteHashNode(node);
			}
		}
	}
}

void deleteHashNode(HashNode *node)
{
	if (node != NULL)
	{
		//该节点在闭散列的前一个点
		if (node->preInHash != NULL)
		{
			node->preInHash->nextInHash = node->nextInHash;
		}
		//该节点在闭散列的后一个点
		if (node->nextInHash != NULL)
		{
			node->nextInHash->preInHash = node->preInHash;
		}
		//该节点的类型，若为Type则按照Type删除，若为Func则按照Func删除。
		if (node->kind == TYPE_KIND)
		{
			deleteType(node->type);
		}
		else if (node->kind == FUNC_KIND)
		{
			deleteFunc(node->func);
		}
	}
}
