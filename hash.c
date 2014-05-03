/*

Batch No- 30

2011A7PS108P - Rishab Kalra

2011A7PS087P- Ishan Bansal

*/

#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include "hashDef.h"

int hashKey(char name[])
{
	int len=strlen(name);
	int x=0;
	int a=31;
	int i;
	for( i=0;i<len;i++)
	{
		x=(name[i]+a*x)%hashSize;
	}
	return x;
}

void insert(hashTable h,char name[],char tokenName[])
{
	int key=hashKey(name);
	
	LINK temp=(LINK)malloc(sizeof(Node));
	temp->next=h[key].head;
	strcpy(temp->name,name);
	int len=strlen(name);
	temp->name[len]='\0';
	strcpy(temp->tokenName,tokenName);
	len=strlen(tokenName);
	temp->tokenName[len]='\0';
	h[key].head=temp;
	
}

void initializeHashTable(hashTable h)
{
	int i;
	for( i=0;i<hashSize;i++)
	{
		h[i].head=NULL;
	}
		
	char tokenName[300];
	char name[300];
	FILE *fp = fopen("keyWords.txt","r");
	while(!feof(fp))
	{
		if(fscanf(fp,"%s %s",name,tokenName)!=2)
		break;
		insert(h,name,tokenName);
	}
	
}




int find(hashTable h,char name[],char tokenName[])
{
	int key=hashKey(name);
	LINK cur=h[key].head;
	while(cur!=NULL)
	{
		if(strcmp(cur->name,name)==0)
		{
		//printf("token is %s\n",cur->tokenName);
		strcpy(tokenName,cur->tokenName);
		return 1;
		}
		cur=cur->next;
	}
	return 0;
}

