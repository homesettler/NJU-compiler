#include "hashTable.h"

unsigned int hash(char *name, int size)
{
    int val = 0,i;
    for(;*name;++name)
    {
        val = (val << 2) + *name;
        if(i = val & ~size)
            val = (val ^ (i >> 12)) & size;
    }
    return val;
}


HashTable *hashInit(int size)
{
    HashTable *hashTable = (HashTable *)malloc(sizeof(HashTable));
    hashTable -> size = size;
    hashTable ->table = (HashNode **)malloc(sizeof(HashNode *)*size);
    hashTable ->hashFind = hashFind;
    hashTable ->hashPush = hashPush;
    int i = 0;
    for(;i<size;i++)
    {
        hashTable->table[i] = NULL;
    }
    return hashTable;
}

int hashPush(HashTable *hashTable,char *name)
{
    if(hashFind(hashTable,name) != NULL)
    {
        return 0;
    }
    HashNode *p = (HashNode *)malloc(sizeof(HashNode));
    strcpy(p->name, name);
    p->next = NULL;
    int address = hash(name,hashTable->size);
    if(hashTable->table[address] == NULL)
    {  
        hashTable->table[address] = p;
        return 1;
    }
    else
    {
        HashNode *temp = hashTable->table[address];
        while(temp->next!=NULL)
        {
            temp = temp->next;
        }
        temp->next = p;
    }

}


HashNode *hashFind(HashTable *hashTable,char *name)
{
    int address = hash(name,hashTable->size);
    HashNode *temp = hashTable->table[address];
    while(temp!=NULL)
    {
        if(strcmp(temp->name,name)==0)
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

