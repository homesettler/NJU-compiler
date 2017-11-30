#include "newHashTable.h"

HashTable *globalHashTable = NULL;
Stack *globalHashStack = NULL;

//Hashֵ����
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
//����HashTable
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

//��ʼ��HashNode�����ڲ�ָ��Type��ָ�뻹������ָ��
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


//��һ��HashNode���뵽HashTable�Ҫ�ּ���Ƿ��ڱ����Ѿ�����
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

//���һ��Ԫ���Ƿ��ڱ��У�depthҲӦ��ͬ��������FUNC_KINDʱ������Ƿ�ֻ������
HashNode *hashCheck(HashTable *hashTable, char *name, int kind, int depth)
{
	int address = hash(hashTable->size, name);
	HashNode *temp = hashTable->table[address];
	while (temp != NULL)
	{
		if (!strcmp(name, temp->name) && temp->depth == depth&&kind == temp->kind)
		{
			return temp;//�ҵ��˶�ӦHash�ڵ�
		}
	}
	return NULL;
}

//�½�һ��Stack�ڵ�,�������Ϊ��ǰջ��ָ�룬����ֵΪ�µ�ջ��ָ�룬
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

//��ջ�е���һ����������ָ������ɾ�������������е�HashNode(������Type��fundecҲ��һ��ɾ����Type��Ҳ���������ݡ���)�������µ�ջ��ָ��
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
	//�ú�����Ϊ��
	if (fundec != NULL)
	{
		//ɾ������ֵ����
		if (tempfundec->retType != NULL)
		{
			deleteType(tempfundec->retType);
		}
		//ɾ��������
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
		//�ýڵ��ڱ�ɢ�е�ǰһ����
		if (node->preInHash != NULL)
		{
			node->preInHash->nextInHash = node->nextInHash;
		}
		//�ýڵ��ڱ�ɢ�еĺ�һ����
		if (node->nextInHash != NULL)
		{
			node->nextInHash->preInHash = node->preInHash;
		}
		//�ýڵ�����ͣ���ΪType����Typeɾ������ΪFunc����Funcɾ����
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
