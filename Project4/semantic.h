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
void Def(Node *root, FieldList* structure);
void DecList(Node *root, Type *type, FieldList *structure);
void Dec(Node *root, Type *type, FieldList *structure);
HashNode *VarDec(Node *root, Type *type, FieldList *structure);
Type *checkInFieldList(char *name, FieldList *structure);
void VarList(Node *root, FuncDec *fun);
void ParamDec(Node *root, FuncDec *fun);
int checkInFunArgs(char *name, HashNode *funArgs);
void CompSt(Node *root,Type *retType, Stack *globalstack);
HashNode *Args(Node *root,HashNode *List);
void StmtList(Node *root, Type *retType);
Type *Assignop(Type *t1, Type *t2, int number);
void Stmt(Node *root, Type *retType);
Type *Exp(Node *root);
HashNode *Args(Node *root, HashNode *List);


void printHashTable(HashTable *hashtable);
