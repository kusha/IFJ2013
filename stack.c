#include <stdio.h>
#include "stack.h"

// Terminals Section

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
	return s->top == MAX_STACK-1;
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

// Noterminals section

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
	return s->top == MAX_STACK-1;
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

void printTermStack ( tStackTerm* s ) {
	int i;
	for (i = s->top; i >= 0; i--) {
		printf("%i ", s->arr[i]);
	}
	printf("\n");
}

 // int main () {
 // 	tStackTerm termStack; 			//stack of terms
	// stackTermInit (&termStack);
	// stackTermPush ( &termStack, 10);
	// printf("%i\n", stackTermTop (&termStack));
 // }
