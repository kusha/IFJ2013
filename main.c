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
			break;

		default:
			switch (statusCode) {
				case LEXICAL_ERROR:
					fprintf(stderr,"Lexical analysis failure.\n");
					fprintf(stderr,"Incident caused at %i:%i.\n",
						troubleLine, troubleColumn);
					break;

				case PARSER_ERROR:
					fprintf(stderr,"Syntax analysis failure.\n");
					break;

				case INTERNAL_ERROR:
					fprintf(stderr,"Internal interpreter error.\n");
					break;

				case S_FUNC_ERROR:
					fprintf(stderr,"Semantic error: undef/redef of the function.\n");
					break;

				case S_PARAM_ERROR:
					fprintf(stderr,"Semantic error: function call params.\n");
					break;

				case S_DECLAR_ERROR:
					fprintf(stderr,"Semantic error: undeclared variable.\n");
					break;
			}
			CLOSE_ALL
			HALT(statusCode)
			break;
	}


	/* -- Try to interprete instruction list -------------------------------*/

	statusCode = interpreterStart(/*same input like parser*/&instructionList);

	switch (statusCode) {
		case SUCCESS:
			break;

		default:
			switch (statusCode) {
				case INTERNAL_ERROR:
					fprintf(stderr,"Internal interpreter error.\n");
					break;

				case S_EXPRESS_ERROR:
					fprintf(stderr,"Semantic error: operation comptability.\n");
					break;

				case S_ZERODIV_ERROR:
					fprintf(stderr,"Semantic error: divide by zero.\n");
					break;

				case S_TYPE_ERROR:
					fprintf(stderr,"Semantic error: type conversion error.\n");
					break;

				case S_OTHER_ERROR:
					fprintf(stderr,"Semantic error: other error.\n");
					break;
			}
			CLOSE_ALL
			HALT(statusCode)
			break;
	}
	

	/* -- Conclusion -------------------------------------------------------*/

	CLOSE_ALL
	HALT(statusCode)

}
