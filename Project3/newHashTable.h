#ifndef __NEWHASHTABLE__
#define __NEWHASHTABLE__

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef struct Type_ Type;
typedef struct FieldList_ FieldList;
typedef struct FuncDec_ FuncDec;
typedef struct Stack_ Stack;
typedef struct HashNode_ HashNode;
typedef struct HashTable_ HashTable;

#define HASH_TABLE_SIZE 0x3fff

#define BASIC_KIND 0x10
#define BASIC_INT 0x11
#define BASIC_FLOAT 0x12

#define ARRAY_KIND 0x30
#define INTARRAY_KIND 0x31
#define FLOATARRAY_KIND 0x32


#define STRUCT_KIND 0x70
#define STRUCT_DEC 0x71

extern HashTable *globalHashTable;
extern Stack *globalHashStack;
struct Type_
{
    char *name;
    int kind;//数据类型
    union
    {
        int basic;
        struct
        {
            Type *elem;
            int size;
        }array;
        FieldList *structure;
    }detail;//类型的详细信息
	int ifCom;
};

struct FieldList_
{
    char *name;
    Type *type;
    FieldList *nextFieldList;
}; 

struct FuncDec_
{
    char *name;
    Type *retType;//返回值类型
    HashNode *argsDec;//参数列表
    int isDef;//是否定义
};

struct Stack_
{
    HashNode *start;
    Stack* nextStack;
    int depth;
};

#define TYPE_KIND 0x100
#define FUNC_KIND 0x200
struct HashNode_
{
    char *name;
    int kind;
    union
    {
        Type *type;
        FuncDec *func;
    };
    int depth;//变量的作用域深度
    HashNode *nextInHash;//闭散列链表的下一个项
    HashNode *preInHash;//闭散列链表的上一个项
    HashNode *nextInStack;
};

struct HashTable_
{
    HashNode **table;
    int size;
};


HashNode *initHashNode(int kind,char *name,int depth);
HashTable *initHashTable(int size);

unsigned int hash(int size,char *name);

int hashPush(HashTable *hashTable,HashNode *node);

Stack *stackPush(Stack *top);

void deleteHashNode(HashNode *node);
void deleteFunc(FuncDec *fundec);
void deleteType(Type *type);
void deleteFieldList(FieldList *structure);

Stack *stackPop(Stack *top);
HashNode *hashCheck(HashTable *hashTable, char *name, int kind, int depth);
int typeCmp(Type *t1,Type *t2);
int FieldCmp(FieldList *f1, FieldList *f2);
#endif