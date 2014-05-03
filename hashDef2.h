/*

Batch No- 30

2011A7PS108P - Rishab Kalra

2011A7PS087P- Ishan Bansal

*/

#ifndef HASHDEF_H
#define HASHDEF_H


#define hashSize2 200
#define MAX 50
#include "setDef.h"

struct node;
typedef struct node* LINK;

typedef struct node
{
	char name[30];	      //this is the hash-key, the name of the teminal or the non-terminal.
	int index;	
	int tOrNt; 	      //terminal (tOrNt==1) or non-terminal (tOrNt==0)
	
	set firstSet;
	set followSet;
	int rightSideOf[40];  //stores the rule no. on whose right side the terminal appears.
	int leftSideOf[20];
	int numRightSideOf;   //no. of rules in which it appears on the right side.
	int numLeftSideOf;
	LINK next;
}Node;

typedef struct
{

	LINK head;
		
}hashTable2[hashSize2];


#endif
