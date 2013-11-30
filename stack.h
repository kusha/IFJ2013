#ifndef __STACK_H__
#define __STACK_H__

#include "common.h"
#include "ial.h"

#define MAX_STACK 200

typedef struct {
	int arr[MAX_STACK];
	int top;
} tStackTerm;

void stackTermInit ( tStackTerm* s );
int stackTermEmpty ( tStackTerm* s );
int stackTermFull ( tStackTerm* s );
int stackTermTop ( tStackTerm* s);
void stackTermPop ( tStackTerm* s );
void stackTermPush ( tStackTerm* s, int c );

typedef struct {
	typeData * arr[MAX_STACK];
	int top;
} tStackNoterm;

void stackNotermInit ( tStackNoterm* s );
int stackNotermEmpty ( tStackNoterm* s );
int stackNotermFull ( tStackNoterm* s );
typeData * stackNotermTop ( tStackNoterm* s);
void stackNotermPop ( tStackNoterm* s );
void stackNotermPush ( tStackNoterm* s, typeData * c );

void printTermStack ( tStackTerm* s ) ;

#endif
