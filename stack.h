/* -- IFJ project 2013 ------------------------------------------------------
**
**	Interpreter of IFJ2013 language
**	4.11.2013 - 15.12.2013
**
**	Team 13 (b/3/I):
**
**	Bank Tomáš			<xbankt00@stud.fit.vutbr.cz>
**	Birger Mark			<xbirge00@stud.fit.vutbr.cz>
**	Botka Roland		<xbotka00@stud.fit.vutbr.cz>
**	Brandejs Zdenko		<xbrand06@stud.fit.vutbr.cz>
**	Khudiakov Daniil	<xkhudi00@stud.fit.vutbr.cz>
**
**	Header for helper stacks for precedence analysis.
**
** -------------------------------------------------------------------------*/


#ifndef __STACK_H__
#define __STACK_H__


#include "common.h"
#include "ial.h"
#include "instructions.h"

#define MAX_STACK_EXPR 1000	//max count of operations in expression
#define MAX_STACK_FUNC 1000	//max count of operations in expression

typedef struct {			//datatype for stack of integer
	int arr[MAX_STACK_EXPR];		//array of integers
	int top;				//counter for top element
} tStackTerm;				//type declaration (without struct)

void stackTermInit ( tStackTerm* s );			//init the stack
int stackTermEmpty ( tStackTerm* s );			//helper (is stack empty?)
int stackTermFull ( tStackTerm* s );			//helper (is stack full?)
int stackTermTop ( tStackTerm* s);				//return top element
void stackTermPop ( tStackTerm* s );			//pop top element
void stackTermPush ( tStackTerm* s, int c );	//push element to the top

typedef struct {					//datatype for stack of type data pointers
	typeData * arr[MAX_STACK_EXPR];		//array of type data pointers
	int top;						//counter for top element
} tStackNoterm;						//type declaration (without struct)

void stackNotermInit ( tStackNoterm* s );				//init the stack
int stackNotermEmpty ( tStackNoterm* s );				//helper (is stack empty?)
int stackNotermFull ( tStackNoterm* s );				//helper (is stack full?)
typeData * stackNotermTop ( tStackNoterm* s);			//return top element
void stackNotermPop ( tStackNoterm* s );				//pop top element
void stackNotermPush ( tStackNoterm* s, typeData * c );	//push element to the top

void printTermStack ( tStackTerm* s ) ;	//debug stack print function

typedef struct {						//datatype for stack of type data pointers
	// typeNodePtr * arrNode[MAX_STACK_FUNC];
	typeListItem* arrList[MAX_STACK_FUNC];
	typeData* arrData[MAX_STACK_FUNC];
	int top;							//counter for top element
} tStackTable;							//type declaration (without struct)

void stackTableInit ( tStackTable* s );				//init the stack
int stackTableEmpty ( tStackTable* s );				//helper (is stack empty?)
int stackTableFull ( tStackTable* s );				//helper (is stack full?)
int stackTableTop ( tStackTable* s, /*typeNodePtr* Node,*/ typeListItem* List, typeData* Data);	//return top element
void stackTablePop ( tStackTable* s );				//pop top element
void stackTablePush ( tStackTable* s, /*typeNodePtr* Node,*/ typeListItem* List, typeData* Data );	//push element to the top


#endif
