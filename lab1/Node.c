#include "Node.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
extern int yylineno;

Node* initNode(char *name,char *value)
{
    Node *p = (Node *)malloc(sizeof(Node));
    strcpy(p->name, name);
    strcpy(p->value, value);
    p->brother = NULL;
    p->child = NULL;
    p->lineno = yylineno;
    return p;
}

void addChild(Node *parent, Node* child)
{
    if(parent!=NULL&&child!=NULL)
    {
        child->brother = parent->child;
        parent->child = child;
        parent->lineno = child->lineno;
    }
}

void printTree(Node *root, int n)
{
    if(root == NULL)
        return;
	for(int i=0; i<n; i++)
		printf("  ");
    if(root->child == NULL)
    {
		if(strcmp(root->name,"FLOAT") == 0)
			printf("FLOAT: %s\n", root->value);
		else if(strcmp(root->name,"INT") == 0)
			printf("INT: %s\n", root->value);
		else if(strcmp(root->name,"ID") == 0||strcmp(root->name,"TYPE") == 0)
			printf("%s: %s\n", root->name, root->value);
		else
			printf("%s\n", root->name);
	}
    else
    {
		printf("%s (%d)\n", root->name, root->lineno);
		Node* p = root->child;
        while(p != NULL)
        {
			printTree(p, n+1);
			p = p->brother;
		}
	}
}
