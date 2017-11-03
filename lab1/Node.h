#ifndef __NODE_H__
#define __NODE_H__

#include "stdio.h"
#include "string.h"
typedef struct node
{
    char name[16];
    char value[32];
    struct node* child;
    struct node* brother; 
}Node;
Node *initNode(char *name, char* value);
void addChild(Node *parent, Node* child);
void printTree(Node* root, int n);

#endif