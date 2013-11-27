/*
	IFJ project 2013
	Interpreter of IFJ2013 language
	4.11.2013 - 15.12.2013
	
	Team 13 (b/3/I):

	Bank Tomáš			<xbankt00@stud.fit.vutbr.cz>
	Birger Mark			<xbirge00@stud.fit.vutbr.cz>
	Botka Roland		<xbotka00@stud.fit.vutbr.cz>
	Brandejs Zdenko		<xbrand06@stud.fit.vutbr.cz>
	Khudiakov Daniil	<xkhudi00@stud.fit.vutbr.cz>
	
	String functions file
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "str.h"

#define DEBUG_FLAG 0

int strInit(string *s) {
	if ((s->str = (char*) malloc(STR_ALLOC_DELTA)) == NULL) {
		if (DEBUG_FLAG) printf("string allocation error\n");
		return STR_ERROR;
	}
	s->str[0] = '\0';
	s->length = 0;
	s->allocated = STR_ALLOC_DELTA;
	return STR_SUCCESS;
}

void strFree(string *s) {	//is OK add after free?
	free(s->str);
}

void strClear(string *s) {
	s->str[0] = '\0';
	s->length = 0;
}

int strAddChar(string *s, char c) {
	if (s->length + 1 >= s->allocated) {
		if (DEBUG_FLAG) printf("string reallocation needed\n");
		if ((s->str = (char*) realloc(s->str, s->length + STR_ALLOC_DELTA)) == NULL) {
			if (DEBUG_FLAG) printf("string allocation error\n");
			return STR_ERROR;
		}
		s->allocated = s->length + STR_ALLOC_DELTA;
	}
	s->str[s->length] = c;
	s->length++;
	s->str[s->length] = '\0';
	return STR_SUCCESS;
}

int strCompareConst(string *s1, char* s2) {
	return strcmp(s1->str, s2)?0:1;
}

int strCompare(string *s1, string *s2) {
	return strcmp(s1->str, s2->str);
}

int strCopy(string *s1, string *s2) {
	int newLength = s2->length;
	if (newLength >= s1->allocated)
	{
		if ((s1->str = (char*) realloc(s1->str, newLength + 1)) == NULL)
			return STR_ERROR;
		s1->allocated = newLength + 1;
	}
	strcpy(s1->str, s2->str);
	s1->length = newLength;
	return STR_SUCCESS;
}

char * strGetContent (string *s) {
	return s->str;
}

// void debugStrPrint(string *s) {
// 	if (DEBUG_FLAG) printf("%i\t%s\n", s->length, s->str);
// }

// int main() {
// 	string myString;
// 	strInit(&myString);
// 	debugStrPrint(&myString);
// 	strAddChar(&myString, 'a');
// 	debugStrPrint(&myString);
// 	strAddChar(&myString, 'a');
// 	debugStrPrint(&myString);
// 	strAddChar(&myString, 'a');
// 	debugStrPrint(&myString);
// 	strAddChar(&myString, 'a');
// 	debugStrPrint(&myString);
// 	strAddChar(&myString, 'a');
// 	debugStrPrint(&myString);
// 	strAddChar(&myString, 'a');
// 	debugStrPrint(&myString);
// 	strAddChar(&myString, 'a');
// 	debugStrPrint(&myString);
// 	strAddChar(&myString, 'a');
// 	debugStrPrint(&myString);
// 	strAddChar(&myString, 'a');
// 	debugStrPrint(&myString);
// 	strClear(&myString);
// 	debugStrPrint(&myString);	
// 	strAddChar(&myString, 'a');
// 	debugStrPrint(&myString);
// 	strAddChar(&myString, 'a');
// 	debugStrPrint(&myString);
// 	strAddChar(&myString, 'a');
// 	debugStrPrint(&myString);
// 	strFree(&myString);
// 	debugStrPrint(&myString);
// 	strAddChar(&myString, 'a');
// 	debugStrPrint(&myString);
// }
































