/*

Batch No- 30

2011A7PS108P - Rishab Kalra

2011A7PS087P- Ishan Bansal

*/

#ifndef PARSERDEF_H
#define PARSERDEF_H


#define NUM_RULES 100
#include "hashDef2.h"
#include "lexerDef.h"

typedef struct _box
{
	char symbol[30];
	struct _box *next;
}box;

typedef box *link;
typedef box *head;


typedef struct
{
	head rules[NUM_RULES];
	int numRules;	
}Grammar;

struct tree_node;
typedef struct tree_node* tree_LINK;


typedef struct tree_node
{
	int isTerminal;
	tokenInfo ti;
	int number_child;
	tree_LINK children[20];
}tree_Node;

#endif
