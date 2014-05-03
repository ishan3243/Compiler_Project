/*

Batch No- 30

2011A7PS108P - Rishab Kalra

2011A7PS087P- Ishan Bansal

*/

#ifndef LEXERDEF_H
#define LEXERDEF_H


#define BUFF_SIZE 1000
//Buffer Policy: If the buffer gets exhausted and we aren't in the final state then the buffer is refilled from 'beginLexeme'
#define buffer char

#define DEFAULT_TOKEN_TYPE realVal
#define DEFAULT_TOKEN_CTYPE double

#define DEFAULT_TOKEN_VALUE 0.9999   	   					//when the token is not real, int or string. 
#define LEX_ERROR "error"
#define BUFFER_OVERFLOW "buffOverflow"
#define LEXER_COMPLETE "$"	   					//returned when lexer completes without errors.
#define ASCII_0 48
#define ASCII_a 97
#define ASCII_A 65
#define MAX_ERRORS 1000

typedef enum  {INTEGER, REAL, STRING, OTHER} tokenType;   			//STRING includes actual strings, funIds and varIds.

//typedef enum  {STATE, TOKEN}returnType;	
					
typedef union {int integerVal; double realVal; char charVal[300];} value; 	//when its an integer or real or string.

typedef struct   
{
	char name[300];								//in case its an identifier or function name.
	value val;
	tokenType type;      							//type 1 for integer, 2 for real, 3 for string, funId or 											varId, type 4 for others.
	int lineNo;
	int size;      	     
	int tokenNo;
}tokenInfo;

typedef struct
{
	tokenInfo ti[MAX_ERRORS];
	int numErrors;
}errorArray;

//typedef union {int state; tokenInfo ti;} stateOrToken;			

//typedef struct
//{
//	returnType rt;
//	stateOrToken rv;
//}returnValue;			// a state can either return a token or return a new state
#endif
