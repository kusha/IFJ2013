/* -- IFJ project 2013 -------------------------------------------------------
**
**	Interpreter of IFJ2013 language
**	4.11.2013 - 15.12.2013
**
**	Team 13 (b/3/I):
**
**	Bank Tomáš			<xbankt00@stud.fit.vutbr.cz>
** +Birger Mark			<xbirge00@stud.fit.vutbr.cz>
**	Botka Roland		<xbotka00@stud.fit.vutbr.cz>
**	Brandejs Zdenko		<xbrand06@stud.fit.vutbr.cz>
** +Khudiakov Daniil	<xkhudi00@stud.fit.vutbr.cz>
**
**	Header for helper stacks for precedence analysis.
**
** -------------------------------------------------------------------------*/


#ifndef __STACK_H__
#define __STACK_H__


/* -- Includes part --------------------------------------------------------*/
#include "common.h"
#include "ial.h"
#include "instructions.h"

/* -- Macro definitions ----------------------------------------------------*/
#define MAX_STACK_EXPR 1000	
#define MAX_STACK_FUNC 1000	
#define STACK_EMPTY -1

/* -------------------------------------------------------------------------*/
typedef struct {			
	int arr[MAX_STACK_EXPR];	
	int top;				
} tStackTerm;					

typedef struct {					
	typeData * arr[MAX_STACK_EXPR];		
	int top;						
} tStackNoterm;

typedef struct {						
	typeData * arrList[MAX_STACK_FUNC];
	typeData * arrData[MAX_STACK_FUNC];
	int top;							
} tStackTable;						
						
void stackTermInit ( tStackTerm* s );			
int stackTermEmpty ( tStackTerm* s );			
int stackTermFull ( tStackTerm* s );			
int stackTermTop ( tStackTerm* s);				
void stackTermPop ( tStackTerm* s );			
void stackTermPush ( tStackTerm* s, int c );

void stackNotermInit ( tStackNoterm* s );				
int stackNotermEmpty ( tStackNoterm* s );				
int stackNotermFull ( tStackNoterm* s );				
typeData * stackNotermTop ( tStackNoterm* s);			
void stackNotermPop ( tStackNoterm* s );				
void stackNotermPush ( tStackNoterm* s, typeData * c );	

void printTermStack ( tStackTerm* s );		

void stackTableInit ( tStackTable* s );				
int stackTableEmpty ( tStackTable* s );				
int stackTableFull ( tStackTable* s );			
int stackTableTop ( tStackTable* s, typeData** List, typeData** Data);
void stackTablePop ( tStackTable* s );	
void stackTablePush ( tStackTable* s, typeData* List, typeData* Data );
int stackTableCount ( tStackTable* s); 

#endif
