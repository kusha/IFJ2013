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
#define AT_END 		0
#define AT_CURRENT	1


void createInstruction(int notEndFlag, int instrCode, typeData * addressOne, typeData * addressTwo, typeData * addressThree) {
	if (DEBUG_FLAG) printf("Called instruction creation: %i\n",instrCode);
	typeInstruction instruction;
	instruction.instrCode = instrCode;
	instruction.addressOne = addressOne;
	instruction.addressTwo = addressTwo;
	instruction.addressThree = addressThree;
	if (notEndFlag == 0) {
		if (listAdd(instrList, instruction)!=SUCCESS) {
			printf(CRED"Error in list addition\n"CNRM);
		}
	} else {
		if (listAddNext(instrList, instruction)!=SUCCESS) {
			printf(CRED"Error in list addition\n"CNRM);
		}
		listNext(instrList);	
	}
}

/* -- Function call tables ---------------------------------------------------
**
**
** -------------------------------------------------------------------------*/

typeCallsArray functionCalls;

typeNodePtr globalTable;
typeNodePtr functionsTable;
typeNodePtr * actualTable;

/* -- Parse starter --------------------------------------------------------*/

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

	//init tables
	initTables();

	//init calls array
	arrayCallsClear( &functionCalls );

	int statusCode;

	UPDATE_TOKEN
	statusCode = PROGRAM();

	//free attribute string
	strFree(&attribute);

	//adding calls instructions
	if (DEBUG_FLAG) printf("Start printing func calls.\n");
	int idx;
	for (idx=0; idx<=functionCalls.size; idx++) {
		listGoto(instrList, functionCalls.instr[idx]);
		
		typeData * currentFunction;
		currentFunction = getVariable(&functionsTable,
			&functionCalls.funcName[idx], SHOULD_EXIST);
		if (currentFunction == NULL) {
			REPORT("Undefined funciton call")
			return S_FUNC_ERROR;
		}

		typeData * returnPoint = getEmpty(&functionsTable);
		returnPoint->type = _FUNCTION;
		returnPoint->instruction  = NULL;

		createInstruction(AT_CURRENT, I_CALL, returnPoint, functionCalls.returnPtr[idx], NULL);

		int i = 0;
		typeData * forMerge1 = arrayGet(&currentFunction->funcWith.inputData, i);
		typeData * forMerge2;
		while (forMerge1 != NULL) {

			if ((forMerge2=arrayGet(&functionCalls.callParams[idx], i))==NULL) {
				REPORT("User func params error")
				return S_PARAM_ERROR;
			}
			createInstruction(AT_CURRENT, I_ASSIGN, forMerge1, forMerge2, NULL);
			i++;
			forMerge1 = arrayGet(&currentFunction->funcWith.inputData, i);
		}

		createInstruction(AT_CURRENT, I_GOTO, NULL, currentFunction, NULL);

		returnPoint->instruction = getPtrToActive(instrList); 

		// printf(CGRN"%p %p %p %p\n"CNRM, 
		// 	(void *) functionCalls.instr[idx],
		// 	(void *) &functionCalls.callParams[idx],
		// 	(void *) &functionCalls.funcName[idx],
		// 	(void *) functionCalls.returnPtr[idx]);
	}
	if (DEBUG_FLAG) printf("Finish printing func calls.\n");

	if (DEBUG_FLAG) printf("Parser complete with code %i\n", statusCode);
	return statusCode;
}



void initTables() {
	globalTable = createTable();
	functionsTable = createTable();
	actualTable = &globalTable;
}




/* -- Recursive descent ------------------------------------------------------
**
**	Input:		updated token + typeData pointer
**	Output:		instructions + result pointer + recovered token + result code
**
**	Desciption of the algorithm.
**
** -------------------------------------------------------------------------*/

int PROGRAM() {
	int status;
	switch (tokenType) {
		case PHP:
			// <program> -> PHP <program_units>
			if (DEBUG_FLAG) printf("<program> -> PHP <program_units>\n");

			createInstruction(AT_END, I_IDLE, NULL, NULL, NULL);

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
			createInstruction(AT_END, I_STOP, NULL, NULL, NULL);
			return SYNTAX_OK;
			break;

	}
	return SYNTAX_WRONG;
}



/* -- Function definition --------------------------------------------------*/

typeInputArray * currentFunctionInput;

int FUNC_DEFINE() {
	// <func_define> -> function id ( <params> ) { <cmd_sequence> }
	if (DEBUG_FLAG) printf("<func_define> -> function id ( <params> ) { <cmd_sequence> }\n");
	int status;
	IS_TOKEN(KEYWORD_FUNCTION)
	UPDATE_TOKEN
	IS_TOKEN(IDENTIFIER)


	//GOTO EXCEPT
	typeData * gotoData = getEmpty(actualTable);
	gotoData->type = _FUNCTION;
	gotoData->instruction  = NULL;
	createInstruction(AT_END, I_GOTO, NULL, gotoData, NULL);

	//COMMON CREATION
	typeData * currentFunction;
	currentFunction = getVariable(&functionsTable, &attribute, SHOULD_NOT_EXIST);
	if (currentFunction == NULL) {
		REPORT("Function redefinition")
		return S_FUNC_ERROR;
	}
	currentFunction->type = _FUNCTION;
	currentFunction->instruction = getPtrToCurrent(instrList);

	//TABLE
	typeNodePtr newTable;
	newTable = createTable();
	actualTable = &newTable;

	//INPUT
	typeInputArray functionInput;
	arrayClear( &functionInput );
	currentFunctionInput = &functionInput;	//set to global

	UPDATE_TOKEN
	IS_TOKEN(LEFT_BRACKET)
	UPDATE_TOKEN
	CALL(PARAMS)
	UPDATE_TOKEN
	IS_TOKEN(RIGHT_BRACKET)

	currentFunction->funcWith.inputData = functionInput;

	UPDATE_TOKEN
	IS_TOKEN(LEFT_CURLY_BRACKET)
	UPDATE_TOKEN
	CALL(CMD_SEQUENCE)

	typeData * retNull = getEmpty(actualTable);
	retNull->type = _NULL;
	retNull->valueOf.type_NULL = NULL;
	createInstruction(AT_END, I_RETURN, retNull, NULL, NULL);

	UPDATE_TOKEN
	IS_TOKEN(RIGHT_CURLY_BRACKET)

	actualTable = &globalTable;
	gotoData->instruction  = getPtrToCurrent(instrList); 


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


			typeData * currentParam = getVariable(actualTable, &attribute, SHOULD_NOT_EXIST);
			arrayAdd( currentFunctionInput, currentParam);


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

			typeData * currentParam = getVariable(actualTable, &attribute, SHOULD_NOT_EXIST);
			arrayAdd( currentFunctionInput, currentParam);

			UPDATE_TOKEN
			CALL(PARAMS_MORE)
			return SYNTAX_OK;
			break;

	}
	return SYNTAX_WRONG;
}

/* -------------------------------------------------------------------------*/


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

/* -- Real commands --------------------------------------------------------*/

typeData * expressionResult;
typeInputArray inputArray;
string resultSaver;
typeData * resultVar;

int CMD() {
	int status;
	switch (tokenType) {
		case IDENTIFIER_VARIABLE: {
			// <cmd> -> $id = <expression> ;
			// <cmd> -> $id = id ( <input> ) ;
			strInit(&resultSaver); 
			if (strCopy(&resultSaver, &attribute)==STR_ERROR) return INTERNAL_ERROR;

			UPDATE_TOKEN
			IS_TOKEN(OPERATION_ASSIGN)
			UPDATE_TOKEN
			switch (tokenType) {
				case IDENTIFIER:
					// <cmd> -> $id = id ( <input> ) ;
					if (DEBUG_FLAG) printf("<cmd> -> $id = id ( <input> ) ;\n");

					string nameSaver;
					strInit(&nameSaver); 
					if (strCopy(&nameSaver, &attribute)==STR_ERROR) return INTERNAL_ERROR;

					arrayClear( &inputArray );
					UPDATE_TOKEN
					IS_TOKEN(LEFT_BRACKET)
					UPDATE_TOKEN
					CALL(INPUT)
					UPDATE_TOKEN
					IS_TOKEN(RIGHT_BRACKET)

					resultVar = getVariable(actualTable, &resultSaver, MAY_NOT_EXIST);

					string convertHelper;
					strInit(&convertHelper); 


					//CHECK FOR BUILT-IN
					if (strCompareConst(&nameSaver, "boolval")) {
						if (strAddChar(&convertHelper, 48+_LOGICAL)==STR_ERROR) return INTERNAL_ERROR;
						typeData * target = getLiteral(actualTable, _INTEGER, &convertHelper);
						typeData * converted;
						if ((converted = arrayGet(&inputArray, 0))==NULL) {
							REPORT("too few params")
							return S_PARAM_ERROR;
						}
						createInstruction(AT_END, I_CONVERT, resultVar, converted, target);
					} else if (strCompareConst(&nameSaver, "doubleval")) {
						if (strAddChar(&convertHelper, 48+_DOUBLE)==STR_ERROR) return INTERNAL_ERROR;
						typeData * target = getLiteral(actualTable, _INTEGER, &convertHelper);
						typeData * converted;
						if ((converted = arrayGet(&inputArray, 0))==NULL) {
							REPORT("too few params")
							return S_PARAM_ERROR;
						}
						createInstruction(AT_END, I_CONVERT, resultVar, converted, target);
					} else if (strCompareConst(&nameSaver, "intval")) {
						if (strAddChar(&convertHelper, 48+_INTEGER)==STR_ERROR) return INTERNAL_ERROR;
						typeData * target = getLiteral(actualTable, _INTEGER, &convertHelper);
						typeData * converted;
						if ((converted = arrayGet(&inputArray, 0))==NULL) {
							REPORT("too few params")
							return S_PARAM_ERROR;
						}
						createInstruction(AT_END, I_CONVERT, resultVar, converted, target);
					} else if (strCompareConst(&nameSaver, "strval")) {
						if (strAddChar(&convertHelper, 48+_STRING)==STR_ERROR) return INTERNAL_ERROR;
						typeData * target = getLiteral(actualTable, _INTEGER, &convertHelper);
						typeData * converted;
						if ((converted = arrayGet(&inputArray, 0))==NULL) {
							REPORT("too few params")
							return S_PARAM_ERROR;
						}
						createInstruction(AT_END, I_CONVERT, resultVar, converted, target);
					} else if (strCompareConst(&nameSaver, "get_string")) {
						createInstruction(AT_END, I_READ, resultVar, NULL, NULL);
					} else if (strCompareConst(&nameSaver, "put_string")) {
						int idx = 0;
						typeData * forPrint= arrayGet(&inputArray, idx);
						while (forPrint != NULL) {
							createInstruction(AT_END, I_WRITE, forPrint, NULL, NULL);
							idx++;
							forPrint= arrayGet(&inputArray, idx);
						}
						char str[MAX_PUT_STRING];
						sprintf(str, "%d", idx);
						int i = 0;
						while (str[i]!='\0') {
							strAddChar(&convertHelper, str[i]);
							i++;
						} 
						typeData * counter = getLiteral(actualTable, _INTEGER, &convertHelper);
						createInstruction(AT_END, I_ASSIGN, resultVar, counter, NULL);
					} else if (strCompareConst(&nameSaver, "strlen")) {
						typeData * inputString;
						if ((inputString = arrayGet(&inputArray, 0))==NULL) {
							REPORT("too few params")
							return S_PARAM_ERROR;
						}
						createInstruction(AT_END, I_STR_LEN, resultVar, inputString, NULL);
					} else if (strCompareConst(&nameSaver, "get_substring")) {
						typeData * inputString;
						typeData * subStart;
						typeData * subFinish;
						if ((inputString = arrayGet(&inputArray, 0))==NULL) {
							REPORT("too few params")
							return S_PARAM_ERROR;
						}
						if ((subStart = arrayGet(&inputArray, 1))==NULL) {
							REPORT("too few params")
							return S_PARAM_ERROR;
						}
						if ((subFinish = arrayGet(&inputArray, 2))==NULL) {
							REPORT("too few params")
							return S_PARAM_ERROR;
						}
						createInstruction(AT_END, I_SUB_STR, NULL, NULL, subStart);
						createInstruction(AT_END, I_SUB_STR, resultVar, inputString, subFinish);
					} else if (strCompareConst(&nameSaver, "find_string")) {
						typeData * inputString1;
						typeData * inputString2;
						if ((inputString1 = arrayGet(&inputArray, 0))==NULL) {
							REPORT("too few param")
							return S_PARAM_ERROR;
						}
						if ((inputString2 = arrayGet(&inputArray, 1))==NULL) {
							REPORT("too few param")
							return S_PARAM_ERROR;
						}
						createInstruction(AT_END, I_FIND_STR, resultVar, inputString1, inputString2);
					} else if (strCompareConst(&nameSaver, "sort_string")) {
						typeData * inputString;
						if ((inputString = arrayGet(&inputArray, 0))==NULL) {
							REPORT("too few params")
							return S_PARAM_ERROR;
						}
						createInstruction(AT_END, I_SORT_STR, resultVar, inputString, NULL);
					} else {															//user function
							
							arrayCallsAdd( &functionCalls,  getPtrToCurrent(instrList),
								inputArray, &nameSaver , resultVar);
							/*
							typeData * currentFunction;
							currentFunction = getVariable(&functionsTable, &nameSaver, SHOULD_EXIST);
							if (currentFunction == NULL) {
								REPORT("Undefined funciton call")
								return S_FUNC_ERROR;
							}

							typeData * returnPoint = getEmpty(actualTable);
							returnPoint->type = _FUNCTION;
							returnPoint->instruction  = NULL;
							createInstruction(I_CALL, returnPoint, resultVar, NULL);

							int idx = 0;
							typeData * forMerge1 = arrayGet(&inputArray, idx);
							typeData * forMerge2;
							while (forMerge1 != NULL) {

								if ((forMerge2=arrayGet(&currentFunction->funcWith.inputData, idx))==NULL) {
									REPORT("User func params error")
									return S_PARAM_ERROR;
								}
								createInstruction(I_ASSIGN, forMerge2, forMerge1, NULL);
								idx++;
								forMerge1 = arrayGet(&inputArray, idx);
							}

							createInstruction(I_GOTO, NULL, currentFunction, NULL);

							returnPoint->instruction = getPtrToCurrent(instrList); 
							*/

					}
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

					resultVar = getVariable(actualTable, &resultSaver, MAY_NOT_EXIST);
					createInstruction(AT_END, I_ASSIGN, resultVar, expressionResult, NULL);

					UPDATE_TOKEN
					IS_TOKEN(SEMICOLON)
					return SYNTAX_OK;
					break;
			}
			return SYNTAX_WRONG;
			break; }

		case KEYWORD_IF:
			// <cmd> -> if ( <expression> ) { <cmd_sequence> } else { <cmd_sequence> }
			if (DEBUG_FLAG) printf("<cmd> -> if ( <expression> ) { <cmd_sequence> } else { <cmd_sequence> }\n");
			UPDATE_TOKEN
			IS_TOKEN(LEFT_BRACKET)
			UPDATE_TOKEN
			CALL(PRECEDENCE)

			typeData * gotoDataIf = getEmpty(actualTable);
			gotoDataIf->type = _FUNCTION;
			gotoDataIf->instruction  = NULL;

			typeData * gotoDataElse = getEmpty(actualTable);
			gotoDataElse->type = _FUNCTION;
			gotoDataElse->instruction  = NULL;

			typeData * gotoDataExit = getEmpty(actualTable);
			gotoDataExit->type = _FUNCTION;
			gotoDataExit->instruction  = NULL;

			createInstruction(AT_END, I_GOTO_IF, expressionResult, gotoDataIf, gotoDataElse);

			UPDATE_TOKEN
			IS_TOKEN(RIGHT_BRACKET)
			UPDATE_TOKEN
			IS_TOKEN(LEFT_CURLY_BRACKET)

			gotoDataIf->instruction = getPtrToCurrent(instrList);

			UPDATE_TOKEN
			CALL(CMD_SEQUENCE)
			UPDATE_TOKEN
			IS_TOKEN(RIGHT_CURLY_BRACKET)

			createInstruction(AT_END, I_GOTO, NULL, gotoDataExit, NULL);

			UPDATE_TOKEN
			IS_TOKEN(KEYWORD_ELSE)
			UPDATE_TOKEN
			IS_TOKEN(LEFT_CURLY_BRACKET)

			gotoDataElse->instruction = getPtrToCurrent(instrList);

			UPDATE_TOKEN
			CALL(CMD_SEQUENCE)
			UPDATE_TOKEN
			IS_TOKEN(RIGHT_CURLY_BRACKET)

			gotoDataExit->instruction = getPtrToCurrent(instrList);

			return SYNTAX_OK;
			break;

		case KEYWORD_WHILE:
			// <cmd> -> while ( <expression> ) { <cmd_sequence> }
			if (DEBUG_FLAG) printf("<cmd> -> while ( <expression> ) { <cmd_sequence> }\n");
			UPDATE_TOKEN
			IS_TOKEN(LEFT_BRACKET)

			typeData * gotoDataRepeat = getEmpty(actualTable);
			gotoDataRepeat->type = _FUNCTION;
			gotoDataRepeat->instruction = getPtrToCurrent(instrList);


			UPDATE_TOKEN
			CALL(PRECEDENCE)


			typeData * gotoDataDo = getEmpty(actualTable);
			gotoDataDo->type = _FUNCTION;
			gotoDataDo->instruction  = NULL;

			typeData * gotoDataBreak = getEmpty(actualTable);
			gotoDataBreak->type = _FUNCTION;
			gotoDataBreak->instruction  = NULL;

			createInstruction(AT_END, I_GOTO_IF, expressionResult, gotoDataDo, gotoDataBreak);

			gotoDataDo->instruction = getPtrToCurrent(instrList);


			UPDATE_TOKEN
			IS_TOKEN(RIGHT_BRACKET)
			UPDATE_TOKEN
			IS_TOKEN(LEFT_CURLY_BRACKET)
			UPDATE_TOKEN
			CALL(CMD_SEQUENCE)
			UPDATE_TOKEN

			createInstruction(AT_END, I_GOTO, NULL, gotoDataRepeat, NULL);

			IS_TOKEN(RIGHT_CURLY_BRACKET)

			gotoDataBreak->instruction = getPtrToCurrent(instrList);

			return SYNTAX_OK;
			break;

		case KEYWORD_RETURN:
			// <cmd> -> return <expression> ;
			if (DEBUG_FLAG) printf("<cmd> -> return <expression> ;\n");
			UPDATE_TOKEN
			CALL(PRECEDENCE)

			createInstruction(AT_END, I_RETURN, expressionResult, NULL, NULL);

			UPDATE_TOKEN
			IS_TOKEN(SEMICOLON)
			return SYNTAX_OK;
			break;

	}
	return SYNTAX_WRONG;
}

/* -------------------------------------------------------------------------*/

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

			arrayAdd( &inputArray, expressionResult);

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

			arrayAdd( &inputArray, expressionResult);

			UPDATE_TOKEN
			CALL(INPUT_MORE)
			return SYNTAX_OK;
			break;

	}
	return SYNTAX_WRONG;
}

/* -------------------------------------------------------------------------*/

/* -- Precedence Parser ------------------------------------------------------
**
**	Input:		updated token + typeData pointer
**	Output:		instructions + result pointer + recovered token + result code
**
**	Desciption of the algorithm.
**
** -------------------------------------------------------------------------*/




/* -- Macro definitions ----------------------------------------------------*/

 
#define CLOSE_BRACKET 13

#define EXPRESSION_END 14

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

static int priorityTable [15][15] = {
/* INCOMING > *	 /	 +   -   .   <   >  <=  >= === !==  ID   (   )   $ /*/
/*/	 *	/*/	 {M	,M	,M	,M	,W	,M	,M	,M	,M	,M	,M	,L	,L	,M 	,M } ,
/*/	 /	/*/	 {M	,M	,M	,M	,W	,M	,M	,M	,M	,M	,M	,L	,L	,M 	,M } ,
/*/	 +	/*/	 {L	,L	,M	,M	,W	,M	,M	,M	,M	,M	,M	,L	,L	,M 	,M } ,
/*/	 -	/*/	 {L	,L	,M	,M	,W	,M	,M	,M	,M	,M	,M	,L	,L	,M 	,M } ,
/*/	 .	/*/	 {W	,W	,W	,W	,M	,M	,M	,M	,M	,M	,M	,L	,L	,M 	,M } ,
/*/	 <	/*/	 {L	,L	,L	,L	,L	,M	,M	,M	,M	,M	,M	,L	,L	,M	,M } ,
/*/	 >	/*/	 {L	,L	,L	,L	,L	,M	,M	,M	,M	,M	,M	,L	,L	,M	,M } ,
/*/	 <=	/*/	 {L	,L	,L	,L	,L	,M	,M	,M	,M	,M	,M	,L	,L	,M	,M } ,
/*/	 >=	/*/	 {L	,L	,L	,L	,L	,M	,M	,M	,M	,M	,M	,L	,L	,M	,M } ,
/*/	=== /*/	 {L	,L	,L	,L	,L	,L	,L	,L	,L	,M	,M	,L	,L	,M	,M } ,
/*/	!==	/*/	 {L	,L	,L	,L	,L	,L	,L	,L	,L	,M	,M	,L	,L	,M	,M } ,
/*/	 ID	/*/	 {M	,M	,M	,M	,M	,M	,M	,M	,M	,M	,M	,W	,W	,M 	,M } ,
/*/	 (	/*/	 {L	,L	,L	,L	,L	,L	,L	,L	,L	,L	,L	,L	,L	,E	,M } ,
/*/	 )	/*/	 {M	,M	,M	,M	,M	,M	,M	,M	,M	,M	,M	,W	,W	,W	,M } ,
/*/	 $	/*/	 {L	,L	,L	,L	,L	,L	,L	,L	,L	,L	,L	,L	,L	,W 	,E }
/* ^^^^^ */
/*IN_STACK*/
};

#undef M
#undef L
#undef E
#undef W


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
		case LITERAL_STRING: 		return 11;
		case LEFT_BRACKET: 			return 12;
		case RIGHT_BRACKET: 		return 13;
		case EXPRESSION_END: 		return 14;
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
		case 12:	return "LEFT_BRACKET"			;
		case 13:	return "RIGHT_BRACKET"			;
		case 14:	return "EXPRESSION_END"			;
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
		default: 	return "WARNING		";
	}
}

char * priorToStr (int priority) {
	switch (priority){
		case PLess:		return "LESS";
		case PMore:		return "MORE";
		case PEqual:	return "EQUAL";
		case PError:	return "ERROR";
		default: 		return "WARNING";
	}
}


#define TERM 		1 
#define NOTERM 		0 
#define WHATEVER	-1

int parserPrecedence() {

	int currentToken = WHATEVER;

	tStackTerm termStack;
	stackTermInit(&termStack);

	tStackNoterm notermStack;
	stackNotermInit(&notermStack);

	stackTermPush(&termStack, EXPRESSION_END);


	typeData * actualNoterm;

	int exitFlag = 0;

	int popedTerm;

	while (1) {

		if (tokenType == IDENTIFIER_VARIABLE 	|| \
			tokenType == LITERAL_NULL 			|| \
			tokenType == LITERAL_LOGICAL 		|| \
			tokenType == LITERAL_INETEGER 		|| \
			tokenType == LITERAL_DOUBLE 		|| \
			tokenType == LITERAL_STRING ) {
			if (tokenType == IDENTIFIER_VARIABLE) {
				actualNoterm = getVariable(actualTable, &attribute, SHOULD_EXIST);
				if (actualNoterm == NULL) {
					REPORT("Variable not declared")
					return S_DECLAR_ERROR;
				}
			} else {
				actualNoterm = getLiteral(actualTable, tokenType-30, &attribute);
			}
			if (actualNoterm == NULL) {
				if (DEBUG_FLAG) printf("STOP! SEMANTIC ERROR! UNDEFINED VAR IN EXPRESSION\n");
			}
			stackNotermPush(&notermStack, actualNoterm);
			currentToken = tokenToPriority(tokenType);
			stackTermPush(&termStack, currentToken);
			if (DEBUG_FLAG) printf("noterminal pushed: %s\n", debugPreced(currentToken));
		} else {
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
				currentToken = tokenToPriority(EXPRESSION_END);
				exitFlag = 1;
				if (DEBUG_FLAG) printf("We should exit\n");
			} else {
				currentToken = tokenToPriority(tokenType);
			}
			popedTerm = stackTermTop(&termStack);
			if (popedTerm == -1) {
				//wrong expression
				return SYNTAX_WRONG;
			}
			int priority = priorityTable[popedTerm][currentToken];
			if (DEBUG_FLAG) printf("priority: %s\n",priorToStr(priority));
			switch (priority) {
				case PMore:
					while (priority!=PLess && priority!=PEqual) {
						popedTerm = stackTermTop(&termStack);
						if (popedTerm == -1) {
							//wrong expression
							return SYNTAX_WRONG;
						}
						if (popedTerm==11) {
							if (DEBUG_FLAG) printf("rule E->id, just pop\n");
							stackTermPop(&termStack);
						} else {
							if (DEBUG_FLAG) printf("activating rule for instruction\n");
							typeData * tempVar = getEmpty(actualTable);
							typeData * operandSecond = stackNotermTop(&notermStack);
							stackNotermPop (&notermStack);
							typeData * operandOne = stackNotermTop(&notermStack);
							stackNotermPop (&notermStack);
							if (operandSecond==NULL||operandOne==NULL) {
								REPORT("Not enough operands in stack, error in expression")
								return SYNTAX_WRONG;
							}
							createInstruction(AT_END, tableToInstruction(popedTerm), tempVar, operandOne, operandSecond);
							stackNotermPush (&notermStack, tempVar);
							stackTermPop(&termStack);
						}
						popedTerm = stackTermTop(&termStack);
						if (popedTerm == -1) {
							//wrong expression
							return SYNTAX_WRONG;
						}
						priority = priorityTable[popedTerm][currentToken];
						if (DEBUG_FLAG) printf("priority: %s\n",priorToStr(priority));
						if (priority == PError) {
							if (currentToken == CLOSE_BRACKET && popedTerm == EXPRESSION_END) {
								exitFlag = 1;
								currentToken = tokenToPriority(tokenType);
 								break;
							} else {
								if (DEBUG_FLAG) printf("error expression exit in PMore\n");
 								return SYNTAX_WRONG;  
 							}
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
			}
		}
		if (exitFlag) {
			expressionResult = stackNotermTop(&notermStack);
			stackNotermPop(&notermStack);
			if (expressionResult==NULL) {
				//wrong expression
				return SYNTAX_WRONG;
			}
			if (stackNotermTop(&notermStack) != NULL) {
				//one more variable: id id
				return SYNTAX_WRONG;
			}
			stackNotermPop (&notermStack);
			RECOVER_TOKEN
			return SYNTAX_OK;
		} else {
			UPDATE_TOKEN
		}
	}
}
