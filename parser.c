/*

Batch No- 30

2011A7PS108P - Rishab Kalra

2011A7PS087P- Ishan Bansal

*/


#include<stdio.h>
#include "parserDef.h"
#include<string.h>
#include<stdlib.h>
#include "set.h"
#include "stack.h"
#include "lexer.h"
#include "hashDef2.h"
#include "hash2.h"
#include "stackDef.h"
#include "setDef.h"
#include "lexer.h"

tree_LINK root;

void initializeTree()
{
	root=(tree_LINK)malloc(sizeof(tree_Node));
	tokenInfo ti;
	strcpy(ti.name,"<mainFunction>");
	root->ti=ti;
	root->number_child=0;
	root->isTerminal=0;
	
	int i;
	for(i=0;i<20;i++)
	{
		root->children[i]=NULL;
	}
}



tree_LINK createTreeNode(tokenInfo ti,int isTerminal)
{
	tree_LINK temp=(tree_LINK)malloc(sizeof(tree_Node));
	temp->ti=ti;
	temp->number_child=0;
	temp->isTerminal=isTerminal;
	
	int i;
	for(i=0;i<20;i++)
	{
		temp->children[i]=NULL;
	}
	
	return temp;
}


int checkUnknownSymbol(char lex[])
{
	int i,j;
	char symbols[] = {'=', '#', '_' , ' ', ']', '[' , '(', ')', ';', ',', '+', '-', '*', '/','@', '.', '<', '>','"'};
	int len = strlen(lex);
	int flag = 0;
	
	for(i=0;i<len;i++)
	{
		flag=0;
		if((lex[i]>='a' && lex[i] <='z')  || (lex[i]>='A' && lex[i] <='Z')  ||  (lex[i]>='0' && lex[i] <='9') ) continue; 
		
		else 
		for(j=0;j<19;j++)
		{	
			if(lex[i]!=symbols[j]) {continue;} 
			else {flag =1; break;}
		}
		
		if(!flag)	return i;
		
		else continue;
	}
	return -1;
}

void printErrors(errorArray e)
{
	int i;
	
	for(i=0;i<e.numErrors;i++)
	{
		
		if(e.ti[i].tokenNo ==1)
		{
			
			if(e.ti[i].val.charVal[0] == '"')
			printf("ERROR_%d : String at line %d is longer than the prescribed length of 20 characters", i+1, e.ti	[i].lineNo);
			else printf("ERROR_%d : Identifier at line %d is longer than the prescribed length of 20 characters", i+1, e.ti	[i].lineNo);
			continue;
		}
		
		int idx = checkUnknownSymbol(e.ti[i].val.charVal);
		if(idx+1)
		{
			printf("ERROR_%d: Unknown Symbol %c at line %d\n", i+1, e.ti[i].val.charVal[idx], e.ti[i].lineNo);
			printf("ERROR_%d: Unknown pattern %s at line %d\n", i+1, e.ti[i].val.charVal, e.ti[i].lineNo);
		}
		else
		{
			printf("ERROR_%d: Unknown pattern %s at line %d\n", i+1, e.ti[i].val.charVal, e.ti[i].lineNo);
		}
	
		
	}
}


void continueLexical(FILE* fp, buffer b[], int *beginLexeme,int *lineNo, int *tokenNo, errorArray *e)
{
/* If errors are there in lexical  analysis phase, then this function is called
*/
	tokenInfo tok;
	while(strcmp(tok.name,"$")!=0)
		{
			tok=getNextToken(fp,b,beginLexeme,lineNo,tokenNo, e);
			//printToken(tok);
		}
		printErrors(*e);
}

tree_LINK parseInputSourceCode(char *testcaseFile,int T[100][100],hashTable2 h,Grammar g,int caseType)
{
/* parsing input code using stack and parse tree*/

	errorArray e;
	e.numErrors= 0;
	//tokenInfo ti;
	FILE* fp=fopen(testcaseFile,"r");
	tokenInfo tok;
	tokenInfo epsilon_tok;
	strcpy(epsilon_tok.name,"EPSILON");
	strcpy(tok.name,"a");
	int j;
	int countTokens=0;
	
	
	tokenInfo tokenArray[100];
	
	
	
	stack s;
	int index;
	s=initializeStack(s);
	s=push(s,"<mainFunction>",0,NULL);
	initializeTree();    	// initializing tree
	stack_Node stackTokenPop;
	//stack_Node stackTokenName;
	char toPushStack[20][300];
	tree_LINK parent=NULL;
	tree_LINK child;
	tokenInfo treeToken;
	int flag=1;
	int in=0;
	int start=1;
	int lineNo=1;
	int tokenNo=0;
	buffer b[BUFF_SIZE];
	int beginLexeme=0;
	
	fp = getStream(fp, b, BUFF_SIZE); 
	
	if(caseType==1)
	{
		while(strcmp(tok.name,"$")!=0)
		{
			tok=getNextToken(fp,b,&beginLexeme,&lineNo,&tokenNo, &e); // reading tokens for option1 
			printToken(tok);
		}
		printErrors(e);
		return root;
	}
	
	tok=getNextToken(fp,b,&beginLexeme,&lineNo,&tokenNo, &e);
	if(e.numErrors>0)
	{
		printf("Lexical error: Parsing cannot be continued further. The following are the lexical errors:-\n\n");
		continueLexical(fp,b,&beginLexeme,&lineNo,&tokenNo, &e); // printing lexical errors
		return root;
	}
	
	
	
	
	
	
	
	while(!isStackEmpty(s))
	{
	
				
		
		
				
		stackTokenPop=pop(&s);
	
		if(strcmp("$",tok.name)==0&&(strcmp("$",stackTokenPop.name)!=0))
		{
			flag=0;
			break;
		}
		if(strcmp("$",stackTokenPop.name)==0)     // if top of stack is $ then stack is empty
		{
			flag=1;
			if(strcmp("$",tok.name)==0)
			{
				printf("Parsing Successful\n");
				break;
			}
			else
			{
				printf("Parsing Error at line %d: Parsing cannot be continued, Stack is empty and tokens are left\n\n\n",tok.lineNo);
				printf("The following are the lexical errors:-\n\n\n");
				continueLexical(fp,b,&beginLexeme,&lineNo,&tokenNo, &e);
				break;
			}
			
		}
		parent=stackTokenPop.tl;
		
	
		 
		LINK stackTokenLink=find2(h,stackTokenPop.name);
		index=stackTokenLink->index;
		
		 
		int tokenIndex=find2(h,tok.name)->index;
		int ruleNo=T[index][tokenIndex];
		
		if(stackTokenLink->tOrNt) // if top of stack is Terminal match the input token
		{
		
			if(strcmp(stackTokenPop.name,"EPSILON")==0)
			{
			if(parent!=NULL)
				{
				child=createTreeNode(epsilon_tok,1);
				parent->children[(parent->number_child)++]=child;
				}
			continue;
			}
			if(strcmp(tok.name,stackTokenPop.name)==0)
			{
				
				
				if(parent!=NULL)
				{
				child=createTreeNode(tok,1);
				parent->children[(parent->number_child)++]=child;
				}
				
			}
			else       // if top of stack is Non Terminal then pop it, and push corresponding to entry in parsing table
			{
				switch(tok.type)
				{
					case INTEGER:
					printf("Syntax Error in parser at line %d: The token %s for %d does not match at line %d. The expected token here is %s\n\n\n",tok.lineNo,tok.name,tok.val.integerVal,tok.lineNo,stackTokenPop.name);
					continueLexical(fp,b,&beginLexeme,&lineNo,&tokenNo, &e);
					return root;
					break;
					
					case REAL:
					printf("Syntax Error in parser at line %d: The token %s for %lf does not match at line %d. The expected token here is %s\n\n\n",tok.lineNo,tok.name,tok.val.realVal,tok.lineNo,stackTokenPop.name);
					continueLexical(fp,b,&beginLexeme,&lineNo,&tokenNo, &e);
					return root;
					break;
					
					case STRING:
					printf("Syntax Error in parser at line %d: The token %s for %s does not match at line %d. The expected token here is %s\n\n\n",tok.lineNo,tok.name,tok.val.charVal,tok.lineNo,stackTokenPop.name);
					continueLexical(fp,b,&beginLexeme,&lineNo,&tokenNo, &e);
					return root;
					break;
					
				}
				
			}
		
			tok=getNextToken(fp,b,&beginLexeme,&lineNo,&tokenNo, &e);
		
		}
		else
		{
	
			strcpy(treeToken.name,stackTokenPop.name);
			child=createTreeNode(treeToken,0);
			if(parent!=NULL)
			{
			
			parent->children[(parent->number_child)++]=child;
			}
			else
			{
			root=child;
			}
			if(ruleNo==-1)
			{
			printf(" Syntax Error in parsing at line %d: No rule defined for terminal %s and nonterminal %s in Parsing Table\n",tok.lineNo,tok.name,stackTokenPop.name);
			continueLexical(fp,b,&beginLexeme,&lineNo,&tokenNo, &e);
			return root;
			break;
			
			}
				link cur=g.rules[ruleNo];
				
				cur=(g.rules[ruleNo])->next;
				int c=0;
				while(cur!=NULL)
				{
					
					strcpy(toPushStack[c++],cur->symbol);
					cur=cur->next;	
				}
				
				for(j=c-1;j>=0;j--)
				{
					LINK ind=find2(h,toPushStack[j]);
					s=push(s,ind->name,ind->tOrNt,child);
					
				}
				
			
		}
	}
	fclose(fp);
	if(flag==0)
	{
	
		printf("Error in parsing: Complete tokens read,but stack is not empty\n");
		continueLexical(fp,b,&beginLexeme,&lineNo,&tokenNo, &e);

	}
	

	
	return root;
}

void printParseTree(tree_LINK PT,FILE* fp,char parentName[])
{

/*
	printing Parse tree.
	
	If node is nonTerminal then lineNo is default -1
	lexemeCurrent Node is ---
	token is empty
	valueIfNumber is -1
	
	
	If node is terminal then and it is not a number then 		default is -1 
*/

	if(PT==NULL)
	return;
	
	value lexemeCurrentNode;
	int lineNo=-1;
	char token[300]="empty";
	char nodeSymbol[300]="empty";
	int valueIfNumber=-1;
	double valueIfReal=-1;
	char parentNodeSymbol[300];
	strcpy(parentNodeSymbol,parentName);
	int isLeafNode;
	
	if(PT->isTerminal)
	{
		lineNo=PT->ti.lineNo;
		isLeafNode=1;
		strcpy(token,(PT->ti).name);
		if(PT->ti.type==INTEGER)
		{
			lexemeCurrentNode.integerVal=(PT->ti).val.integerVal;
			valueIfNumber=(PT->ti).val.integerVal;
			fprintf(fp,"%d	%d	%s	%d	%s	%d	%s\n\n",lexemeCurrentNode.integerVal,lineNo,token,valueIfNumber,parentNodeSymbol,isLeafNode,nodeSymbol);
		}
		else if(PT->ti.type==REAL)
		{
			lexemeCurrentNode.realVal=(PT->ti).val.realVal;
			valueIfReal=(PT->ti).val.realVal;
			fprintf(fp,"%lf	%d	%s	%lf	%s	%d	%s\n\n",lexemeCurrentNode.realVal,lineNo,token,valueIfReal,parentNodeSymbol,isLeafNode,nodeSymbol);
		}
		else if(PT->ti.type==STRING)
		{
			strcpy(lexemeCurrentNode.charVal,(PT->ti).val.charVal);
			fprintf(fp,"%s	%d	%s	%d	%s	%d	%s\n\n",lexemeCurrentNode.charVal,lineNo,token,valueIfNumber,parentNodeSymbol,isLeafNode,nodeSymbol);
		}
		
		//else
		//fprintf(fp,"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa  %s  bbbbbbbbbbbbbbbbbbbbbbbbb \n",token);
		
	
	}
	else
	{
		strcpy(lexemeCurrentNode.charVal,PT->ti.val.charVal);
		isLeafNode=0;
		strcpy(nodeSymbol,(PT->ti).name);
		fprintf(fp,"--- %d	%s	%d	%s	%d	%s\n\n",lineNo,token,valueIfNumber,parentNodeSymbol,isLeafNode,nodeSymbol);
	}
	
	
	int i;
	for(i=0;i<PT->number_child;i++)
	{
		printParseTree(PT->children[i],fp,nodeSymbol);
	}
	
}










Grammar createEmptyGrammar()
{
	Grammar g;
	g.numRules=-1; int i;
	for(i=0;i<NUM_RULES;i++)
	{
		g.rules[i] = NULL;
	}
	return g;
}

void createNewNode (Grammar *g, char c[])
{
	
	link temp = g->rules[g->numRules];
	if(temp == NULL)	
	{
		
		g->rules[g->numRules] = (link)malloc(sizeof(box));
		strcpy(g->rules[g->numRules]->symbol, c);
		g->rules[g->numRules]->next = NULL;				
	}
	
	else
	{
		while(temp->next!=NULL)
		{
			temp = temp->next;
		}	
			
		link temp2 = (link)malloc(sizeof(box));		
		strcpy(temp2->symbol, c);
		temp2->next=NULL;
	
		temp->next = temp2;
	}
	
	return;
}


Grammar readGrammarAndMakeHashTable(FILE *fp, hashTable2 h, int *numNonTerminals, int *numTerminals)
{
	/* This function populates the grammar in an array of linked lists and also populates a hashtable which maps the terminal or non-terminal to a unique index*/
	
	*numTerminals = 0;
	*numNonTerminals = 0;

	Grammar g = createEmptyGrammar();	//make numRules = -1 initially
	int ruleNumber = 0;
	int leftOrRight = 0;
	int currTerminalIdx = 0;
	int currNonTerminalIdx = 0;
	//int ruleEnded = 0;
	char leftSide[100];
	char rightSide[100];
	char s[100];
	
	LINK l;  //hashLink
	
	while(!feof(fp))
	{
		
		
		if(fscanf(fp,"%s",s)!=1){break;}  //printf("%s\n", s);
		if(s[1]==')' || s[2] == ')')   {/*printf("bye1\n");*/leftOrRight = 0; g.numRules++; continue;}
		
		if(leftOrRight == 0)
		{
			//hashing taken care of
			l = find2(h, s);
			if(l==NULL)  
			{
				l=insert2(h,s);
				l->index = currNonTerminalIdx;
				currNonTerminalIdx++;
			}
			
			l->leftSideOf[l->numLeftSideOf]=g.numRules;
			l->numLeftSideOf++;
			
			 
			
			strcpy(leftSide, s); 
			createNewNode(&g, leftSide);
			fscanf(fp,"%s",s);     //flushing out the '->' here only
			leftOrRight = 1;
			continue;
		}
		
		else if(leftOrRight == 1 && s[0]!='|')
		{
			
			l = find2(h, s);
			strcpy(rightSide, s);
			if(l==NULL)
			{
				l=insert2(h,s);
				if(s[0]=='<')
				{
					l->index = currNonTerminalIdx;
					currNonTerminalIdx++;
				}
				
				else
				{
					l->index = currTerminalIdx;
					currTerminalIdx++;
				}	
			}
			
			l->rightSideOf[l->numRightSideOf]=g.numRules;
			l->numRightSideOf++;
			//endHashing
			
			createNewNode(&g, rightSide);
			continue;
		}
		
		else //(s[0] == '|')
		{
			//printf("bye4\n");
			//ruleEnded = 1;
			g.numRules ++ ;
			createNewNode(&g, leftSide);	//filling in the left side again
			
			
			LINK l = find2(h, leftSide);		
			l->leftSideOf[l->numLeftSideOf] = g.numRules;
			l->numLeftSideOf++;				
		}
		
	}
	
	
	//add a special entry for "$"
	LINK x = insert2(h, "$");
	{
		x->index = currTerminalIdx;
		currTerminalIdx++;
	}
	
	*numTerminals = currTerminalIdx;
	*numNonTerminals = currNonTerminalIdx;
	
	g.numRules++; 
	return g;	
}



void printGrammar(Grammar g)
{
	printf("numrules %d\n", g.numRules);
	int i,j;
	for(i=0;i<=g.numRules;i++)
	{
		link temp = g.rules[i];
		for(j=0;temp!=NULL;j++)
		{	
			printf("%s ",temp->symbol);
			if(j==0) printf("-> ");
			temp=temp->next;
		}
		printf("\n");
	}
	return; 
}

set findFirstSet(link x, Grammar G, hashTable2 h)   //elements are non-unique. Doesn't matter!!
{
	int checkForEpsilon = 1;
	int i;
	LINK temp1 ;
	set s = createEmptySet();
	set temp = createEmptySet();
	
	//firstCheck if its already in the hashTable
	//if(!isEmpty(temp1->firstSet))
	//{
	//	return setUnion(s, temp1->firstSet);
	//}
	//endCheck
	
	//base case
	//if(x->symbol[0]!='<')  //its a terminal
	//{
	//	//printf("oye!!");
	//	s = insertSet(s, x->symbol);
	
	//	//if(strcmp(x->symbol, "EPSILON")!=0)	
	//	//checkForEpsilon = checkForEpsilon & 0;	
	//}
	////end base case
	link l = x;
	
	/*while(l!=NULL)
	{printf("%s--  ", l->symbol); l=l->next;}
	printf("\n");
	*/
		for(i=0;x!=NULL;i++)
		{
			if(x->symbol[0]!='<')
			{
				checkForEpsilon = 0;
				temp = createEmptySet();  temp = insertSet(temp, x->symbol);
				if(i==0) return temp;
				else 
				{
					s=setUnion(s, temp);	
					break;
				}
			}
			
			
			//printf("FIRST %s\n", x->symbol);
			//sleep(1);
			//printf("@@@@  %d\n", temp1->numLeftSideOf);
			
			else
			{
				temp1 = find2(h, x->symbol);
				for(i=0;i<temp1->numLeftSideOf;i++)      
				{
					
					/*printf("\n!!printing where it is\n");
				
					link l1 = G.rules[temp1->leftSideOf[i]];
					{
						while(l1!=NULL) {printf("%s  ", l1->symbol); l1=l1->next;}		
					}
					printf("\n");*/
				
					link l = G.rules[temp1->leftSideOf[i]];
					l=l->next;
				
					temp = findFirstSet(l, G, h);
					s = setUnion(s, temp);
				
								
				}
			
				
			}
			//printf("raj!!\n");		
			
			if(isMember(s, "EPSILON"))  
			{
				s = removeElement(s, "EPSILON");
				checkForEpsilon = checkForEpsilon & 1;
				//printf("yoda\n");
				//printSet(temp);
				x=x->next;
				//printf("bazingaa\n");
				destroySet(temp);
				//printf("shelodon!!\n");
			}
				
			else {checkForEpsilon = checkForEpsilon & 0; break;}
						
		}
		//printf("heloooo!!!!!!!!!\n");
		if(checkForEpsilon == 1) 
		{s = insertSet(s, "EPSILON");}
		//printf("heloowdwdoo\n");
	
	
	//addTo hashTable
	//temp1->firstSet = createEmptySet();
	//temp1->firstSet = setUnion(temp1->firstSet, s);
	//added to hashtable
	//printf("END\n");
	return s;
	
}


set findFollowSet(char nonTerminal[], Grammar G, hashTable2 h, set *stackStore)  //later assume that all first sets are already in the hashtable.
{
	if(isMember(*stackStore, nonTerminal)) return createEmptySet();
	
	*stackStore = insertSet(*stackStore, nonTerminal);
	
	LINK temp1 = find2(h, nonTerminal);		
	set s= createEmptySet();
	set temp = createEmptySet();
	char lhs[80] = "\0";
	int i;  int count =0;
	
	//special case
	if(temp1->numRightSideOf==0)     //its not on right side of any symbol
		return insertSet(s, "$");
	
	//printf("#### %d\n", temp1->numRightSideOf);
	for(i=0;i<temp1->numRightSideOf;i++)
	{
		if(i>0 && temp1->rightSideOf[i]==temp1->rightSideOf[i-1]) count++;
		else count = 0;
		 
		//printf("FOLLOW %s\n", nonTerminal);
		
		/*printf("printing where it is\n");
		
		link l1 = G.rules[temp1->rightSideOf[i]];
		{
			while(l1!=NULL) {printf("%s  ", l1->symbol); l1=l1->next;}		
		}
		printf("\n");*/
		
		link l = G.rules[temp1->rightSideOf[i]];
		
		strcpy(lhs, l->symbol);     //storing the lhs just in case the nonTerm appears on the rightmost end of the rule.

		//sleep(1);
		//finding nonTerminal in G
		l=l->next;
		while(l!=NULL)
		{
			if(strcmp(l->symbol, nonTerminal) == 0 && count==0)
			{/*printf("foundIT!!!!\n") ;*/ break;}
			
			else if(strcmp(l->symbol, nonTerminal) == 0 && count!=0) {count--; l=l->next;} 
			
			else
			l=l->next;
		}
		//found the nonTerminal in G
		
		if(l->next!=NULL)
		{
			temp = findFirstSet(l->next, G, h);
			
			if(isMember(temp, "EPSILON"))
			{
				temp = removeElement(temp, "EPSILON"); 
				temp = setUnion(temp, findFollowSet(lhs, G, h, stackStore));
			}
			
		}
		
		else	//its on the rightmost end of the rule
			temp = findFollowSet(lhs, G, h, stackStore);
		//printf("stuart\n");
		//printSet(s); printSet(temp);
		s = setUnion(s, temp); 	destroySet(temp);	
		//printf("penny\n");
	}
	//removeElement(*stackStore, nonTerminal);
	//printf("\nFollow of %s\n\n", nonTerminal);
	//printSet(s);
	return s;
}

set findFollowWrap(char nonTerminal[], Grammar g, hashTable2 h)
{
	set stackStore = createEmptySet();
	set ret = findFollowSet(nonTerminal, g, h, &stackStore);
	destroySet(stackStore);
	return ret;
}


void createParseTable(hashTable2 h, Grammar g, int table[100][100], int numTerminals, int numNonTerminals)
{
	LINK cur;
	int terminalIndex,nonTerminalIndex;
	char name[300];
	link grammar_link;
	int i,j;
	int k;
	int grammarIndex;
	set  firstSet,followSet;
	for( i=0;i<100;i++)
	{
		for(j=0;j<100;j++)
		table[i][j]=-1;
	}
	//memset(table,-1,sizeof(table));
	for(i=0;i<hashSize2;i++)
	{
		cur=h[i].head;
		while(cur!=NULL)
		{
			//printf("hi\n");
			if(!(cur->tOrNt))
			{
				strcpy(name, cur->name);
				nonTerminalIndex=cur->index;
				for(j=0;j<cur->numLeftSideOf;j++)
				{
					link temp=g.rules[cur->leftSideOf[j]];
					
					grammarIndex=cur->leftSideOf[j];
					grammar_link=g.rules[grammarIndex]->next;
					firstSet=findFirstSet(grammar_link,g,h);
					for(k=0;k<firstSet.size;k++)
					{
						//if(strcmp(firstSet.arr[k],find2(h,firstSet.arr[k])->name)!=0)
						//{
					//		printf("wrong!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
					//	}
						if(strcmp(firstSet.arr[k],"EPSILON")==0)  continue;
						
						terminalIndex=find2(h,firstSet.arr[k])->index;
						if((table[nonTerminalIndex][terminalIndex])!=-1)
						{
						printf("First Conflict in %d,\n",grammarIndex);
							//printSet(firstSet);
							
							
						}
						
						table[nonTerminalIndex][terminalIndex]=grammarIndex;
					}
					
					temp=g.rules[cur->leftSideOf[j]];
					if(isMember(firstSet, "EPSILON"))
					{
						//printf("the non terminal with epsilon is %s\n",name);
						followSet = findFollowWrap(name ,g, h);
						
						for(k=0;k<followSet.size;k++)
						{
							terminalIndex=find2(h,followSet.arr[k])->index;
							if((table[nonTerminalIndex][terminalIndex])!=-1)
							{
								printf("FollowConflict in %d\n",grammarIndex);
								
							}
							
							table[nonTerminalIndex][terminalIndex]=grammarIndex;
						}
						//destroySet(followSet);
					}
					//destroySet(firstSet);
				}
				
			}
			cur=cur->next;
		}
	}	
}

void printFollowSets(Grammar g, hashTable2 h)
{
	int i;
	set s;
	for(i=0;i<g.numRules;i++)
	{
		s = findFollowWrap(g.rules[i]->symbol, g, h);
		printf("FOLLOW OF %s\n", g.rules[i]->symbol);
		printSet(s);
		printf("\n\n");
	}
	return ;
}

