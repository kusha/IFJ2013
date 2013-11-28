#ifndef __STACK_H__
#define __STACK_H__

#include "common.h"
#include "ial.h"

#define MAX_STACK 200

typedef struct {
	int arr[MAX_STACK];
	int top;
} tStackTerm;

int stackTermEmpty ( const tStackTerm* s );
int stackTermFull ( const tStackTerm* s );
int stackTermTop ( const tStackTerm* s);
void stackTermPop ( tStackTerm* s );
void stackTermPush ( tStackTerm* s, int c );

typedef struct {
	typeData * arr[MAX_STACK];
	int top;
} tStackNoterm;

void stackNotermInit ( tStackNoterm* s );
int stackNotermEmpty ( const tStackNoterm* s );
int stackNotermFull ( const tStackNoterm* s );
typeData * stackNotermTop ( const tStackNoterm* s);
void stackNotermPush ( tStackNoterm* s, typeData * c );


#endif
