#include "stdio.h"
#include "stdlib.h"
#include "string.h"
int HashSize = 0x3fff;

#define NameSize 100
typedef struct hashnode
{
    char name[NameSize];
    struct hashnode *next;
}HashNode;

typedef struct hashtable
{
    HashNode **table;
    int size;
    int (*hashPush)(struct hashtable *hashTable,char *name);
    HashNode* (*hashFind)(struct hashtable *hashTable,char *name);
}HashTable;

unsigned int hash(char *name,int size);
HashTable *hashInit(int size);
int hashPush(HashTable *hashTable,char *name);
HashNode *hashFind(HashTable *hashTable,char *name);

