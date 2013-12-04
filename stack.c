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
**	Helper stack for precedence analysis.
**
** -------------------------------------------------------------------------*/


/* -- Includes part --------------------------------------------------------*/

#include <stdio.h>
#include "stack.h"


/* -- Functionы for stack of integer ---------------------------------------*/

void stackTermInit ( tStackTerm* s ) {
	if (s == NULL) {
		if (DEBUG_FLAG) printf("NULL pointer isn't a stack!\n");
	} else {
		s->top=-1;
	}
}

int stackTermEmpty ( tStackTerm* s ) {
	return s->top == -1;
}

int stackTermFull ( tStackTerm* s ) {
	return s->top == MAX_STACK_EXPR-1;
}

int stackTermTop ( tStackTerm* s) {
	if (stackTermEmpty(s)) {
		if (DEBUG_FLAG) printf("No elements is stack TERMINALS!\n");
		return -1;
	} else {
		return s->arr[s->top];
	}
}

void stackTermPop ( tStackTerm* s ) {
	if (!stackTermEmpty(s)) {
		s->top--;
	}
}

void stackTermPush ( tStackTerm* s, int c ) {
	if (stackTermFull(s)) {
		if (DEBUG_FLAG) printf("Stack is full!\n");
	} else {
		s->top++;
		s->arr[s->top] = c;
	}
}


/* -- Functionы for stack of type Data pointers ----------------------------*/

void stackNotermInit ( tStackNoterm* s ) {
	if (s == NULL) {
		if (DEBUG_FLAG) printf("NULL pointer isn't a stack!\n");
	} else {
		s->top=-1;
	}
}

int stackNotermEmpty ( tStackNoterm* s ) {
	return s->top == -1;
}

int stackNotermFull ( tStackNoterm* s ) {
	return s->top == MAX_STACK_EXPR-1;
}

typeData * stackNotermTop ( tStackNoterm* s) {
	if (stackNotermEmpty(s)) {
		if (DEBUG_FLAG) printf("No elements is stack NOTERMINALS!\n");
		return NULL;
	} else {
		return s->arr[s->top];
	}
}

void stackNotermPop ( tStackNoterm* s ) {
	if (!stackNotermEmpty(s)) {
		s->top--;
	}
}

void stackNotermPush ( tStackNoterm* s, typeData * c ) {
	if (stackNotermFull(s)) {
		if (DEBUG_FLAG) printf("Stack is full!\n");
	} else {
		s->top++;
		s->arr[s->top] = c;
	}
}

/* -- Functionы for stack of Nodes ---------------------------------------*/

void stackTableInit ( tStackTable* s ) {
	if (s == NULL) {
		if (DEBUG_FLAG) printf("NULL pointer isn't a stack!\n");
	} else {
		s->top=-1;
	}
}

int stackTableEmpty ( tStackTable* s ) {
	return s->top == -1;
}

int stackTableFull ( tStackTable* s ) {
	return s->top == MAX_STACK_FUNC-1;
}

int stackTableTop ( tStackTable* s, /*typeNodePtr* Node,*/ typeListItem* List, typeData* Data) {
	if (stackTableEmpty(s)) {
		if (DEBUG_FLAG) printf("No elements is stack TERMINALS!\n");
		return -1;
	} else {
		/*Node=s->arrNode[s->top];*/
		List=s->arrList[s->top];
		Data=s->arrData[s->top];
		return SUCCESS;
	}
}

void stackTablePop ( tStackTable* s ) {
	if (!stackTableEmpty(s)) {
		s->top--;
	}
}

void stackTablePush ( tStackTable* s, /*typeNodePtr* Node,*/ typeListItem* List, typeData* Data ) {
	if (stackTableFull(s)) {
		if (DEBUG_FLAG) printf("Stack is full!\n");
	} else {
		s->top++;
		/*s->arrNode[s->top]=Node;*/
		s->arrList[s->top]=List;
		s->arrData[s->top]=Data;
	}
}

/* -- Debug only function for stack printing -------------------------------*/

void printTermStack ( tStackTerm* s ) {
	int i;
	for (i = s->top; i >= 0; i--) {
		printf("%i ", s->arr[i]);
	}
	printf("\n");
}
