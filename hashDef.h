/*

Batch No- 30

2011A7PS108P - Rishab Kalra

2011A7PS087P- Ishan Bansal

*/

#ifndef HASHDEF_H
#define HASHDEF_H


#define hashSize 30

struct node1;
typedef struct node* LINK;

typedef struct node
{
	char name[300];
	char tokenName[300];
	LINK next;
	
}Node;

typedef struct
{

	LINK head;
		
}hashTable[hashSize];

#endif
