/*

Batch No- 30

2011A7PS108P - Rishab Kalra

2011A7PS087P- Ishan Bansal

*/

#ifndef STACKDEF_H
#define STACKDEF_H

#include "parserDef.h"

struct stack_node;

typedef struct stack_node* stack_LINK;
typedef struct stack_node
{
	char name[300];
	stack_LINK next;
	stack_LINK prev;
	int isTerminal;
	tree_LINK tl;
}stack_Node;

typedef struct
{
	stack_LINK top;
	int count; 
}stack;

extern void initializeTree();

#endif
