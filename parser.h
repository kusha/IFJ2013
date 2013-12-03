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
	
	Primary and Secondary parser units header file

	uses in main.c ...
*/

#ifndef __PARSER_H__
#define __PARSER_H__

#include "common.h"
#include "lexer.h"
#include "tables.h"
#include "stack.h"
#include "instructions.h"
#include "arrays.h"


// #include "stack.h"

#define SYNTAX_OK		0
#define SYNTAX_WRONG	2

//common resources
extern int tokenType;
extern int expectedTokenType;

// shared functions prototypes section
int parseStarter(/* pointers to sumbol table, instruction list*/typeList *instructionList);
char *debugTokens(int token);
void initTables();


#endif
