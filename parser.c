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
#include "tables.h" 

///*	//DEBUG ONLY
char *debugTokens(int token) {
	switch (token) {
		case 0 : 	return "PHP"				; break;
		case 1 : 	return "END"				; break;
		case 10: 	return "IDENTIFIER"			; break;
		case 11: 	return "IDENTIFIER_VARIABLE"; break;
		case 20: 	return "KEYWORD_ELSE"		; break;
		case 21: 	return "KEYWORD_FUNCTION"	; break;
		case 22: 	return "KEYWORD_IF"			; break;
		case 23: 	return "KEYWORD_RETURN"		; break;
		case 24: 	return "KEYWORD_WHILE"		; break;
		case 30: 	return "LITERAL_NULL"		; break;
		case 31: 	return "LITERAL_LOGICAL"	; break;
		case 32: 	return "LITERAL_INETEGER"	; break;
		case 33: 	return "LITERAL_DOUBLE"		; break;
		case 34: 	return "LITERAL_STRING"		; break;
		case 40: 	return "COMMA"				; break;
		case 41: 	return "SEMICOLON"			; break;
		case 42: 	return "LEFT_BRACKET"		; break;
		case 43: 	return "RIGHT_BRACKET"		; break;
		case 44: 	return "LEFT_CURLY_BRACKET"	; break;
		case 45: 	return "RIGHT_CURLY_BRACKET"; break;
		case 50: 	return "OPERATION_ASSIGN"	; break;
		case 51: 	return "OPERATION_PLUS"		; break;
		case 52: 	return "OPERATION_MINUS"	; break;
		case 53: 	return "OPERATION_MULTIPLY"	; break;
		case 54: 	return "OPERATION_DIVIDE"	; break;
		case 55: 	return "OPERATION_CONCATEN"	; break;
		case 60: 	return "COMPARE_IS "		; break;
		case 61: 	return "COMPARE_IS_NOT"		; break;
		case 62: 	return "COMPARE_LESS"		; break;
		case 63: 	return "COMPARE_LESS_EQ"	; break;
		case 64: 	return "COMPARE_MORE"		; break;
		case 65: 	return "COMPARE_MORE_EQ"	; break;
		default:	return "NO_TOKEN_WARN"		; break;
	}
}
//*/

// global variables

string attribute;			// attribute string from lexer
int tokenType;				// token type from lexer
typeList *instrList;	// list of instructions - common
							// symbol tables
int recoverFlag = 0;

int expectedTokenType;		// for debug


// internal prototypes

/* parseStarter prototype already in header file */
#define PROGRAM			r0
#define PROGRAM_UNITS	r1
#define FUNC_DEFINE		r2
#define PARAMS			r3
#define PARAMS_MORE		r4
#define CMD_SEQUENCE	r5
#define CMD				r6
#define INPUT			r7
#define INPUT_MORE		r8
#define PRECEDENCE 		parserPrecedence

int PROGRAM();
int PROGRAM_UNITS();
int FUNC_DEFINE();
int PARAMS();
int PARAMS_MORE();
int CMD_SEQUENCE();
int CMD();
int INPUT();
int INPUT_MORE();

int parserPrecedence();

// DEBUG COLORS!!! TO DELETE
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"


#define UPDATE_TOKEN																			\
	/* if (DEBUG_FLAG) printf(KRED "Token: %s\t%s\n" KBLU,debugTokens(tokenType),attribute.str); */	\
	if (recoverFlag) {																			\
		recoverFlag = 0;																		\
	} else {																					\
		strClear(&attribute);																	\
		if ((tokenType=getToken(&attribute))==LEXER_ERROR) return LEXICAL_ERROR;				\
	}

#define RECOVER_TOKEN																					\
	/* if (DEBUG_FLAG) printf(KMAG "RECOVERED TOKEN: %s\t%s\n" KBLU,debugTokens(tokenType),attribute.str); */	\
	recoverFlag = 1;

#define CALL(RULE) \
	if ((status = RULE())!=SYNTAX_OK) { \
		/* printf(KGRN "debug! ACTUAL:%s\n" KBLU,debugTokens(tokenType)); */ \
		return status; \
	} 

#define IS_TOKEN(NEEDED) \
	expectedTokenType = NEEDED; \
	/* printf(KYEL "ACTUAL:%s\tNEED:%s\n"KBLU,debugTokens(tokenType),debugTokens(NEEDED)); */ \
	if (tokenType != NEEDED) { return SYNTAX_WRONG; } else { expectedTokenType = -1; }

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

	// init string for attribute
	if (strInit(&attribute) == STR_ERROR) {
		if (DEBUG_FLAG) printf("Attribute string allocation error\n");
		return INTERNAL_ERROR;
	}

	// // test of interpreter
	// createInstruction();
	// createInstruction();


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

	int statusCode;

	UPDATE_TOKEN
	statusCode = PROGRAM();

	//free attribute string
	strFree(&attribute);

	if (DEBUG_FLAG) printf("Parser complete with code %i\n", statusCode);
	return statusCode;
}

// recursive descent

int PROGRAM() {
	int status;
	switch (tokenType) {
		case PHP:
			// <program> -> PHP <program_units>
			if (DEBUG_FLAG) printf("<program> -> PHP <program_units>\n");
			UPDATE_TOKEN
			CALL(PROGRAM_UNITS)
			// no instructions for this rule
			return SYNTAX_OK;
			break;
	}
	return SYNTAX_WRONG;
}

int PROGRAM_UNITS() {
	int status;
	switch (tokenType) {
		case KEYWORD_FUNCTION:
			// <program_units> -> <func_define> <program_units>
			if (DEBUG_FLAG) printf("<program_units> -> <func_define> <program_units>\n");
			CALL(FUNC_DEFINE)
			UPDATE_TOKEN
			CALL(PROGRAM_UNITS)
			return SYNTAX_OK;
			break;

		case IDENTIFIER_VARIABLE:
		case KEYWORD_IF:
		case KEYWORD_WHILE:
		case KEYWORD_RETURN:
			// <program_units> -> <cmd_sequence> <program_units>
			if (DEBUG_FLAG) printf("<program_units> -> <cmd_sequence> <program_units>\n");
			CALL(CMD_SEQUENCE)
			UPDATE_TOKEN
			CALL(PROGRAM_UNITS)
			return SYNTAX_OK;
			break;

		case END:
			// <program_units>	-> EOF
			if (DEBUG_FLAG) printf("<program_units>	-> EOF\n");
			// instructions for this rule
			return SYNTAX_OK;
			break;

	}
	return SYNTAX_WRONG;
}

int FUNC_DEFINE() {
	// <func_define> -> function id ( <params> ) { <cmd_sequence> }
	if (DEBUG_FLAG) printf("<func_define> -> function id ( <params> ) { <cmd_sequence> }\n");
	int status;
	IS_TOKEN(KEYWORD_FUNCTION)
	UPDATE_TOKEN
	IS_TOKEN(IDENTIFIER)
	UPDATE_TOKEN
	IS_TOKEN(LEFT_BRACKET)
	UPDATE_TOKEN
	CALL(PARAMS)
	UPDATE_TOKEN
	IS_TOKEN(RIGHT_BRACKET)
	UPDATE_TOKEN
	IS_TOKEN(LEFT_CURLY_BRACKET)
	UPDATE_TOKEN
	CALL(CMD_SEQUENCE)
	UPDATE_TOKEN
	IS_TOKEN(RIGHT_CURLY_BRACKET)
	return SYNTAX_OK;
}

int PARAMS() {
	int status;
	switch (tokenType) {
		case RIGHT_BRACKET:		//WARNING NOT WORKING () in expressions!
			// <params> -> E : )
			if (DEBUG_FLAG) printf("<params> -> E : )\n");
			RECOVER_TOKEN
			return SYNTAX_OK;
			break;

		case IDENTIFIER_VARIABLE:
			// <params> -> $id <params_more>
			if (DEBUG_FLAG) printf("<params> -> $id <params_more>\n");
			UPDATE_TOKEN
			CALL(PARAMS_MORE)
			return SYNTAX_OK;
			break;

	}
	return SYNTAX_WRONG;
}

int PARAMS_MORE() {
	int status;
	switch (tokenType) {
		case RIGHT_BRACKET:		//WARNING NOT WORKING () in expressions!
			// <params_more> -> E : )
			if (DEBUG_FLAG) printf("<params_more> -> E : )\n");
			RECOVER_TOKEN
			return SYNTAX_OK;
			break;

		case COMMA:
			// <params_more> -> , $id <params_more>
			if (DEBUG_FLAG) printf("<params_more> -> , $id <params_more>\n");
			UPDATE_TOKEN
			IS_TOKEN(IDENTIFIER_VARIABLE)
			UPDATE_TOKEN
			CALL(PARAMS_MORE)
			return SYNTAX_OK;
			break;

	}
	return SYNTAX_WRONG;
}

int CMD_SEQUENCE() {
	int status;
	switch (tokenType) {
		case RIGHT_CURLY_BRACKET:
		case KEYWORD_FUNCTION:
		case END:
			// <cmd_sequence> -> E : } function EOF
			if (DEBUG_FLAG) printf("<cmd_sequence> -> E : } function EOF\n");
			RECOVER_TOKEN
			return SYNTAX_OK;
			break;

		case IDENTIFIER_VARIABLE:
		case KEYWORD_IF:
		case KEYWORD_WHILE:
		case KEYWORD_RETURN:
			// <cmd_sequence> -> <cmd> <cmd_sequence>
			if (DEBUG_FLAG) printf("<cmd_sequence> -> <cmd> <cmd_sequence>\n");
			CALL(CMD)
			UPDATE_TOKEN
			CALL(CMD_SEQUENCE)
			return SYNTAX_OK;
			break;

	}
	return SYNTAX_WRONG;
}

int CMD() {
	int status;
	switch (tokenType) {
		case IDENTIFIER_VARIABLE:
			// <cmd> -> $id = <expression> ;
			// <cmd> -> $id = id ( <input> ) ;
			UPDATE_TOKEN
			IS_TOKEN(OPERATION_ASSIGN)
			UPDATE_TOKEN
			switch (tokenType) {
				case IDENTIFIER:
					// <cmd> -> $id = id ( <input> ) ;
					if (DEBUG_FLAG) printf("<cmd> -> $id = id ( <input> ) ;\n");
					UPDATE_TOKEN
					IS_TOKEN(LEFT_BRACKET)
					UPDATE_TOKEN
					CALL(INPUT)
					UPDATE_TOKEN
					IS_TOKEN(RIGHT_BRACKET)
					UPDATE_TOKEN
					IS_TOKEN(SEMICOLON)
					return SYNTAX_OK;
					break;

				case LEFT_BRACKET:	//added left bracket( expression can start with it)
				case IDENTIFIER_VARIABLE:
				case LITERAL_NULL:
				case LITERAL_LOGICAL:
				case LITERAL_INETEGER:
				case LITERAL_DOUBLE:
				case LITERAL_STRING:
				case OPERATION_PLUS:
				case OPERATION_MINUS:
				case OPERATION_MULTIPLY:
				case OPERATION_DIVIDE:
				case OPERATION_CONCATEN:
				case COMPARE_IS:
				case COMPARE_IS_NOT:
				case COMPARE_LESS:
				case COMPARE_MORE:
				case COMPARE_MORE_EQ:
					// <cmd> -> $id = <expression> ;
					if (DEBUG_FLAG) printf("<cmd> -> $id = <expression> ;\n");
					CALL(PRECEDENCE)
					UPDATE_TOKEN
					IS_TOKEN(SEMICOLON)
					return SYNTAX_OK;
					break;
			}
			return SYNTAX_WRONG;
			break;

		case KEYWORD_IF:
			// <cmd> -> if ( <expression> ) { <cmd_sequence> } else { <cmd_sequence> }
			if (DEBUG_FLAG) printf("<cmd> -> if ( <expression> ) { <cmd_sequence> } else { <cmd_sequence> }\n");
			UPDATE_TOKEN
			IS_TOKEN(LEFT_BRACKET)
			UPDATE_TOKEN
			CALL(PRECEDENCE)
			UPDATE_TOKEN
			IS_TOKEN(RIGHT_BRACKET)
			UPDATE_TOKEN
			IS_TOKEN(LEFT_CURLY_BRACKET)
			UPDATE_TOKEN
			CALL(CMD_SEQUENCE)
			UPDATE_TOKEN
			IS_TOKEN(RIGHT_CURLY_BRACKET)
			UPDATE_TOKEN
			IS_TOKEN(KEYWORD_ELSE)
			UPDATE_TOKEN
			IS_TOKEN(LEFT_CURLY_BRACKET)
			UPDATE_TOKEN
			CALL(CMD_SEQUENCE)
			UPDATE_TOKEN
			IS_TOKEN(RIGHT_CURLY_BRACKET)
			return SYNTAX_OK;
			break;

		case KEYWORD_WHILE:
			// <cmd> -> while ( <expression> ) { <cmd_sequence> }
			if (DEBUG_FLAG) printf("<cmd> -> while ( <expression> ) { <cmd_sequence> }\n");
			UPDATE_TOKEN
			IS_TOKEN(LEFT_BRACKET)
			UPDATE_TOKEN
			CALL(PRECEDENCE)
			UPDATE_TOKEN
			IS_TOKEN(RIGHT_BRACKET)
			UPDATE_TOKEN
			IS_TOKEN(LEFT_CURLY_BRACKET)
			UPDATE_TOKEN
			CALL(CMD_SEQUENCE)
			UPDATE_TOKEN
			IS_TOKEN(RIGHT_CURLY_BRACKET)
			return SYNTAX_OK;
			break;

		case KEYWORD_RETURN:
			// <cmd> -> return <expression> ;
			if (DEBUG_FLAG) printf("<cmd> -> return <expression> ;\n");
			UPDATE_TOKEN
			CALL(PRECEDENCE)
			UPDATE_TOKEN
			IS_TOKEN(SEMICOLON)
			return SYNTAX_OK;
			break;

	}
	return SYNTAX_WRONG;
}

int INPUT() {
	int status;
	switch (tokenType) {
		case RIGHT_BRACKET:		//WARNING NOT WORKING () in expressions!
			// <input> -> E : )
			if (DEBUG_FLAG) printf("<input> -> E : )\n");
			RECOVER_TOKEN
			return SYNTAX_OK;
			break;

		case LEFT_BRACKET:	//added left bracket( expression can start with it)
		case IDENTIFIER_VARIABLE:
		case LITERAL_NULL:
		case LITERAL_LOGICAL:
		case LITERAL_INETEGER:
		case LITERAL_DOUBLE:
		case LITERAL_STRING:
		case OPERATION_PLUS:
		case OPERATION_MINUS:
		case OPERATION_MULTIPLY:
		case OPERATION_DIVIDE:
		case OPERATION_CONCATEN:
		case COMPARE_IS:
		case COMPARE_IS_NOT:
		case COMPARE_LESS:
		case COMPARE_MORE:
		case COMPARE_MORE_EQ:
			// <input> -> <expression> <input_more>
			if (DEBUG_FLAG) printf("<input> -> <expression> <input_more>\n");
			CALL(PRECEDENCE)
			UPDATE_TOKEN
			CALL(INPUT_MORE)
			return SYNTAX_OK;
			break;

	}
	return SYNTAX_WRONG;
}

int INPUT_MORE() {
	int status;
	switch (tokenType) {
		case RIGHT_BRACKET:		//WARNING NOT WORKING () in expressions!
			// <input_more> -> E : )
			if (DEBUG_FLAG) printf("<input_more> -> E : )\n");
			RECOVER_TOKEN
			return SYNTAX_OK;
			break;

		case COMMA:
			// <input_more> -> , <expression> <input_more>
			if (DEBUG_FLAG) printf("<input_more> -> , <expression> <input_more>\n");
			UPDATE_TOKEN
			CALL(PRECEDENCE)
			UPDATE_TOKEN
			CALL(INPUT_MORE)
			return SYNTAX_OK;
			break;

	}
	return SYNTAX_WRONG;
}


// Precedence Parser Implementation

// global section of precedence parser

#define TERM 		1 			//OPERATORS
#define NOTERM 		0 			//OPERANDS
#define WHATEVER	-1

int bracketsCounter = 0;

int expectedType = WHATEVER; // -1 ?! 0 term 1 noterm 
int currentType = WHATEVER; // -1 ?! 0 term 1 noterm

int parserPrecedence() {

	printf("TOKEN: %s\t EXPECTED: %i\n", debugTokens(tokenType), expectedType);

	typeData * actualNoterm;
	int actualTerm;

	// prepare stack nodes
	switch (tokenType) {
		case IDENTIFIER_VARIABLE:
		case LITERAL_NULL:
		case LITERAL_LOGICAL:
		case LITERAL_INETEGER:
		case LITERAL_DOUBLE:
		case LITERAL_STRING:
			//noterm at input
			if (expectedType == TERM) {
				if (DEBUG_FLAG) printf("unexepected noterm in expression\n");
				return SYNTAX_WRONG;	//unexpected token
			} else {
				expectedType = TERM;
			}
			if (tokenType == IDENTIFIER_VARIABLE) {
				actualNoterm = getVariable(&attribute, SHOULD_EXIST);
			} else {
				actualNoterm = getLiteral(tokenType-30, &attribute); //TOKEN -> DATA_TYPE
			}
			if (actualNoterm == NULL) {
				if (DEBUG_FLAG) printf("STOP! SEMANTIC ERROR! UNDEFINED VAR IN EXPRESSION\n");
			}
			currentType = NOTERM;
			break;

		case OPERATION_PLUS:
		case OPERATION_MINUS:
		case OPERATION_MULTIPLY:
		case OPERATION_DIVIDE:
		case OPERATION_CONCATEN:
		case COMPARE_IS:
		case COMPARE_IS_NOT:
		case COMPARE_LESS:
		case COMPARE_MORE:
		case COMPARE_MORE_EQ:
			//term at input
			if (expectedType == NOTERM) {
				if (DEBUG_FLAG) printf("unexepected noterm in expression\n");
				return SYNTAX_WRONG;	//unexpected token
			} else {
				expectedType = NOTERM;
			}
			actualTerm = tokenType;
			currentType = TERM;
			break;

		case LEFT_BRACKET:
			// firstly control tokens
			if (currentType != NOTERM) {		//NOT noterm bcs can be first!!!
				currentType = TERM;
				expectedType = NOTERM; 		//next after ( will be NOTERM
			} else {
				if (DEBUG_FLAG) printf("unexepected left bracket\n");
				return SYNTAX_WRONG;	//unexpected token
			}
			//and than bracket counter
			++bracketsCounter;
			//update term
			actualTerm = tokenType;
			break;

		case RIGHT_BRACKET:
			// firstly control tokens
			if (currentType != TERM) {		//NOT term bcs can be first!!! ?!WTF
				currentType = NOTERM;
				expectedType = TERM; 		//next after ( will be NOTERM
			} else {
				if (DEBUG_FLAG) printf("unexepected right bracket\n");
				return SYNTAX_WRONG;	//unexpected token
			}
			//and than bracket counter
			--bracketsCounter;
			if (bracketsCounter<0) {	//exit right bracket position
				if (DEBUG_FLAG) printf("1exit from PP with recovered %s\n", debugTokens(tokenType));
				RECOVER_TOKEN
				return SYNTAX_OK;
			}
			//update term
			actualTerm = tokenType;
			break;

		default:
			//maybe token isn't for expression?
			if (bracketsCounter != 0) {
				if (DEBUG_FLAG) printf("exit from expression with brackets error\n");
				return SYNTAX_WRONG;	//unexpected token
			}
			RECOVER_TOKEN
			return SYNTAX_OK;
	}

	//ready to continue
	UPDATE_TOKEN
	return parserPrecedence();
}


// 	printf("%s ", debugTokens(tokenType));

// 	int status;
// 	switch (tokenType) {
// 		case IDENTIFIER_VARIABLE:
// 		case LITERAL_NULL:
// 		case LITERAL_LOGICAL:
// 		case LITERAL_INETEGER:
// 		case LITERAL_DOUBLE:
// 		case LITERAL_STRING:
// 		case OPERATION_PLUS:
// 		case OPERATION_MINUS:
// 		case OPERATION_MULTIPLY:
// 		case OPERATION_DIVIDE:
// 		case OPERATION_CONCATEN:
// 		case COMPARE_IS:
// 		case COMPARE_IS_NOT:
// 		case COMPARE_LESS:
// 		case COMPARE_MORE:
// 		case COMPARE_MORE_EQ:
// 			// <expression> -> <expression>
// 			if (DEBUG_FLAG) printf("<expression> -> <expression>\n");
// 			UPDATE_TOKEN
// 			parserPrecedence();
// 			return SYNTAX_OK;

// 		default:
// 			// <program_units>	-> EOF
// 			if (DEBUG_FLAG) printf("<expression> -> E\n");
// 			// instructions for this rule
// 			RECOVER_TOKEN
// 			return SYNTAX_OK;

// 	}
// 	return SYNTAX_WRONG;
// }

// int recursivePrecedence () {
// 	return SYNTAX_OK;
// }




















