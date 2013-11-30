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
	if (DEBUG_FLAG) printf("\n");
	switch (statusCode) {
		case SUCCESS:	
			if (DEBUG_FLAG) printf("\t["KGRN"OK"KNRM"]\tLexical and syntax analysis\n\n");
			printf("\r");
			break;

		default:
			printf(KGRN"\t%i"KNRM":"KGRN"%i\t"KNRM,troubleLine,troubleColumn);
			switch (statusCode) {
				case LEXICAL_ERROR:
					printf(KRED"lexical error:\t"KNRM" unexpected token at symbol '"KYEL"%c"KNRM"'\n\n", troubleCharacter);
					break;
				case PARSER_ERROR:
					printf(KRED"syntax error:\t"KNRM);
					if (expectedTokenType == -1) {
						printf("unexpected rule at token "KYEL"%s"KNRM, debugTokens(tokenType));
					} else {
						printf("expected token is "KYEL"%s"KNRM" instead of "KYEL"%s"KNRM, debugTokens(expectedTokenType), debugTokens(tokenType));
					}
					printf("\n");
					break;
			}
			if (DEBUG_FLAG) printf("\t["KRED"FAIL"KNRM"]\tLexical and syntax analysis\n");
			CLOSE_ALL
			if (DEBUG_FLAG) printf("\t["KBLU"INFO"KNRM"]\tExit with status code %i\n\n",statusCode);
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




