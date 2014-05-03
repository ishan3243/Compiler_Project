/*

Batch No- 30

2011A7PS108P - Rishab Kalra

2011A7PS087P- Ishan Bansal

*/



#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include "hashDef2.h"
#include "set.h"

void createEmptyHashTable(hashTable2 h)
{

	int i;
	for(i=0;i<hashSize2;i++)
	{
		h[i].head=NULL;
	}

}


int hashKey2(char name[])
{
	int len=strlen(name);
	int x=0;
	int a=31;
	int i;
	for( i=0;i<len;i++)
	{
		x=(name[i]+a*x)%hashSize2;
	}
	return x;
}



LINK insert2(hashTable2 h,char name[])
{
	int key=hashKey2(name);
	
	LINK temp=(LINK)malloc(sizeof(Node));
	temp->next=h[key].head;
	strcpy(temp->name,name);
	int len=strlen(name);
	h[key].head=temp;
	
	//initialize the other things
	temp->numRightSideOf = 0;
	temp->numLeftSideOf = 0;
	temp->firstSet = createEmptySet();
	
	if(temp->name[0] == '<' )
	{
		temp->tOrNt = 0;
		temp->followSet = createEmptySet();
		//handle the rhs thing in the place where hash is used.
	}
	
	else
	{
		temp->tOrNt = 1;
	}
	
	return temp;
}

LINK find2(hashTable2 h,char name[])
{
	int key=hashKey2(name);
	LINK cur=h[key].head;
	while(cur!=NULL)
	{
		if(strcmp(cur->name,name)==0)
		{
			return cur;
		}
		cur=cur->next;
	}
	return NULL;
}

void printHashTable(hashTable2 h)
{
	int i=0;
	while(i<hashSize2)
	{
		LINK temp = h[i].head;
		while(temp!=NULL)
		{
			printf("%s				%d	%d",temp->name, temp->index, temp->tOrNt);
			temp=temp->next;
			printf("\n");
		}
		i++;
		
	}
}

