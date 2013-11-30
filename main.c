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
**	Main program - control flow of the program.
**
** -------------------------------------------------------------------------*/


/* -- Includes part --------------------------------------------------------*/

#include <stdio.h>	// for file opening
#include "main.h"	// every .c file include same .h file


/* -- Macros definition ----------------------------------------------------*/

#define CLOSE_ALL					\
	listDispose(&instructionList);	\
	fclose(file);


/* -- Main program flow ----------------------------------------------------*/

int main (int argc, char * argv[]) {

	/* -- Check input, delegate file pointer -------------------------------*/

	FILE *file;
	if (argc == 1 || argc > 2) {
		REPORT("Wrong input params.")
		HALT(INTERNAL_ERROR)
	}
	if ((file = fopen(argv[1],"r")) == NULL) {
		REPORT("File opening error.")
		HALT(INTERNAL_ERROR)
	}
	delegateSourceFile(file);


	/* -- Prepare instruction list -----------------------------------------*/

	typeList instructionList;
	listInit(&instructionList);


	/* -- Common variables -------------------------------------------------*/

	int statusCode;


	/* -- Try to run parser ------------------------------------------------*/
	
	statusCode = parseStarter(&instructionList/*maybe also tables?*/);

	switch (statusCode) {
		case SUCCESS:	
			REPORT("Lexical and syntax analysis - OK.")
			break;

		default:
			switch (statusCode) {
				case LEXICAL_ERROR:
					REPORT("Lexical analysis failure.")
					break;

				case PARSER_ERROR:
					REPORT("Syntax analysis failure.")
					break;
			}
			CLOSE_ALL
			HALT(statusCode)
			break;
	}


	/* -- Try to interprete instruction list -------------------------------*/

	statusCode = interpreterStart(/* same input like parser*/&instructionList);

	switch (statusCode) {
		case SUCCESS:	
			REPORT("Interpreter - OK.")
			break;

		default:
			// TODO switch for semantic errors
			REPORT("Interpreter failure.")
			CLOSE_ALL
			HALT(statusCode)
			break;
	}
	

	/* -- Conclusion -------------------------------------------------------*/

	CLOSE_ALL
	HALT(statusCode)

}




