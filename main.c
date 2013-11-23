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
#include "parser.h"

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

	int statusCode;
	//processing section
	//STEP 1 init symbol table
	//STEP 2 init instrukctions list
	statusCode = parsePrimary(/* pointers to sumbol table, instruction list*/);
	switch (statusCode) {
		case SUCCESS:	
			if (DEBUG_FLAG) printf("lexer+parser OK\n");
			break;
		default:		
			if (DEBUG_FLAG) printf("lexer+parser ERROR\n");
			//destroy tables, lists
			fclose(file);
			return statusCode;
			break;
	}

	//symbols tables and instrukctions debugging
	//statusCode = interpreterRun(/* pointers to sumbol table, instruction list*/);
	//check returned status code

	//destroy tables, lists
	fclose(file);

	return statusCode;
}




