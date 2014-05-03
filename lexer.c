/*

Batch No- 30

2011A7PS108P - Rishab Kalra

2011A7PS087P- Ishan Bansal

*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include "hash.h"
#include "hashDef.h"
#include "lexerDef.h"




tokenInfo buffOverflowToken(int reloadPoint)  					//reloadPoint is the offset by which fp needs to backoff.
{
	tokenInfo ti;
	strcpy(ti.name, BUFFER_OVERFLOW);
	ti.val.integerVal = reloadPoint;
	return ti;
}

tokenInfo createToken(char name[], int lineNo,int size, int tokenNo, void *value, tokenType type)
{
	tokenInfo ti;
	strcpy(ti.name, name);
	ti.lineNo=lineNo;
	ti.size=size;
	ti.type = type;
	ti.tokenNo = tokenNo;
	
	switch(type)
	{
		case INTEGER:
		ti.val.integerVal = *((int *)value);
		break;
		
		case REAL:
		ti.val.realVal = *((double *)value);
		break;
		
		case STRING:
		strcpy(ti.val.charVal, (char *)value);
		break;
		
		default:   //type 4
		ti.val.DEFAULT_TOKEN_TYPE = (DEFAULT_TOKEN_CTYPE)DEFAULT_TOKEN_VALUE ;
	}
	return ti;
	
}

FILE *getStream(FILE *fp, buffer b[], int k)
{
	char c;
	int count=0;
	while((c=fgetc(fp))!=EOF)
	{
		b[count]=c; count++;
		if(count==k)
		break;
	}

	if(c==EOF)
	{
		b[count]=EOF;
		
	}
	return fp;	
}

void printToken(tokenInfo ti)
{
	printf("%d. ",ti.tokenNo);
	printf("TOKEN NAME: %s    ",ti.name);
	printf("LEXEME: ");
	switch(ti.type)
	{
		case INTEGER:
		printf("%d\n",ti.val.integerVal);
		break;
		
		case REAL:
		printf("%.2f\n",ti.val.realVal);
		break;
		
		case STRING:
		printf("%s\n",ti.val.charVal);
		break;
		
		default:   //type 4
		printf("default\n");
	}
	
}

tokenInfo lexerCompleteToken(int *tokenNo)
{
	tokenInfo ti;

	ti.tokenNo = *tokenNo; 
	strcpy(ti.name,LEXER_COMPLETE);
	return ti;
}

int inline isDigit(char c)
{
	if((int)c>=ASCII_0 && (int)c<=(ASCII_0+9)) return 1;
	else return 0;
}

int inline isAnyLetter(char c)
{
	if((c>=ASCII_a && c<=(ASCII_a+25)) || (c>=ASCII_A && c<=(ASCII_A+25))) return 1;
	else return 0;
}


int inline isSmallLetter(char c)
{
	if((c>=ASCII_a && c<=(ASCII_a+25)))
	return 1;
	
	return 0;
}

tokenInfo  getNextTokenHELPER (FILE *source, buffer b[],int *beginLexeme,int *lineNo, int *tokenNo,hashTable h, errorArray *e);
FILE *handleBufferOverflow(FILE *source, buffer b[], int reloadPoint, int boolean);


tokenInfo getNextToken(FILE *source, buffer b[], int *beginLexeme, int *lineNo, int *tokenNo, errorArray *e)
{
	hashTable h;
	initializeHashTable(h);
	
		if(*beginLexeme >= BUFF_SIZE)
				source = handleBufferOverflow(source, b, 0, 0);
				
		while(*beginLexeme<BUFF_SIZE)
		{
						
		
			
			tokenInfo tok=getNextTokenHELPER(source, b, beginLexeme, lineNo, tokenNo,h, e);
					
			if(strcmp(tok.name,BUFFER_OVERFLOW) == 0)
			{	
				source = handleBufferOverflow(source, b, tok.val.integerVal, 0);
				*beginLexeme = 0;
				(*tokenNo)--;				    //tokenNo has to be reset!!
			}	
			
			//handling bufferoverflow in case of a comment
			//insert a '#' at the beginning of the buffer so that the lexer still sees the following text as a comment.
			else if(strcmp(tok.name,"COMMENT_OVERFLOW")==0)
			{
				b[0] = '#'; 
				source = handleBufferOverflow(source, b+1, 0, 1);
				*beginLexeme = 0;
				(*tokenNo)--;
			}
			
			else if(strcmp(tok.name,"COMMENT_COMPLETED")==0)
			{
				(*tokenNo)--;
			}
			
			else	{return tok; } 
		}	
	
}



tokenInfo  getNextTokenHELPER (FILE *source, buffer b[],int *beginLexeme,int *lineNo, int *tokenNo,hashTable h, errorArray *e)
{
	int intAccumulator = 0;
	double realAccumulator = 0.0;
	char stringAccumulator[500];
	int patternIdx = 0;
	char patternAccumulator[300];
	int currPosition = 0;       //its the position where the accumulator is at.
	
	DEFAULT_TOKEN_CTYPE def = DEFAULT_TOKEN_VALUE; 
	(*tokenNo)++;
	int lookahead = *beginLexeme;
	char tokenName[300];
	int state = -1;
	int len=0;
	tokenInfo ti;
	int errorType = 0;
	//int isRetract=0;	
	while(lookahead<BUFF_SIZE)
	{
		if(b[lookahead]!='\n'&&b[lookahead]!='\t'&&b[lookahead]!=' ')
		patternAccumulator[patternIdx++] = b[lookahead];
		
		switch(state)
		{
			
			case -2:
				patternAccumulator[patternIdx-1] = '\0'; 
				patternIdx = 0;
				currPosition=0;
				strcpy(ti.name,LEX_ERROR);
				ti.tokenNo=errorType;
				strcpy(ti.val.charVal,patternAccumulator);
				ti.lineNo = *lineNo;
 				ti.type = STRING;
	
				e->ti[e->numErrors]=ti;
				e->numErrors++;
	
				
				if(lookahead>=BUFF_SIZE) 
				{
					source = handleBufferOverflow(source, b, 0, 0); 
					*beginLexeme = 0;
					lookahead = *beginLexeme;
				}
				
				
				while(b[lookahead]!=';')
				{
					
					
					if(b[lookahead] == '\n')
					(*lineNo)++;
		
					(lookahead)++;
					
					if((lookahead)>=BUFF_SIZE) 
					{
						source = handleBufferOverflow(source, b, 0, 0); 
						*beginLexeme = 0;
						lookahead = *beginLexeme;
					}
				}
				errorType  = 0;
						
				*beginLexeme = (lookahead)+1;
				lookahead++;
				state = -1;
				break;
			
			
			case -1:
			while(b[lookahead]==' '||b[lookahead]=='\t'||b[lookahead]=='\n')	//skip white spaces
			{
				if(b[lookahead] == '\n')         (*lineNo)++;	
			
				(*beginLexeme)++;  lookahead++;
		
				if(*beginLexeme >= BUFF_SIZE) 	return buffOverflowToken(0);   //the buffer hasn't actually overflown, its 												only exhausted. Thats why the argument is zero.
			}
			if(b[lookahead]==EOF) 	return lexerCompleteToken(tokenNo);
			state=0; break;
			
			
			case 0:
			len++;
			if(b[lookahead] == '#')						       //skip comments
			{	
				//printf("Reading a comment!\n");
				while(1)
				{
					if(lookahead >= BUFF_SIZE)
						//comment buffer has overflown. Needs to be handeled differently.
						return createToken("COMMENT_OVERFLOW", *lineNo, -999, *tokenNo, &def, OTHER);
					
					
					
					else if(b[lookahead] == '\n')		//comment ended
					{
						//printf("comment ended\n");
						(*beginLexeme) = lookahead + 1;
						//lookahead = *beginLexeme;
						(*lineNo)++ ;
						return createToken("COMMENT_COMPLETED", *lineNo, -999, *tokenNo, &def, OTHER);
					}
					//printf("char in comment %c\n", b[lookahead]);
					lookahead++;
					
				}
				
				
			}
			
			//printf("im here\n"); pause();
			else if(b[lookahead] == '<') state = 1;
			else if(b[lookahead] == '>') state = 2;
			else if(isDigit(b[lookahead])) 
			{
				intAccumulator = intAccumulator*10 + (b[lookahead] - ASCII_0);	
				state = 3;
			}
			
			else if(isAnyLetter(b[lookahead])) 
			{
				stringAccumulator[currPosition] = b[lookahead]; 
				currPosition++ ;
				state = 4;
			}
			
			else if(b[lookahead]=='=') state=6;
			
			else if(b[lookahead]=='_') 
			{
			stringAccumulator[currPosition] = b[lookahead]; 
			currPosition++ ;
			state=7;
			}
			
			else if(b[lookahead]=='"') state=8;
			
			else if(b[lookahead]=='[') state=9;
			
			else if(b[lookahead]==']') state=10;
			
			else if(b[lookahead]=='(') state=11;
			
			else if(b[lookahead]==')') state=12;
			
			else if(b[lookahead]==';') state=13;
			
			else if(b[lookahead]==',') state=14;
			
			else if(b[lookahead]=='+') state=15;
			
			else if(b[lookahead]=='-') state=16;
			
			else if(b[lookahead]=='*') state=17;
			
			else if(b[lookahead]=='/') state=18;
			
			else if(b[lookahead]=='@') state=20;
			
			else if(b[lookahead]=='.') state=21;
			
			else  
			{
				state = -2;
				break;
			}  //lexical error
			
			lookahead ++;
			break;
			
		
			case 1:
			 			 // EQ and ASSIGNOP
			if(b[lookahead]=='=')
			{
				*beginLexeme = lookahead+1;
				return createToken("LE", *lineNo, -999, *tokenNo, "<=", STRING);
			}
			

			else
			{
				*beginLexeme = lookahead;
				return createToken("LT", *lineNo, -999, *tokenNo, "<", STRING);
			} 	
			break;
			
			
			case 2:
			
			if(b[lookahead]=='=')
			{
				*beginLexeme = lookahead+1;
				return createToken("GE", *lineNo, -999, *tokenNo, ">=", STRING);
			}
			

			else
			{
				*beginLexeme = lookahead;
				//printf("i!!!m here\n"); pause();
				return createToken("GT", *lineNo, -999, *tokenNo, ">", STRING);
				//printToken(t); pause();
			} 	
			break;
			
			
			case 3:
			if(isDigit(b[lookahead])) 
			{
				intAccumulator = intAccumulator*10 + (b[lookahead] - ASCII_0);	
				state = 3;
			} 
			
			else if(b[lookahead] == '.') 	state = 5;
			
			else
			{
				*beginLexeme = lookahead;
				return createToken("NUM", *lineNo, -999, *tokenNo, &intAccumulator, INTEGER);
			} 
			lookahead++;
			break;
			
			
			case 4:
			len++;
			if(isAnyLetter(b[lookahead])) {stringAccumulator[currPosition] = b[lookahead]; currPosition++; state = 4;}
			else if(isDigit(b[lookahead])) 
			{
				*beginLexeme = lookahead+1;
				stringAccumulator[currPosition] = b[lookahead];
				stringAccumulator[currPosition+1] = '\0';
				
				if((lookahead+1)>= BUFF_SIZE)          
				{
					return buffOverflowToken(BUFF_SIZE - (*beginLexeme));
				} 
				
				if(isDigit(b[lookahead+1]) || isAnyLetter(b[lookahead+1]))
				{
					patternAccumulator[patternIdx++] = b[lookahead+1];
					state = -2;
					break;
				}
				
				if(currPosition>=20)
				{
					errorType = 1;
					state = -2;
					break;
				}
				return createToken("ID", *lineNo, len, *tokenNo, stringAccumulator, STRING);
			}
				
			else 
			{
				len--;
				*beginLexeme = lookahead;
				stringAccumulator[currPosition] = '\0';
				if(find(h,stringAccumulator,tokenName))
				{
					return createToken(tokenName, *lineNo, -999, *tokenNo, stringAccumulator, STRING);	
				}
				
				if(currPosition>20)
				{
					errorType = 1;
					state = -2;
					break;
				}
									
				return createToken("ID", *lineNo, len, *tokenNo, stringAccumulator, STRING);
			}
			
			lookahead++;
			break;
			
			
			
			case 5:				      //make a separate state for    .and.	.or. 	.not.
						
			if(isDigit(b[lookahead]))
			{
				*beginLexeme = lookahead+1;
				realAccumulator = (double)intAccumulator + 0.10*(double)(b[lookahead]-ASCII_0);
				state = 22;
			}
			
			else 
			{
				*beginLexeme = lookahead-1;
				return createToken("NUM", *lineNo, -999, *tokenNo, &intAccumulator, INTEGER);
			}
			lookahead++;
			
			break;
			
			
			case 6:
			
			if(b[lookahead]=='=')
			{
				*beginLexeme = lookahead+1;
				return createToken("EQ", *lineNo, -999, *tokenNo, "==", STRING);
			}
			

			else if(b[lookahead]=='/')
			{
				state=19;
				lookahead++;
			}
			else
			{
				*beginLexeme=lookahead;
				return createToken("ASSIGNOP",*lineNo,-999,*tokenNo, "=", STRING);
			} 	
			break;
			
			case 7:
			len++;
			if(isDigit(b[lookahead]) || isAnyLetter(b[lookahead]))
			{
				stringAccumulator[currPosition] = b[lookahead]; 
				currPosition++; 
				state = 7;
				lookahead++;
			}
			else
			{
				len--;
				stringAccumulator[currPosition]='\0';
				*beginLexeme=lookahead;
				if(find(h,stringAccumulator,tokenName))
				{
					return createToken(tokenName, *lineNo,-999, *tokenNo, stringAccumulator, STRING);	
				}
				
				return createToken("FUNID",*lineNo,len,*tokenNo,stringAccumulator,STRING);
			}
			break;
			
			case 8:
			len++;
			if(isSmallLetter(b[lookahead]) || b[lookahead]==' ')
			{
				if(b[lookahead]==' ')
				patternAccumulator[patternIdx++]=' ';
				stringAccumulator[currPosition] = b[lookahead]; 
				currPosition++; 
				state = 8;
				lookahead++;
			}
			
			else if(b[lookahead]=='"')
			{
				len--;
				len--;
				stringAccumulator[currPosition]='\0';			
				*beginLexeme=lookahead+1;
				if(currPosition>20)
				{
					errorType = 1;
					state = -2;
					break;
				}
				return createToken("STR",*lineNo,len,*tokenNo,stringAccumulator,STRING);
			}
			
			else  
			{
				state = -2;
				break;
			}  //lexical error

			break;
			
			
			
			case 9:
			
			*beginLexeme=lookahead;
			return createToken("SQO",*lineNo,-999,*tokenNo,"[",STRING);
			break;
			
			case 10:
			
			*beginLexeme=lookahead;
			return createToken("SQC",*lineNo,-999,*tokenNo,"]",STRING);
			break;
			
			case 11:
			
			*beginLexeme=lookahead;
			return createToken("OP",*lineNo,-999,*tokenNo, "(",STRING);
			break;
			
			case 12:
			
			*beginLexeme=lookahead;
			return createToken("CL",*lineNo,-999,*tokenNo,")",STRING);
			break;
			
			case 13:
			
			*beginLexeme=lookahead;
			return createToken("SEMICOLON",*lineNo,-999,*tokenNo,";",STRING);
			break;
			
			case 14:
			
			*beginLexeme=lookahead;
			return createToken("COMMA",*lineNo,-999,*tokenNo,",",STRING);
			break;
			
			case 15:
			
			*beginLexeme=lookahead;
			return createToken("PLUS",*lineNo,-999,*tokenNo,"+",STRING);
			break;
			
			case 16:
			
			*beginLexeme=lookahead;
			return createToken("MINUS",*lineNo,-999,*tokenNo,"-",STRING);
			break;
			
			case 17:
			
			*beginLexeme=lookahead;
			return createToken("MUL",*lineNo,-999,*tokenNo,"*",STRING);
			break;
			
			case 18:
			
			*beginLexeme=lookahead;
			return createToken("DIV",*lineNo,-999,*tokenNo,"/",STRING);
			break;
			
			case 19:
			
			if(b[lookahead]=='=')
			{
				*beginLexeme=lookahead+1;
				return createToken("NE",*lineNo,-999,*tokenNo,"=/=",STRING);
			}
			else  
			{
				state = -2;
				break;
			} //lexical error

			break;
			
			case 20:
			
			*beginLexeme=lookahead;
			return createToken("SIZE",*lineNo,-999,*tokenNo,"@",STRING);
			break;
			
			case 21:
			
				switch(b[lookahead])
				{
					case 'a':	{lookahead++; state =23;	break;}
					case 'o':	{lookahead++; state= 24;	break;}
					case 'n':	{lookahead++; state = 25;	break;}
					default:	
					    state = -2;
					    break;
					  //lexical error

				}
				
				break;
			
			case 22:
			if(isDigit(b[lookahead]))
			{
				*beginLexeme = lookahead+1;
				realAccumulator = realAccumulator + 0.01*(double)(b[lookahead]-ASCII_0);
				return createToken("RNUM",*lineNo,-999,*tokenNo, &realAccumulator, REAL);
			}
		
			else  
			{
				state = -2;
				break;
			} 
			lookahead++;
			break;
			
			case 23:
			if(b[lookahead]=='n') state = 26;
			else  
			{
				state = -2;
				break;
			} 
			lookahead++;
			break;
			
			case 24:
			if(b[lookahead]=='r')
			state = 27;
			else  
			{
				state = -2;
				break;
			}
			lookahead++;
			break;
			
			case 25:
			if(b[lookahead]=='o')
			state =28;
			else  
			{
				state = -2;
				break;
			}
			lookahead++;
			break;
			
			case 26:
			if(b[lookahead]=='d')
			state = 29;
			else  
			{
				state = -2;
				break;
			}
			lookahead++;
			break;
			
			case 27:
			if(b[lookahead]=='.')
			{
				*beginLexeme=lookahead+1;	
				return createToken("OR",*lineNo,-999,*tokenNo, ".or.", STRING);
			}
			else  
			{
				state = -2;
				break;
			}
			break;
	
			case 28:
			if(b[lookahead]=='t')
			state = 30;
			else  
			{
				state = -2;
				break;
			}
			lookahead++;
			break;
			
			case 29:
			if(b[lookahead]=='.')
			{
				*beginLexeme=lookahead+1;	
				return createToken("AND",*lineNo,-999,*tokenNo, ".and.", STRING);
			}
			else  
			{
				state = -2;
				break;
			}
			break;
			
			case 30:
			if(b[lookahead]=='.')
			{
				*beginLexeme=lookahead+1;	
				return createToken("NOT",*lineNo,-999,*tokenNo, ".not.", STRING);
			}
			else  
			{
				state = -2;
				break;
			}
			break;



			
		}
		
	}	
	
	if(lookahead>=BUFF_SIZE)
		return buffOverflowToken(lookahead - (*beginLexeme));  //the amount by which fp needs to move back.
									
}



void printBuffer (char b[])
{
	int i=0;
	while(b[i]!=EOF && i<BUFF_SIZE)
	{
		printf("%c",b[i]);
		i++;
	}
	printf("\n");
}

FILE *handleBufferOverflow(FILE *source, buffer b[], int reloadPoint, int boolean)   /*if boolean = 0 ==> normal overflow
										       else commentOverflow	*/
{
	fseek(source, -reloadPoint, SEEK_CUR);
	source=(boolean==1) ? getStream(source,b,BUFF_SIZE-1) : getStream(source,b,BUFF_SIZE);  //BUFF_SIZE-1 because it already has a '#'
	//printf("amount to backoff: %d\n",reloadPoint);
	
	//printf("PRINTING THE BUFFER\n");
	//printBuffer(b);
	//printf("\n");
	
	return source;
}


