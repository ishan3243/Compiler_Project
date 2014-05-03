/*

Batch No- 30

2011A7PS108P - Rishab Kalra

2011A7PS087P- Ishan Bansal

*/


#ifndef STACK_H
#define STACK_H

#include "stackDef.h"

extern stack initializeStack(stack s);

extern stack push(stack s,char name[],int isTerminal,tree_LINK tl);

extern stack_Node pop(stack *s);

extern int isStackEmpty(stack s);

extern void print(stack s);

#endif
