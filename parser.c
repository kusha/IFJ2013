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
**	Descripition of parser.
**
** -------------------------------------------------------------------------*/



#include <stdio.h>
#include "parser.h"

#undef DEBUG_FLAG
#define DEBUG_FLAG 0

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

void createInstruction(int instrCode, typeData * addressOne, typeData * addressTwo, typeData * addressThree) {
	printf(CGRN"Called instruction creation: %i\n"CNRM,instrCode);
	typeInstruction instruction;
	instruction.instrCode = instrCode;
	instruction.addressOne = addressOne;
	instruction.addressTwo = addressTwo;
	instruction.addressThree = addressThree;
	if (listAdd(instrList, instruction)!=SUCCESS) {
		printf(CRED"Error in list addition\n"CNRM);
	}
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
				case COMPARE_LESS_EQ:
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

/* -- Precedence Parser ------------------------------------------------------
**
**	Input:		updated token + typeData pointer
**	Output:		instructions + result pointer + recovered token + result code
**
**	Desciption of the algorithm.
**
** -------------------------------------------------------------------------*/


/* -- Macro definitions ----------------------------------------------------*/

#undef DEBUG_FLAG
#define DEBUG_FLAG 1

#define EXPRESSION_END 15

#define PLess 0
#define PMore 1
#define PEqual 2
#define PError -1


/* -- Priority table ---------------------------------------------------------
**
**	Incoming tokens:

**
** -------------------------------------------------------------------------*/



#define M	PMore
#define L	PLess
#define E	PEqual
#define W	PError

// IDENTIFIER_VARIABLE
// LITERAL_NULL
// LITERAL_LOGICAL
// LITERAL_INETEGER
// LITERAL_DOUBLE
// LITERAL_STRING

static int priorityTable [16][16] = {
/* INCOMING > *   /   +   -   .   <   >   <=  >= === !== ID  STR  (   )   $ /*/
/*/	 *	/*/	{ M , M , M , M , M , M , M , M , M , M , M , M , M , M , M , M } ,
/*/	 /	/*/	{ M , M , M , M , M , M , M , M , M , M , M , M , M , M , M , M } ,
/*/	 +	/*/	{ M , M , M , M , M , M , M , M , M , M , M , M , M , M , M , M } ,
/*/	 -	/*/	{ M , M , M , M , M , M , M , M , M , M , M , M , M , M , M , M } ,
/*/	 .	/*/	{ M , M , M , M , M , M , M , M , M , M , M , M , M , M , M , M } ,
/*/	 <	/*/	{ M , M , M , M , M , M , M , M , M , M , M , M , M , M , M , M } ,
/*/	 >	/*/	{ M , M , M , M , M , M , M , M , M , M , M , M , M , M , M , M } ,
/*/	 <=	/*/	{ M , M , M , M , M , M , M , M , M , M , M , M , M , M , M , M } ,
/*/	 >=	/*/	{ M , M , M , M , M , M , M , M , M , M , M , M , M , M , M , M } ,
/*/	=== /*/	{ M , M , M , M , M , M , M , M , M , M , M , M , M , M , M , M } ,
/*/	!==	/*/	{ M , M , M , M , M , M , M , M , M , M , M , M , M , M , M , M } ,
/*/	 ID	/*/	{ M , M , M , M , M , M , M , M , M , M , M , M , M , M , M , M } ,
/*/	STR	/*/	{ M , M , M , M , M , M , M , M , M , M , M , M , M , M , M , M } ,
/*/	 (	/*/	{ M , M , M , M , M , M , M , M , M , M , M , M , M , M , M , M } ,
/*/	 )	/*/	{ M , M , M , M , M , M , M , M , M , M , M , M , M , M , M , M } ,
/*/	 $	/*/	{ M , M , M , M , M , M , M , M , M , M , M , M , M , M , M , M }
/* ^^^^^ */
/*ALREADY*/
};

#undef M
#undef L
#undef E
#undef W

static int PTable [16][16] = {												//incoming
/*      			*[53]	/[54]	+[51]	-[52]	.[55]	<[62]	>[64]	<=[63]	>=[65]	===[60]	!==[61]	ID[10]	STR[34]	([42]	)[43]	$    */
/* *[53]	*/ {	PMore	,PMore	,PMore	,PMore	,PError	,PMore	,PMore	,PMore	,PMore	,PMore	,PMore	,PLess	,PError	,PLess	,PMore 	,PMore	},
/* /[54]	*/ {	PMore	,PMore	,PMore	,PMore	,PError	,PMore	,PMore	,PMore	,PMore	,PMore	,PMore	,PLess	,PError	,PLess	,PMore 	,PMore	},
/* +[51]	*/ {	PLess	,PLess	,PMore	,PMore	,PError	,PMore	,PMore	,PMore	,PMore	,PMore	,PMore	,PLess	,PError	,PLess	,PMore 	,PMore	},
/* -[52]	*/ {	PLess	,PLess	,PMore	,PMore	,PError	,PMore	,PMore	,PMore	,PMore	,PMore	,PMore	,PLess	,PError	,PLess	,PMore 	,PMore	},
/* .[55]	*/ {	PError	,PError	,PError	,PError	,PMore	,PMore	,PMore	,PMore	,PMore	,PMore	,PMore	,PLess	,PLess	,PLess	,PMore 	,PMore	},
/* <[62]	*/ {	PLess	,PLess	,PLess	,PLess	,PLess	,PError	,PError	,PError	,PError	,PError	,PError	,PLess	,PLess	,PLess	,PLess 	,PMore	},
/* >[64]	*/ {	PLess	,PLess	,PLess	,PLess	,PLess	,PError	,PError	,PError	,PError	,PError	,PError	,PLess	,PLess	,PLess	,PLess 	,PMore	},
/* <=[63]	*/ {	PLess	,PLess	,PLess	,PLess	,PLess	,PError	,PError	,PError	,PError	,PError	,PError	,PLess	,PLess	,PLess	,PLess 	,PMore	},
/* >=[65]	*/ {	PLess	,PLess	,PLess	,PLess	,PLess	,PError	,PError	,PError	,PError	,PError	,PError	,PLess	,PLess	,PLess	,PLess 	,PMore	},
/* ===[60]	*/ {	PLess	,PLess	,PLess	,PLess	,PLess	,PError	,PError	,PError	,PError	,PError	,PError	,PLess	,PLess	,PLess	,PLess 	,PMore	},
/* !==[61]	*/ {	PLess	,PLess	,PLess	,PLess	,PLess	,PError	,PError	,PError	,PError	,PError	,PError	,PLess	,PLess	,PLess	,PLess 	,PMore	},
/* ID[10]	*/ {	PMore	,PMore	,PMore	,PMore	,PMore	,PMore	,PMore	,PMore	,PMore	,PMore	,PMore	,PError	,PError	,PError	,PMore 	,PMore	},
/* STR[34]	*/ {	PError	,PError	,PError	,PError	,PMore	,PMore	,PMore	,PMore	,PMore	,PMore	,PMore	,PError	,PError	,PError	,PMore 	,PMore	},
/* ([42]	*/ {	PLess	,PLess	,PLess	,PLess	,PLess	,PLess	,PLess	,PLess	,PLess	,PLess	,PLess	,PLess	,PLess	,PError	,PEqual ,PMore	},
/* )[43]	*/ {	PMore	,PMore	,PMore	,PMore	,PMore	,PMore	,PMore	,PMore	,PMore	,PMore	,PMore	,PError	,PError	,PError	,PError ,PMore	},
/* $		*/ {	PLess	,PLess	,PLess	,PLess	,PLess	,PLess	,PLess	,PLess	,PLess	,PLess	,PLess	,PLess	,PLess	,PLess	,PLess 	,PEqual	}
//in stack
};


/* -- Helper functions -----------------------------------------------------*/

int tokenToPriority (int token) {
	switch (token){
		case OPERATION_MULTIPLY: 	return 0;
		case OPERATION_DIVIDE: 		return 1;
		case OPERATION_PLUS: 		return 2;
		case OPERATION_MINUS: 		return 3;
		case OPERATION_CONCATEN: 	return 4;
		case COMPARE_LESS: 			return 5;
		case COMPARE_MORE: 			return 6;
		case COMPARE_LESS_EQ: 		return 7;
		case COMPARE_MORE_EQ: 		return 8;
		case COMPARE_IS: 			return 9;
		case COMPARE_IS_NOT: 		return 10;
		case IDENTIFIER_VARIABLE:
		case LITERAL_NULL:
		case LITERAL_LOGICAL:
		case LITERAL_INETEGER:
		case LITERAL_DOUBLE:
									return 11;
		case LITERAL_STRING: 		return 12;
		case LEFT_BRACKET: 			return 13;
		case RIGHT_BRACKET: 		return 14;
		case EXPRESSION_END: 		return 15;
		default: 					return -1;
	}
}

char * debugPreced (int token) {
	switch (token){
		case 0:		return "OPERATION_MULTIPLY"		;
		case 1:		return "OPERATION_DIVIDE"		;
		case 2:		return "OPERATION_PLUS"			;
		case 3:		return "OPERATION_MINUS"		;
		case 4:		return "OPERATION_CONCATEN"		;
		case 5:		return "COMPARE_LESS"			;
		case 6:		return "COMPARE_MORE"			;
		case 7:		return "COMPARE_LESS_EQ"		;
		case 8:		return "COMPARE_MORE_EQ"		;
		case 9:		return "COMPARE_IS"				;
		case 10:	return "COMPARE_IS_NOT"			;
		case 11:	return "IDENTIFIER/LITERAL"		;
		case 12:	return "LITERAL_STRING"			;
		case 13:	return "LEFT_BRACKET"			;
		case 14:	return "RIGHT_BRACKET"			;
		case 15:	return "EXPRESSION_END"			;
		default: 	return "WARNING!!!UNDEF OPER"	;
	}
}

int tableToInstruction (int inTable) {
	switch (inTable){
		case 0: 	return I_MULTIPLY	;
		case 1: 	return I_DIVIDE		;
		case 2: 	return I_PLUS		;
		case 3: 	return I_MINUS		;
		case 4: 	return I_CONCATEN	;
		case 5: 	return I_C_LESS		;
		case 6: 	return I_C_MORE		;
		case 7: 	return I_C_LESS_EQ	;
		case 8: 	return I_C_MORE_EQ	;
		case 9: 	return I_C_IS		;
		case 10: 	return I_C_IS_NOT	;
		default: 	REPORT("Udefined instruction added to list.")
					return -1;
	}
}



char * priorToInstr (int priority) {
	switch (priority){
		case 0: 	return "I_MULTIPLY	";
		case 1: 	return "I_DIVIDE	";
		case 2: 	return "I_PLUS		";
		case 3: 	return "I_MINUS		";
		case 4: 	return "I_CONCATEN	";
		case 5: 	return "I_C_LESS	";
		case 6: 	return "I_C_MORE	";
		case 7: 	return "I_C_LESS_EQ	";
		case 8: 	return "I_C_MORE_EQ	";
		case 9: 	return "I_C_IS		";
		case 10: 	return "I_C_IS_NOT	";
	}
}

char * priorToStr (int priority) {
	switch (priority){
		case PLess:		return "LESS";
		case PMore:		return "MORE";
		case PEqual:	return "EQUAL";
		case PError:	return "ERROR";
	}
}


#define TERM 		1 			//OPERATORS
#define NOTERM 		0 			//OPERANDS
#define WHATEVER	-1

/*
#define PUSH_CURRENT										\
	if (DEBUG_FLAG) printf("just pushed: %s\n", debugTokens(currentToken)); \
	switch (currentType) {									\
		case NOTERM:										\
			stackNotermPush(&notermStack, actualNoterm);	\
			break;											\
		case TERM:											\
			stackTermPush(&termStack, actualTerm);			\
			break;											\
	} */

// int priorityCheck(int alrady, int current, tStackTerm * termStack) {
// 	int currentPrior = tokenToPriority(currentToken);
// 	if (stackTermEmpty(termStack)) {

// 	}
// 	if (currentPrior == -1) {
// 		return PMore;
// 	}
// 	PTable[stackTermTop(&termStack)][tokenToPriority(currentToken)]
// }

int parserPrecedence() {

	int bracketsCounter = 0;

	int currentToken = WHATEVER;
	int lastToken = WHATEVER;

	int flagNoterm = 0;

	// int currentType = WHATEVER;
	// int lastType = WHATEVER;

	tStackTerm termStack;
	stackTermInit(&termStack);

	tStackNoterm notermStack;		// stack of noterms
	stackNotermInit(&notermStack);

	stackTermPush(&termStack, EXPRESSION_END); // push dollar

	//cleared in while

	typeData * actualNoterm;
	// int actualTerm;

	int exitFlag = 0;


	//start of while
	while (1) {

		// actualTerm = 0;

		//update types

		// lastType = currentType;

		printf("\nSTACK CONTENT:\n");
		printTermStack(&termStack);
		printf("tokenType: %s\n", debugTokens(tokenType));

		//NO TERMINAL ALWAYS PUSH
		if (tokenType == IDENTIFIER_VARIABLE 	|| \
			tokenType == LITERAL_NULL 			|| \
			tokenType == LITERAL_LOGICAL 		|| \
			tokenType == LITERAL_INETEGER 		|| \
			tokenType == LITERAL_DOUBLE 		|| \
			tokenType == LITERAL_STRING ) {
			if (tokenType == IDENTIFIER_VARIABLE) {
				actualNoterm = getVariable(&attribute, SHOULD_EXIST);
			} else {
				actualNoterm = getLiteral(tokenType-30, &attribute); //TOKEN -> DATA_TYPE
			}
			if (actualNoterm == NULL) {
				if (DEBUG_FLAG) printf("STOP! SEMANTIC ERROR! UNDEFINED VAR IN EXPRESSION\n");
			}
			stackNotermPush(&notermStack, actualNoterm);
			currentToken = tokenToPriority(tokenType);
			stackTermPush(&termStack, currentToken);
			if (DEBUG_FLAG) printf("noterminal pushed: %s\n", debugPreced(currentToken));
		} else {
			//TERMINAL CASE
			if (!(tokenType == OPERATION_PLUS	|| \
			tokenType == OPERATION_MINUS 		|| \
			tokenType == OPERATION_MULTIPLY 	|| \
			tokenType == OPERATION_DIVIDE 		|| \
			tokenType == OPERATION_CONCATEN 	|| \
			tokenType == COMPARE_IS 			|| \
			tokenType == COMPARE_IS_NOT 		|| \
			tokenType == COMPARE_LESS 			|| \
			tokenType == COMPARE_LESS_EQ 		|| \
			tokenType == COMPARE_MORE 			|| \
			tokenType == COMPARE_MORE_EQ 		|| \
			tokenType == LEFT_BRACKET 			|| \
			tokenType == RIGHT_BRACKET 			)) {
				//exit from expression
				currentToken = tokenToPriority(EXPRESSION_END);
				exitFlag = 1;
				if (DEBUG_FLAG) printf("We should exit\n");
			} else {
				currentToken = tokenToPriority(tokenType);
			}
			int priority = PTable[stackTermTop(&termStack)][currentToken];
			if (DEBUG_FLAG) printf("priority: %s\n",priorToStr(priority));
			switch (priority) {
				case PMore:
					while (priority!=PLess && priority!=PEqual) {
						int popedTerm = stackTermTop(&termStack);
						if (popedTerm==11) {	//vseeeee!!!!!! //check for rule E->id
							if (DEBUG_FLAG) printf("rule E->id, just pop\n");
							stackTermPop(&termStack);
						} else {
							if (DEBUG_FLAG) printf("activating rule for instruction\n");
							typeData * tempVar = getEmpty();
							typeData * operandSecond = stackNotermTop(&notermStack);
							stackNotermPop (&notermStack);
							typeData * operandOne = stackNotermTop(&notermStack);
							stackNotermPop (&notermStack);
							//CREATE INSTRUCTION!
							printf(CBLU "%i = %i %s %i\n" CNRM ,tempVar->valueOf.type_INTEGER,operandOne->valueOf.type_INTEGER,debugPreced(popedTerm),operandSecond->valueOf.type_INTEGER);
							createInstruction(tableToInstruction(popedTerm), tempVar, operandOne, operandSecond);
							stackNotermPush (&notermStack, tempVar);
							stackTermPop(&termStack);
						}
						priority = PTable[stackTermTop(&termStack)][currentToken];
						if (DEBUG_FLAG) printf("priority: %s\n",priorToStr(priority));
						if (priority == PError) {
							// if (!(currentToken == EXPRESSION_END && stackTermTop(&termStack) == EXPRESSION_END)) {
							if (DEBUG_FLAG) printf("error expression exit in PMore\n");
							return SYNTAX_WRONG;
							// }
						}
						if (priority == PEqual) {
							stackTermPop(&termStack);
						}
					}
					if (priority != PEqual) {
						stackTermPush(&termStack, currentToken);
					}
					break;
				case PLess:
					stackTermPush (&termStack, currentToken);
					break;
				case PEqual:
					stackTermPop(&termStack);
					break;
				case PError:
					if (DEBUG_FLAG) printf("error expression exit\n");
					return SYNTAX_WRONG;
				//Eequal neeesed
			}
		}
		if (exitFlag) {
			// ONE tDAta in nonterm stack for return

			RECOVER_TOKEN
			return SYNTAX_OK;
		} else {
			UPDATE_TOKEN
		}
	}
}

// 		// prepare stack nodes
// 		switch (tokenType) {
// 			case IDENTIFIER_VARIABLE:
// 			case LITERAL_NULL:
// 			case LITERAL_LOGICAL:
// 			case LITERAL_INETEGER:
// 			case LITERAL_DOUBLE:
// 			case LITERAL_STRING:
// 				//noterm at input
// 				// if (expectedType == TERM) {
// 				// 	if (DEBUG_FLAG) printf("unexepected noterm in expression\n");
// 				// 	return SYNTAX_WRONG;	//unexpected token
// 				// } else {
// 				// 	expectedType = TERM;
// 				// }
// 				if (tokenType == IDENTIFIER_VARIABLE) {
// 					actualNoterm = getVariable(&attribute, SHOULD_EXIST);
// 				} else {
// 					actualNoterm = getLiteral(tokenType-30, &attribute); //TOKEN -> DATA_TYPE
// 				}
// 				if (actualNoterm == NULL) {
// 					if (DEBUG_FLAG) printf("STOP! SEMANTIC ERROR! UNDEFINED VAR IN EXPRESSION\n");
// 				}
// 				if (DEBUG_FLAG) printf("noterminal pushed: %s\n", debugTokens(currentToken));
// 				stackNotermPush(&notermStack, actualNoterm);
// 				// lastType = currentType;
// 				// currentType = NOTERM;
// 				break;

// 			case OPERATION_PLUS:
// 			case OPERATION_MINUS:
// 			case OPERATION_MULTIPLY:
// 			case OPERATION_DIVIDE:
// 			case OPERATION_CONCATEN:
// 			case COMPARE_IS:
// 			case COMPARE_IS_NOT:
// 			case COMPARE_LESS:
// 			case COMPARE_MORE:
// 			case COMPARE_MORE_EQ:
// 				//term at input
// 				// if (expectedType == NOTERM) {
// 				// 	if (DEBUG_FLAG) printf("unexepected noterm in expression\n");
// 				// 	return SYNTAX_WRONG;	//unexpected token
// 				// } else {
// 				// 	expectedType = NOTERM;
// 				// }
// 				lastToken = tokenType;
// 				currentToken = tokenType;
				
// 				// lastType = currentType;
// 				// currentType = TERM;
// 				break;

// 			case LEFT_BRACKET:
// 				// firstly control tokens
// 				// if (currentType != NOTERM) {		//NOT noterm bcs can be first!!!
// 				// 	currentType = TERM;
// 				// 	expectedType = NOTERM; 		//next after ( will be NOTERM
// 				// } else {
// 				// 	if (DEBUG_FLAG) printf("unexepected left bracket\n");
// 				// 	return SYNTAX_WRONG;	//unexpected token
// 				// }
// 				//and than bracket counter
// 				++bracketsCounter;
// 				//update term
// 				// actualTerm = tokenType;
// 				// currentType = TERM;
// 				break;

// 			case RIGHT_BRACKET:
// 				// firstly control tokens
// 				// if (currentType != TERM) {		//NOT term bcs can be first!!! ?!WTF
// 				// 	currentType = NOTERM;
// 				// 	expectedType = TERM; 		//next after ( will be NOTERM
// 				// } else {
// 				// 	if (DEBUG_FLAG) printf("unexepected right bracket\n");
// 				// 	return SYNTAX_WRONG;	//unexpected token
// 				// }
// 				//and than bracket counter
// 				--bracketsCounter;
// 				if (bracketsCounter<0) {	//exit right bracket position
// 					if (DEBUG_FLAG) printf("exit from PP with recovered %s\n", debugTokens(tokenType));
// 					RECOVER_TOKEN
// 					return SYNTAX_OK;
// 				}
// 				//update term
// 				// actualTerm = tokenType;
// 				// currentType = TERM;
// 				break;

// 			default:
// 				//maybe token isn't for expression?
// 				if (bracketsCounter != 0) {
// 					if (DEBUG_FLAG) printf("exit from expression with brackets error\n");
// 					return SYNTAX_WRONG;	//unexpected token
// 				}
// 				RECOVER_TOKEN
// 				return SYNTAX_OK;
// 		}



// 		if (lastToken == WHATEVER) {	//just put first noterm to stack
// 			PUSH_CURRENT //first can be (
// 		} else {
// 			int priority = PTable[tokenToPriority(lastToken)][tokenToPriority(currentToken)];
// 			if (DEBUG_FLAG) printf("prtiority: %i\n",priority );
// 			//declaration before switch
// 			typeData * tempVar;
// 			typeData * operandOne;
// 			typeData * operandSecond;
// 			//switch
// 			switch (priority) {
// 				case PError:
// 					if (DEBUG_FLAG) printf("unresolved sequence in expression: %s %s\n", debugTokens(lastToken), debugTokens(currentToken));
// 					return SYNTAX_WRONG;
// 					break;
// 				case PLess:
// 					PUSH_CURRENT
// 					break;
// 				case PMore:
// 				case PEqual:
// 					// current rype should be term - noterm has more priority

// 					tempVar = getEmpty();
// 					operandOne = stackNotermTop(&notermStack);
// 					stackNotermPop (&notermStack);
// 					operandSecond = actualNoterm;
// 					//CREATE INSTRUCTION!
// 					printf(KBLU "%p = %p %s %p\n"KNRM,tempVar,operandOne,debugTokens(currentToken),operandSecond);
// 					stackNotermPush (&notermStack, tempVar);
// 					stackTermPop(&termStack);
// 					stackTermPush (&termStack, actualTerm);
// 					// }
// 					break;
				
// 			}
// 		}

// 		//ready to continue
// 		UPDATE_TOKEN
// 	}

// 	return SYNTAX_WRONG;
// }


// // 	printf("%s ", debugTokens(tokenType));

// // 	int status;
// // 	switch (tokenType) {
// // 		case IDENTIFIER_VARIABLE:
// // 		case LITERAL_NULL:
// // 		case LITERAL_LOGICAL:
// // 		case LITERAL_INETEGER:
// // 		case LITERAL_DOUBLE:
// // 		case LITERAL_STRING:
// // 		case OPERATION_PLUS:
// // 		case OPERATION_MINUS:
// // 		case OPERATION_MULTIPLY:
// // 		case OPERATION_DIVIDE:
// // 		case OPERATION_CONCATEN:
// // 		case COMPARE_IS:
// // 		case COMPARE_IS_NOT:
// // 		case COMPARE_LESS:
// // 		case COMPARE_MORE:
// // 		case COMPARE_MORE_EQ:
// // 			// <expression> -> <expression>
// // 			if (DEBUG_FLAG) printf("<expression> -> <expression>\n");
// // 			UPDATE_TOKEN
// // 			parserPrecedence();
// // 			return SYNTAX_OK;

// // 		default:
// // 			// <program_units>	-> EOF
// // 			if (DEBUG_FLAG) printf("<expression> -> E\n");
// // 			// instructions for this rule
// // 			RECOVER_TOKEN
// // 			return SYNTAX_OK;

// // 	}
// // 	return SYNTAX_WRONG;
// // }

// // int recursivePrecedence () {
// // 	return SYNTAX_OK;
// // }




















