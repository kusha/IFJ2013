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
**	Defines for all units of the project.
**
** -------------------------------------------------------------------------*/


#ifndef __COMMON_H__
#define __COMMON_H__


#define DEBUG_FLAG		0 	// print debug output flag
#define REPORT(MSG)		if (DEBUG_FLAG) fprintf(stderr,MSG"\n");

#define CNRM	"\x1B[0m"	// print color code: normal
#define CRED	"\x1B[31m"	// print color code: red
#define CGRN	"\x1B[32m"	// print color code: green
#define CYEL	"\x1B[33m"	// print color code: yellow
#define CBLU	"\x1B[34m"	// print color code: blue
#define CMAG	"\x1B[35m"	// print color code: magenta

#define SUCCESS			0 	// exit codes: successfull operation
#define LEXICAL_ERROR	1	// exit codes: lexical analysis error
#define PARSER_ERROR	2	// exit codes: syntax analysis error

#define S_FUNC_ERROR	3	// exit codes: semantic error: un/re def function
#define S_PARAM_ERROR	4	// exit codes: semantic error: func params error
#define S_DECLAR_ERROR	5	// exit codes: semantic error: undeclared var
#define S_ZERODIV_ERROR	10	// exit codes: semantic error: divide by zero
#define S_TYPE_ERROR	11	// exit codes: semantic error: type conversion
#define S_EXPRESS_ERROR	12	// exit codes: semantic error: expression comptab
#define S_OTHER_ERROR	13	// exit codes: semantic error: another case

#define INTERNAL_ERROR	99	// exit codes: internal interpreter error
#define ALLOC_FAIL 		-1	// error occurred during allocation

extern int troubleLine;
extern int troubleColumn;
extern int troubleCharacter;

#endif
