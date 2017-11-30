#include "Node.h"
#include "newHashTable.h"
void Program(Node *root);
void ExtDefList(Node *root);
void ExtDef(Node *root);
Type *Specifier(Node *root);
Type *StructSpecifier(Node *root);
FuncDec *FunDec(Node *root, Type *p);
void ExtDecList(Node *root, Type *p);
void DefList(Node *root, FieldList *structure);
Type* Def(Node *root, FieldList* structure);
void DecList(Node *root, Type *type, FieldList *structure);
void Dec(Node *root, Type *type, FieldList *structure);
HashNode *VarDec(Node *root, Type *type, FieldList *structure);
Type *checkInFieldList(char *name, FieldList *structure);
void VarList(Node *root, FuncDec *fun);
void ParamDec(Node *root, FuncDec *fun);
int checkInFunArgs(char *name, HashNode *funArgs);



void printHashTable(HashTable *hashtable);
