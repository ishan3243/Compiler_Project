/*

Batch No- 30

2011A7PS108P - Rishab Kalra

2011A7PS087P- Ishan Bansal

*/


#include<stdlib.h>
#include<string.h>
#include "setDef.h"
#include<stdio.h>

set createEmptySet()
{
	set s; int i;
	s.size = 0;
	return s;
}

void destroySet(set s)
{
	int i;
	for(i=0;i<s.size;i++)
		free(s.arr[i]);
	return ;
}

set insertSet (set s, char *c)
{
	s.arr[s.size] = (char *)malloc(sizeof(char)*MAX_SIZE);
	strcpy(s.arr[s.size], c);
	s.size++;
	return s;
}

set setUnion(set first, set second)  //adds second to first
{
	//printf("trololol\n");
	int i;
	
	for(i=0;i<second.size;i++)
		if(!isMember(first, second.arr[i]))
			first=insertSet(first, second.arr[i]);	
	
	//printf("lollol\n");
	return first;	

}

void printSet(set s)
{
	printf("PRINTING SET %d\n", s.size);
	int i;
	for(i=0;i<s.size;i++)
	printf("%s\n", s.arr[i]);
}

int isMember(set s, char elem[])
{
	int i;
	for(i=0;i<s.size;i++)
	{
		if(strcmp(s.arr[i], elem)==0) return 1;	
	}
	return 0;
}

int isEmpty(set s)
{
	if(s.size == 0)	return 1;
	return 0;
}

set removeElement(set s, char elem[])
{
	int i;
	set s1 = createEmptySet();
	for(i=0;i<s.size;i++)
	{
		if(strcmp(s.arr[i], elem) != 0)
		s1 = insertSet(s1, s.arr[i]) ;
	}
	return s1;
}



