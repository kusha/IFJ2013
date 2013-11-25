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
	
	Main file - control flow of the program
*/

#include <stdio.h>

#include "main.h"
#include "lexer.h"
#include "parser.h" /*
#include "instructions.h" via parser.h */ 
#include "interpreter.h"

#define CLOSE_ALL \
	listDispose(&instructionList); \
	fclose(file);


int main (int argc, char*argv[]) {

	//input data section
	FILE *file;
	if (argc == 1 || argc > 2) {
		if (DEBUG_FLAG) printf("wrong program params\n");
		return INTERNAL_ERROR;
	}
	if ((file = fopen(argv[1],"r")) == NULL) {
		if (DEBUG_FLAG) printf("file opening error\n");
		return INTERNAL_ERROR;
	}
	delegateSourceFile(file); //delegate sources to scanner

	//prepare data structures
	typeList instructionList;
	listInit(&instructionList);

	int statusCode;
	
	statusCode = parseStarter(/* pointers to sumbol table, instruction list*/&instructionList);
	switch (statusCode) {
		case SUCCESS:	
			if (DEBUG_FLAG) printf("lexer+parser OK\n");
			break;
		default:		
			if (DEBUG_FLAG) printf("lexer+parser ERROR\n");
			CLOSE_ALL
			return statusCode;
			break;
	}

	statusCode = interpreterStart(/* same input like parser*/&instructionList);
	switch (statusCode) {
		case SUCCESS:	
			if (DEBUG_FLAG) printf("interpreter OK\n");
			break;
		default:		
			if (DEBUG_FLAG) printf("interpreter ERROR\n");
			CLOSE_ALL
			return statusCode;
			break;
	}
	
	CLOSE_ALL	//destroy tables, lists, close file

	return statusCode;
}




