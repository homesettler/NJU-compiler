#include "Node.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
Node* initNode(char *name,char *value)
{
    Node *p = (Node *)malloc(sizeof(Node));
    strcpy(p->name, name);
    strcpy(p->value, value);
    p->brother = NULL;
    p->child = NULL;
    return p;
}

void addChild(Node *parent, Node* child)
{
    if(parent!=NULL&&child!=NULL)
    {
        child->brother = parent->child;
        parent->child = child;
    }
}

void printTree(Node *root, int n)
{
    if(root == NULL)
        return;
}