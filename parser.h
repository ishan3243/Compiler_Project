/*

Batch No- 30

2011A7PS108P - Rishab Kalra

2011A7PS087P- Ishan Bansal

*/


#include "parserDef.h"
extern Grammar createEmptyGrammar();
extern void createNewNode (Grammar g, char c[]);
extern Grammar readGrammarAndMakeHashTable(FILE *fp, hashTable2 h, int *numTerminals, int *numNonTerminals);
extern void printGrammar(Grammar g);
extern set findFollowWrap(char nonTerminal[], Grammar G, hashTable2 h);
extern set findFirstSet(link x, Grammar G, hashTable2 h);
extern void createParseTable(hashTable2 h, Grammar g, int table[100][100], int numTerminals, int numNonTerminals);
extern void printFollowSets(Grammar g, hashTable2 h);
extern tree_LINK parseInputSourceCode(char *testcaseFile,int T[100][100],hashTable2 h,Grammar g,int caseType);
extern tree_LINK createTreeNode(tokenInfo ti,int isTerminal);
extern void initializeTree();
extern void printParseTree(tree_LINK PT,FILE* fp,char parentName[]);
