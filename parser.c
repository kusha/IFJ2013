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
	
	Parser file - main processing part
*/

#include <stdio.h>
#include "main.h"
#include "lexer.h"
#include "parser.h" /*
#include "instructions.h" via parser.h */ 

/*	//DEBUG ONLY
char *debugTokens(int token){
	switch (token) {
		case 0 : 	return "PHP                 "; break;
		case 1 : 	return "END                 "; break;
		case 10: 	return "IDENTIFIER          "; break;
		case 11: 	return "IDENTIFIER_VARIABLE "; break;
		case 20: 	return "KEYWORD_ELSE        "; break;
		case 21: 	return "KEYWORD_FUNCTION    "; break;
		case 22: 	return "KEYWORD_IF          "; break;
		case 23: 	return "KEYWORD_RETURN      "; break;
		case 24: 	return "KEYWORD_WHILE       "; break;
		case 30: 	return "LITERAL_NULL        "; break;
		case 31: 	return "LITERAL_LOGICAL     "; break;
		case 32: 	return "LITERAL_INETEGER    "; break;
		case 33: 	return "LITERAL_DOUBLE      "; break;
		case 34: 	return "LITERAL_STRING      "; break;
		case 40: 	return "COMMA               "; break;
		case 41: 	return "SEMICOLON           "; break;
		case 42: 	return "LEFT_BRACKET        "; break;
		case 43: 	return "RIGHT_BRACKET       "; break;
		case 44: 	return "LEFT_CURLY_BRACKET  "; break;
		case 45: 	return "RIGHT_CURLY_BRACKET "; break;
		case 50: 	return "OPERATION_ASSIGN    "; break;
		case 51: 	return "OPERATION_PLUS      "; break;
		case 52: 	return "OPERATION_MINUS     "; break;
		case 53: 	return "OPERATION_MULTIPLY  "; break;
		case 54: 	return "OPERATION_DIVIDE    "; break;
		case 55: 	return "OPERATION_CONCATEN  "; break;
		case 60: 	return "COMPARE_IS          "; break;
		case 61: 	return "COMPARE_IS_NOT      "; break;
		case 62: 	return "COMPARE_LESS        "; break;
		case 63: 	return "COMPARE_LESS_EQ     "; break;
		case 64: 	return "COMPARE_MORE        "; break;
		case 65: 	return "COMPARE_MORE_EQ     "; break;
		default:	return "NO_TOKEN_WARN       "; break;
	}
}
*/

// global variables

string attribute;			// atribute string from lexer
int tokenType;				// token type from lexer
typeList *instrList;	// list of instructions - common
							// symbol tables

// internal prototypes

/* parseStarter prototype already in header file */

// generate functions

void createInstruction(/*int instrCode, void* addressOne, void* addressTwo, void* addressThree*/) {
	typeInstruction instruction;
	instruction.instrCode = 0;
	instruction.addressOne = NULL;		//DEBUG ONLY!!!!
	instruction.addressTwo = NULL;
	instruction.addressThree = NULL;
	listAdd(instrList, instruction);
}

// starter function 

int parseStarter(/* pointers to sumbol table, instruction list*/typeList *instructionList) {
	if (DEBUG_FLAG) printf("Parser start\n");

	// delegate instrusction list and tables to global variables
	instrList = instructionList;

	// init string for atribute
	if (strInit(&attribute) == STR_ERROR) {
		if (DEBUG_FLAG) printf("Atribute string allocation error\n");
		return INTERNAL_ERROR;
	}

	// test of interpreter
	createInstruction();
	createInstruction();

	/* 

	DEBUG ONLY, PRINTS ALL TOKENS

	while (tokenType != END) {
		tokenType = getToken(&attribute);
		if (tokenType == LEXER_ERROR) {
			return LEXICAL_ERROR; //lexical error reached
		} else {
			if (DEBUG_FLAG) printf("Token: %s\t%s\n",debugTokens(tokenType),attribute.str);
			strClear(&attribute);
		}
	}

	*/

	//free atribute string
	strFree(&attribute);

	if (DEBUG_FLAG) printf("Parser complete\n");
	return SUCCESS;
}

// recursive descent

// precedence parser


