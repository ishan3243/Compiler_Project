/*

Batch No- 30

2011A7PS108P - Rishab Kalra

2011A7PS087P- Ishan Bansal

*/


#include "stackDef.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

stack initializeStack(stack s)
{
	stack_LINK temp=(stack_LINK)malloc(sizeof(stack_Node));
	strcpy(temp->name,"$");
	temp->isTerminal=1;
	temp->tl=NULL;
	s.top=temp;
	temp->next=NULL;
	temp->prev=NULL;
	s.count=1;
	return s;
}

stack push(stack s,char name[],int isTerminal,tree_LINK tl)
{
	stack_LINK temp=(stack_LINK)malloc(sizeof(stack_Node));
	temp->next=NULL;
	temp->tl=tl;
	temp->isTerminal=isTerminal;
	strcpy(temp->name,name);
	temp->prev=s.top;
	(s.top)->next=temp;
	s.top=temp;
	
	s.count++;
	return s;
}

stack_Node pop(stack *s)
{
	stack_LINK temp;
	stack_Node def;
	strcpy(def.name,"error");
	if((*s).count==0)
	{
		printf("Error: Stack is empty\n");
		return def;
	}
	temp=(*s).top;
	stack_Node t=*temp;
	
	(*s).top=temp->prev;
	if((*s).top!=NULL)
	(*s).top->next=NULL;
	free(temp);
	(*s).count--;
	return t;
}

void printStack(stack s)
{
	stack_LINK cur=s.top;
	while(cur!=NULL)
	{
		printf("%s\n",cur->name);
		cur=cur->prev;
	}
}

int isStackEmpty(stack s)
{
	return (s.count==0);
}

